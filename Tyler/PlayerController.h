//------------------------------------------------------------------------------
//
// File Name:	PlayerController.h
// Author(s):	Tyler Miller
// Project:		PAC-MAN
// Course:		WANIC VGP2 2018-2019
//
// Copyright © 2019 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Include Files:
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

	class PlayerController : public GridMovement
	{
	public:
		//------------------------------------------------------------------------------
		// Public Functions:
		//------------------------------------------------------------------------------

		// Constructor
		PlayerController();

		// Write object data to file
		// Params:
		//   parser = The parser that is writing this object to a file.
		void Serialize(Parser& parser) const override;

		// Read object data from a file
		// Params:
		//   parser = The parser that is reading this object's data from a file.
		void Deserialize(Parser& parser) override;

		//Set the keys for player
		//Params:
		//up = unsigned int for key, left = unsigned int for key,
		//down= unsigned int for key, right = unsigned int for key
		void SetKeyBinds(unsigned int up, unsigned int left, unsigned int down, unsigned int right);

	protected:

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
		virtual void OnIntersection(AdjacentTile adjacentTiles[4], size_t emptyCount)override;

	private:
		//------------------------------------------------------------------------------
		// Private Variables:
		//------------------------------------------------------------------------------

		// Properties what types of Keys
		unsigned int upKey;
		unsigned int leftKey;
		unsigned int downKey;
		unsigned int rightKey;
	};
}

//------------------------------------------------------------------------------

