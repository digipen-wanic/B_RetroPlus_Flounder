//------------------------------------------------------------------------------
//
// File Name:	GridMovement.h
// Author(s):	David Cohen (david.cohen)
// Project:		BetaFramework
// Course:		WANIC VGP2 2018-2019
//
// Copyright © 2018 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

#include "Component.h" // base class
#include <Vector2D.h>

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Forward Declarations:
//------------------------------------------------------------------------------

class Transform;
class Tilemap;
class SpriteTilemap;

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

namespace Behaviors
{

	class GridMovement : public Component
	{
	public:
		//------------------------------------------------------------------------------
		// Public Functions:
		//------------------------------------------------------------------------------

		// Constructor
		// Params:
		//   speed = How fast the game object moves between tiles.
		GridMovement(float speed = 64.0f);

		// Clone a component and return a pointer to the cloned component.
		// Returns:
		//   A pointer to a dynamically allocated clone of the component.
		Component* Clone() const override;

		// Initialize this component (happens at object creation).
		void Initialize() override;

		// Updates components using a fixed timestep (usually just physics)
		// Params:
		//	 dt = A fixed change in time, usually 1/60th of a second.
		void FixedUpdate(float dt) override;

		// Write object data to file
		// Params:
		//   parser = The parser that is writing this object to a file.
		void Serialize(Parser& parser) const override;

		// Read object data from a file
		// Params:
		//   parser = The parser that is reading this object's data from a file.
		void Deserialize(Parser& parser) override;

		// Sets the tilemap used for the grid.
		// Params:
		//   tilemap = The tilemap.
		//   spriteTilemap = The sprite tilemap.
		void SetTilemap(Tilemap* tilemap, SpriteTilemap* spriteTilemap);

	protected:
		//------------------------------------------------------------------------------
		// Protected Enums:
		//------------------------------------------------------------------------------

		enum Direction
		{
			UP = 0,
			LEFT,
			DOWN,
			RIGHT,

			DIRECTION_MAX
		};

		//------------------------------------------------------------------------------
		// Protected Structs:
		//------------------------------------------------------------------------------

		// Helper struct for keeping track of an adjacent tile - stores the tile's coordinate and the direction to move to get there.
		struct AdjacentTile
		{
			// The tile's coordinate.
			Vector2D pos;

			// The direction to move to get to this tile.
			Direction direction;
		};

		//------------------------------------------------------------------------------
		// Protected Functions:
		//------------------------------------------------------------------------------

		// Fills out a Vector2D array with all adjacent tile coordinates.
		// Params:
		//   tiles = The array of tiles. Must have a size of 4!
		//   tilesSize = How many tiles were written.
		void GetAdjacentTiles(AdjacentTile tiles[4], size_t& tilesSize);

		// Fills out a Vector2D array with all adjacent empty tile coordinates.
		// Params:
		//   tiles = The array of tiles. Must have a size of 4!
		//   tilesSize = How many tiles were written.
		void GetAdjacentEmptyTiles(AdjacentTile tiles[4], size_t& tilesSize);

		// Called when finished moving to the next tile.
		virtual void OnTileMove();

		// Called when met with an intersection after finishing moving to the next tile.
		// Params:
		//   adjacentTiles = An array of adjacent tiles.
		virtual void OnIntersection(AdjacentTile adjacentTiles[4], size_t adjacentTilesSize);

	private:
		//------------------------------------------------------------------------------
		// Private Variables:
		//------------------------------------------------------------------------------

		// Properties (save to/load from file)
		float speed;

		// The tilemap used for the grid.
		Tilemap* tilemap;
		SpriteTilemap* spriteTilemap;

		// Components
		Transform* transform;

		// Other variables
		float tileProgress;
		Direction direction;
		Vector2D oldTile;
		Vector2D newTile;
	};
}

//------------------------------------------------------------------------------
