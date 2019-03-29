//------------------------------------------------------------------------------
//
// File Name:	GridMovement.h
// Author(s):	David Cohen (david.cohen)
// Project:		PAC-MAN
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
		GridMovement(float speed = 4.0f);

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

		// Sets the speed of the game object.
		void SetSpeed(float speed);

		// Gets the speed of the game object.
		float GetSpeed() const;

		// Sets whether the game object is frozen.
		void SetFrozen(bool frozen);

		// Gets whether the game object is frozen.
		bool IsFrozen() const;

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

			// Whether this tile is off the edge of the screen.
			bool edge;

			// Whether this tile is empty.
			bool empty;

			// The direction to move to get to this tile.
			Direction direction;
		};

		//------------------------------------------------------------------------------
		// Protected Functions:
		//------------------------------------------------------------------------------

		// Gets the old tile.
		Vector2D GetOldTile() const;

		// Gets the tilemap (constant).
		const Tilemap* GetTilemap() const;

		// Gets the sprite tilemap (constant).
		const SpriteTilemap* GetSpriteTilemap() const;

		// Fills out a Vector2D array with all adjacent tile coordinates.
		// Params:
		//   tiles = The array of tiles. Must have a size of 4!
		//   emptyCount = How many empty tiles were found.
		void GetAdjacentTiles(AdjacentTile tiles[4], size_t& emptyCount);

		// Creates an AdjacentTile struct.
		// Params:
		//   pos = The tile's coordinate.
		//   direction = The direction to move to get to this tile.
		// Returns:
		//   The filled out AdjacentTile struct.
		AdjacentTile GetAdjacentTile(Vector2D pos, Direction direction);

		// Helper function to get the cell value at the specified coordinate. If the coordinate was not valid, it returns 0.
		// Params:
		//   pos = The coordinate to get the cell value at.
		//   valid = Whether the coordinate was valid or not.
		// Returns:
		//   The cell value at the specified coordinate. If the coordinate was not valid, returns 0.
		int GetCellValue(Vector2D pos, bool& valid);

		// Called when finished moving to the next tile.
		// Params:
		//   adjacentTiles = An array of adjacent tiles.
		//   emptyCount = How many empty tiles were found.
		virtual void OnTileMove(AdjacentTile adjacentTiles[4], size_t emptyCount) = 0;

		// Called when met with an intersection after finishing moving to the next tile.
		// Params:
		//   adjacentTiles = An array of adjacent empty tiles.
		//   emptyCount = How many empty tiles were found.
		virtual void OnIntersection(AdjacentTile adjacentTiles[4], size_t emptyCount) = 0;

		//------------------------------------------------------------------------------
		// Protected Variables:
		//------------------------------------------------------------------------------

		// Other variables
		Direction direction;

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
		bool frozen;
		Vector2D oldTile;
		Vector2D newTile;
	};
}

//------------------------------------------------------------------------------
