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

	class BaseAI : public GridMovement
	{
	public:
		//------------------------------------------------------------------------------
		// Public Functions:
		//------------------------------------------------------------------------------

		// Constructor
		BaseAI();

		// Sets the ghost to the frightened state.
		void SetFrightened();

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

	private:
		//------------------------------------------------------------------------------
		// Private Variables:
		//------------------------------------------------------------------------------

		// Other variables
		bool forceReverse;
		Vector2D target;
		Mode mode;
		unsigned wave;
	};
}

//------------------------------------------------------------------------------
