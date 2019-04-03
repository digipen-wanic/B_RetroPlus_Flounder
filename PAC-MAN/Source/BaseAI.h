//------------------------------------------------------------------------------
//
// File Name:	BaseAI.h
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

#include "GridMovement.h"

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

namespace Behaviors
{
	class GhostAnimation;

	class BaseAI : public GridMovement
	{
	public:
		//------------------------------------------------------------------------------
		// Public Functions:
		//------------------------------------------------------------------------------

		// Constructor
		// Params:
		//   dotsLeftToLeave = How many dots the player must eat before the ghost moves.
		BaseAI(unsigned dotsLeftToLeave);

		// Initialize this component (happens at object creation).
		void Initialize() override;

		// Updates components using a fixed timestep (usually just physics)
		// Params:
		//	 dt = A fixed change in time, usually 1/60th of a second.
		virtual void FixedUpdate(float dt) override;

		// Write object data to file
		// Params:
		//   parser = The parser that is writing this object to a file.
		void Serialize(Parser& parser) const override;

		// Read object data from a file
		// Params:
		//   parser = The parser that is reading this object's data from a file.
		void Deserialize(Parser& parser) override;

		// Draw a sprite (Sprite can be textured or untextured).
		void Draw() override;

		// Sets the ghost to the frightened state.
		void SetFrightened();

		// Returns whether the ghost is in the frightened state.
		bool IsFrightened() const;

		// Marks the enemy as dead.
		void SetDead();

		// Returns whether the ghost is dead.
		bool IsDead() const;

		// Adds an overridden direction for a specific tile.
		// Params:
		//   tile = The coordinates of the tile to add an override for.
		//   overriddenDirection = The direction the AI should be forced to move in when encountering the specified tile.
		void AddOverrideTile(Vector2D tile, Direction overriddenDirection);

		// Adds an overridden direction exclusion for a specific tile.
		// Params:
		//   tile = The coordinates of the tile to add an override for.
		//   excludedDirection = The direction the AI cannot move in when encountering the specified tile.
		void AddOverrideExclusionTile(Vector2D tile, Direction excludedDirection);

	protected:
		//------------------------------------------------------------------------------
		// Protected Enums:
		//------------------------------------------------------------------------------

		enum Mode
		{
			SCATTER = 0,
			CHASE,
			FRIGHTENED,

			MODE_MAX
		};

		//------------------------------------------------------------------------------
		// Protected Functions:
		//------------------------------------------------------------------------------

		// Called when finished moving to the next tile.
		// Params:
		//   adjacentTiles = An array of adjacent tiles.
		//   emptyCount = How many empty tiles were found.
		virtual void OnTileMove(AdjacentTile adjacentTiles[4], size_t emptyCount) override;

		// Called when met with an intersection after finishing moving to the next tile.
		// Params:
		//   adjacentTiles = An array of adjacent empty tiles.
		//   emptyCount = How many empty tiles were found.
		virtual void OnIntersection(AdjacentTile adjacentTiles[4], size_t emptyCount) override;

		// Called when the AI should choose a target.
		// Params:
		//   adjacentTiles = An array of adjacent empty tiles.
		//   emptyCount = How many empty tiles were found.
		virtual void OnTarget(AdjacentTile adjacentTiles[4], size_t emptyCount) = 0;

		//------------------------------------------------------------------------------
		// Protected Variables:
		//------------------------------------------------------------------------------

		// The player game object.
		GameObject* player;

		// Other variables
		Vector2D target;
		Vector2D scatterTarget;
		Mode mode;

	private:
		//------------------------------------------------------------------------------
		// Private Structures:
		//------------------------------------------------------------------------------

		struct OverriddenTile
		{
			Vector2D pos;
			Direction direction;
		};

		//------------------------------------------------------------------------------
		// Private Functions:
		//------------------------------------------------------------------------------

		// Handles pathfinding and setting direction.
		// Params:
		//   adjacentTiles = An array of adjacent tiles.
		//   emptyCount = How many empty tiles were found.
		void Pathfind(AdjacentTile adjacentTiles[4], size_t emptyCount);

		//------------------------------------------------------------------------------
		// Private Variables:
		//------------------------------------------------------------------------------

		// Components
		GhostAnimation* ghostAnimation;

		// Other variables
		bool hasMoved;
		unsigned dotsLeftToLeave;
		bool forceReverse;
		bool isDead;
		unsigned wave;
		std::vector<OverriddenTile> overriddenTiles;
		std::vector<OverriddenTile> overriddenExclusionTiles;

		// Insertion operator for OverriddenTile.
		// Params:
		//   os = The output stream.
		//   overriddenTile = The overridden tile.
		// Returns:
		//   A reference to the output stream.
		friend std::ostream& operator<<(std::ostream& os, const OverriddenTile& overriddenTile);

		// Extraction operator for OverriddenTile.
		// Params:
		//   is = The input stream.
		//   overriddenTile = The overridden tile.
		// Returns:
		//   A reference to the input stream.
		friend std::istream& operator>>(std::istream& is, OverriddenTile& overriddenTile);

		friend class GhostAnimation;
	};
}

//------------------------------------------------------------------------------
