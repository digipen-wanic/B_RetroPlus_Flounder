//------------------------------------------------------------------------------
//
// File Name:	GridMovement.cpp
// Author(s):	David Cohen (david.cohen)
// Project:		PAC-MAN
// Course:		WANIC VGP2 2018-2019
//
// Copyright © 2018 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "GridMovement.h"

// Systems
#include <Parser.h>
#include <Interpolation.h>
#include <GameObject.h>
#include <Tilemap.h>

// Components
#include <Transform.h>
#include <SpriteTilemap.h>

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

namespace Behaviors
{
	//------------------------------------------------------------------------------
	// Public Functions:
	//------------------------------------------------------------------------------

	// Constructor
	// Params:
	//   speed = How fast the game object moves between tiles.
	GridMovement::GridMovement(float speed) : Component("GridMovement"), speed(speed),
		tilemap(nullptr), spriteTilemap(nullptr), transform(nullptr), tileProgress(0.0f), direction(RIGHT), oldTile(), newTile()
	{
	}

	// Initialize this component (happens at object creation).
	void GridMovement::Initialize()
	{
		// Store the required components for ease of access.
		transform = GetOwner()->GetComponent<Transform>();

		Vector2D tileSpace = spriteTilemap->WorldToTile(transform->GetTranslation());
		oldTile = newTile = Vector2D(floor(tileSpace.x), floor(tileSpace.y));
	}

	// Updates components using a fixed timestep (usually just physics)
	// Params:
	//	 dt = A fixed change in time, usually 1/60th of a second.
	void GridMovement::FixedUpdate(float dt)
	{
		tileProgress += dt * speed;

		// Check if we have reached the end of this movement.
		if (tileProgress > 1.0f)
		{
			bool currentTileValid;
			GetCellValue(newTile, currentTileValid);
			if (currentTileValid)
			{
				AdjacentTile adjacentTiles[4];
				size_t emptyCount;
				GetAdjacentTiles(adjacentTiles, emptyCount);

				// If there are more than 2 directions we could move, let the child class handle the intersection.
				if (emptyCount > 2)
					OnIntersection(adjacentTiles, emptyCount);

				// Give child class a chance to update direction when we have reached the end of a move.
				OnTileMove(adjacentTiles, emptyCount);

				oldTile = newTile;

				// Calculate the new tile based on the target direction.
				switch (direction)
				{
				case UP:
					newTile = oldTile + Vector2D(0.0f, -1.0f);
					break;
				case LEFT:
					newTile = oldTile + Vector2D(-1.0f, 0.0f);
					break;
				case DOWN:
					newTile = oldTile + Vector2D(0.0f, 1.0f);
					break;
				case RIGHT:
					newTile = oldTile + Vector2D(1.0f, 0.0f);
					break;
				}

				// If the tile in the target direction is not empty, cancel the movement.
				bool newTileValid;
				if (GetCellValue(newTile, newTileValid) > 0)
					newTile = oldTile;

				// If a new tile has been chosen, subtract 1 from tile movement progress so that we interpolate the correct amount.
				if (!AlmostEqual(oldTile, newTile))
				{
					tileProgress -= 1.0f;
				}
				// If the tile is the same (nowhere to go) clamp tile movement progress to 1.
				else
				{
					tileProgress = 1.0f;
				}
			}
			// If the current coordinate is outside the map, wrap around.
			else
			{
				// The old tile is closer to the center and the new tile is off the screen.
				// We want to have the game object appear from off-screen, so we swap the
				// old tile and new tile first (so the old tile is off the screen and new
				// tile is closer to the center)

				std::swap(oldTile, newTile);
				
				// Mirror the coordinates around the axis we are moving.
				switch (direction)
				{
				case UP:
				case DOWN:
					newTile = Vector2D(newTile.x, tilemap->GetHeight() - newTile.y - 1);
					oldTile = Vector2D(oldTile.x, tilemap->GetHeight() - oldTile.y - 1);
					break;
				case LEFT:
				case RIGHT:
					newTile = Vector2D(tilemap->GetWidth() - newTile.x - 1, newTile.y);
					oldTile = Vector2D(tilemap->GetWidth() - oldTile.x - 1, oldTile.y);
					break;
				}

				tileProgress -= 1.0f;
			}
		}

		// Linearly interpolate from the old tile to the new tile based on how much tile movement progress we have.
		Vector2D tileSpace = Interpolate(oldTile, newTile, std::min(1.0f, tileProgress));
		Vector2D worldSpace = spriteTilemap->TileToWorld(tileSpace);
		transform->SetTranslation(worldSpace);
	}

	// Write object data to file
	// Params:
	//   parser = The parser that is writing this object to a file.
	void GridMovement::Serialize(Parser& parser) const
	{
		parser.WriteVariable("speed", speed);
	}

	// Read object data from a file
	// Params:
	//   parser = The parser that is reading this object's data from a file.
	void GridMovement::Deserialize(Parser& parser)
	{
		parser.ReadVariable("speed", speed);
	}

	// Sets the speed of the game object.
	void GridMovement::SetSpeed(float speed_)
	{
		speed = speed_;
	}

	// Gets the speed of the game object.
	float GridMovement::GetSpeed() const
	{
		return speed;
	}

	// Sets whether the game object is frozen.
	void GridMovement::SetFrozen(bool frozen_)
	{
		frozen = frozen_;
	}

	// Gets whether the game object is frozen.
	bool GridMovement::IsFrozen() const
	{
		return frozen;
	}

	// Sets the tilemap used for the grid.
	// Params:
	//   tilemap = The tilemap.
	//   spriteTilemap = The sprite tilemap.
	void GridMovement::SetTilemap(Tilemap* tilemap_, SpriteTilemap* spriteTilemap_)
	{
		tilemap = tilemap_;
		spriteTilemap = spriteTilemap_;
	}

	//------------------------------------------------------------------------------
	// Protected Functions:
	//------------------------------------------------------------------------------

	// Gets the old tile.
	Vector2D GridMovement::GetOldTile() const
	{
		return oldTile;
	}

	// Gets the tilemap (constant).
	const Tilemap* GridMovement::GetTilemap() const
	{
		return tilemap;
	}

	// Gets the sprite tilemap (constant).
	const SpriteTilemap* GridMovement::GetSpriteTilemap() const
	{
		return spriteTilemap;
	}

	// Fills out a Vector2D array with all adjacent tile coordinates.
	// Params:
	//   tiles = The array of tiles. Must have a size of 4!
	//   emptyCount = How many empty tiles were found.
	void GridMovement::GetAdjacentTiles(AdjacentTile tiles[4], size_t& emptyCount)
	{
		// Check tile above.
		tiles[0] = GetAdjacentTile(Vector2D(newTile.x, newTile.y - 1), UP);

		// Check tile to the left.
		tiles[1] = GetAdjacentTile(Vector2D(newTile.x - 1, newTile.y), LEFT);

		// Check tile below.
		tiles[2] = GetAdjacentTile(Vector2D(newTile.x, newTile.y + 1), DOWN);

		// Check tile to the right.
		tiles[3] = GetAdjacentTile(Vector2D(newTile.x + 1, newTile.y), RIGHT);

		// Count the number of empty tiles.
		emptyCount = 0;
		for (size_t i = 0; i < 4; i++)
			if (tiles[i].empty)
				++emptyCount;
	}

	// Creates an AdjacentTile struct.
	// Params:
	//   pos = The tile's coordinate.
	//   direction = The direction to move to get to this tile.
	// Returns:
	//   The filled out AdjacentTile struct.
	GridMovement::AdjacentTile GridMovement::GetAdjacentTile(Vector2D pos, Direction direction_)
	{
		AdjacentTile tile = { pos, true, true, direction_ };
		bool valid;
		tile.empty = GetCellValue(tile.pos, valid) == 0;
		tile.edge = !valid;
		return tile;
	}

	// Helper function to get the cell value at the specified coordinate. If the coordinate was not valid, it returns 0.
	// Params:
	//   pos = The coordinate to get the cell value at.
	//   valid = Whether the coordinate was valid or not.
	// Returns:
	//   The cell value at the specified coordinate. If the coordinate was not valid, returns 0.
	int GridMovement::GetCellValue(Vector2D pos, bool& valid)
	{
		if (pos.x >= 0 && pos.y >= 0 && pos.x < tilemap->GetWidth() && pos.y < tilemap->GetHeight())
		{
			valid = true;
			return tilemap->GetCellValue(static_cast<unsigned>(pos.x), static_cast<unsigned>(pos.y));
		}
		else
		{
			valid = 0;
			return 0;
		}
	}
}

//------------------------------------------------------------------------------
