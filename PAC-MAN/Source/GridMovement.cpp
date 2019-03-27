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
			size_t adjacentTilesSize;
			GetAdjacentEmptyTiles(adjacentTiles, adjacentTilesSize);

			// Give child class a chance to update direction when we have reached the end of a move.
			OnTileMove(adjacentTiles, adjacentTilesSize);

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
	//   tilesSize = How many tiles were written.
	void GridMovement::GetAdjacentTiles(AdjacentTile tiles[4], size_t& tilesSize)
	{
		tilesSize = 0;

		// Check tile above.
		if (newTile.y - 1 >= 0)
			tiles[tilesSize++] = { Vector2D(newTile.x, newTile.y - 1), UP };

		// Check tile to the left.
		if (newTile.x - 1 >= 0)
			tiles[tilesSize++] = { Vector2D(newTile.x - 1, newTile.y), LEFT };

		// Check tile below.
		if (newTile.y + 1 < tilemap->GetHeight())
			tiles[tilesSize++] = { Vector2D(newTile.x, newTile.y + 1), DOWN };

		// Check tile to the right.
		if (newTile.x + 1 < tilemap->GetWidth())
			tiles[tilesSize++] = { Vector2D(newTile.x + 1, newTile.y), RIGHT };
	}

	// Fills out a Vector2D array with all adjacent empty tile coordinates.
	// Params:
	//   tiles = The array of tiles. Must have a size of 4!
	//   tilesSize = How many tiles were written.
	void GridMovement::GetAdjacentEmptyTiles(AdjacentTile tiles[4], size_t& tilesSize)
	{
		tilesSize = 0;

		AdjacentTile adjacentTiles[4];
		size_t adjacentTilesSize;

		// Get all valid adjacent tiles.
		GetAdjacentTiles(adjacentTiles, adjacentTilesSize);

		// Check if each adjacent tile is empty, and if so add it to the array.
		for (size_t i = 0; i < adjacentTilesSize; i++)
			if (tilemap->GetCellValue(static_cast<unsigned>(adjacentTiles[i].pos.x), static_cast<unsigned>(adjacentTiles[i].pos.y)) == 0)
				tiles[tilesSize++] = adjacentTiles[i];
	}

	// Called when finished moving to the next tile.
	// Params:
	//   adjacentTiles = An array of adjacent empty tiles.
	//   adjacentTilesSize = The number of elements in the array of adjacent empty tiles.
	void GridMovement::OnTileMove(AdjacentTile adjacentTiles[4], size_t adjacentTilesSize)
	{
		// If there are more than 2 directions we could move, let the child class handle the intersection.
		if (adjacentTilesSize > 2)
			OnIntersection(adjacentTiles, adjacentTilesSize);

		// FIXME: THIS CODE IS HERE FOR DEMONSTRATION PURPOSES ONLY - REMOVE IT ONCE PLAYERCONTROLLER/AI IS IMPLEMENTED!
		if (adjacentTilesSize < 4)
		{
			// Choose a new direction that isn't backwards.
			AdjacentTile adjacentTile;
			do
			{
				adjacentTile = adjacentTiles[rand() % adjacentTilesSize];
			} while (AlmostEqual(adjacentTile.pos, oldTile));
			direction = adjacentTile.direction;
		}
	}

	// Called when met with an intersection after finishing moving to the next tile.
	// Params:
	//   adjacentTiles = An array of adjacent empty tiles.
	//   adjacentTilesSize = The number of elements in the array of adjacent empty tiles.
	void GridMovement::OnIntersection(AdjacentTile adjacentTiles[4], size_t adjacentTilesSize)
	{
		UNREFERENCED_PARAMETER(adjacentTiles);
		UNREFERENCED_PARAMETER(adjacentTilesSize);
	}
}

//------------------------------------------------------------------------------
