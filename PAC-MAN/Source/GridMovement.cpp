//------------------------------------------------------------------------------
//
// File Name:	GridMovement.cpp
// Author(s):	David Cohen (david.cohen)
// Project:		BetaFramework
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
		tilemap(nullptr), spriteTilemap(nullptr), transform(nullptr), tileProgress(0.0f), direction(UP), oldTile(), newTile()
	{
	}

	// Clone a component and return a pointer to the cloned component.
	// Returns:
	//   A pointer to a dynamically allocated clone of the component.
	Component* GridMovement::Clone() const
	{
		return new GridMovement(*this);
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
			AdjacentTile adjacentTiles[4];
			size_t emptyCount;
			GetAdjacentTiles(adjacentTiles, emptyCount);

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
			if (tilemap->GetCellValue(static_cast<unsigned>(newTile.x), static_cast<unsigned>(newTile.y)) > 0)
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

	// Fills out a Vector2D array with all adjacent tile coordinates.
	// Params:
	//   tiles = The array of tiles. Must have a size of 4!
	//   emptyCount = How many empty tiles were found.
	void GridMovement::GetAdjacentTiles(AdjacentTile tiles[4], size_t& emptyCount)
	{
		emptyCount = 0;

		tiles[0] = { Vector2D(newTile.x, newTile.y - 1), true, true, UP };

		// Check tile above.
		GetAdjacentTile(tiles[0]);
		if (tiles[0].empty)
			++emptyCount;

		tiles[1] = { Vector2D(newTile.x - 1, newTile.y), true, true, LEFT };

		// Check tile to the left.
		GetAdjacentTile(tiles[1]);
		if (tiles[1].empty)
			++emptyCount;

		tiles[2] = { Vector2D(newTile.x, newTile.y + 1), true, true, DOWN };

		// Check tile below.
		GetAdjacentTile(tiles[2]);
		if (tiles[2].empty)
			++emptyCount;

		tiles[3] = { Vector2D(newTile.x + 1, newTile.y), true, true, RIGHT };

		// Check tile to the right.
		GetAdjacentTile(tiles[3]);
		if (tiles[3].empty)
			++emptyCount;
	}

	// Fills out an AdjacentTile struct.
	// Params:
	//   tile = The tile to fill out.
	void GridMovement::GetAdjacentTile(AdjacentTile& tile)
	{
		if (tile.pos.x >= 0 && tile.pos.y >= 0 && tile.pos.x < tilemap->GetWidth() && tile.pos.y < tilemap->GetHeight())
		{
			tile.edge = false;
			tile.empty = tilemap->GetCellValue(static_cast<unsigned>(tile.pos.x), static_cast<unsigned>(tile.pos.y)) == 0;
		}
	}

	// Called when finished moving to the next tile.
	// Params:
	//   adjacentTiles = An array of adjacent tiles.
	//   emptyCount = How many empty tiles were found.
	void GridMovement::OnTileMove(AdjacentTile adjacentTiles[4], size_t emptyCount)
	{
		// If there are more than 2 directions we could move, let the child class handle the intersection.
		if (emptyCount > 2)
			OnIntersection(adjacentTiles, emptyCount);
	}

	// Called when met with an intersection after finishing moving to the next tile.
	// Params:
	//   adjacentTiles = An array of adjacent empty tiles.
		//   emptyCount = How many empty tiles were found.
	void GridMovement::OnIntersection(AdjacentTile adjacentTiles[4], size_t emptyCount)
	{
		UNREFERENCED_PARAMETER(adjacentTiles);
		UNREFERENCED_PARAMETER(emptyCount);
	}
}

//------------------------------------------------------------------------------
