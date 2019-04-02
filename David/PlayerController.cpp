//------------------------------------------------------------------------------
//
// File Name:	PlayerController.h
// Author(s):	Tyler Miller (miller.t), A.J. Bussman (anthony.bussman)
// Project:		PAC-MAN
// Course:		WANIC VGP2 2018-2019
//
// Copyright © 2019 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "PlayerController.h"
#include "Parser.h"
#include "Input.h"

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

namespace Behaviors
{
	//------------------------------------------------------------------------------
	// Public Functions:
	//------------------------------------------------------------------------------

	// Default constructor.
	PlayerController::PlayerController() : upKey('W'), leftKey('A'), downKey('S'), rightKey('D')
	{
	}

	// Clone a component and return a pointer to the cloned component.
	// Returns:
	//   A pointer to a dynamically allocated clone of the component.
	Component* PlayerController::Clone() const
	{
		return new PlayerController(*this);
	}

	// Write object data to file
	// Params:
	//   parser = The parser that is writing this object to a file.
	void PlayerController::Serialize(Parser & parser) const
	{
		GridMovement::Serialize(parser);

		parser.WriteVariable("upKey", upKey);
		parser.WriteVariable("leftKey", leftKey);
		parser.WriteVariable("downKey", downKey);
		parser.WriteVariable("rightKey", rightKey);
	}

	// Read object data from a file
	// Params:
	//   parser = The parser that is reading this object's data from a file.
	void PlayerController::Deserialize(Parser & parser)
	{
		GridMovement::Deserialize(parser);

		parser.ReadVariable("upKey", upKey);
		parser.ReadVariable("leftKey", leftKey);
		parser.ReadVariable("downKey", downKey);
		parser.ReadVariable("rightKey", rightKey);
	}

	// Set the keys for player.
	// Params:
	//   up = Virtual keycode for the up keybind.
	//   left = Virtual keycode for the left keybind.
	//   down = Virtual keycode for the down keybind.
	//   right = Virtual keycode for the right keybind.
	void PlayerController::SetKeyBinds(unsigned up, unsigned left, unsigned down, unsigned right)
	{
		upKey = up;
		leftKey = left;
		downKey = down;
		rightKey = right;
	}

	// Returns the desired speed variable
	// Params:
	//	 SpeedIndex = which speed variable is desired
	//   0 = NormalSpeed, 1 = NormDotSpeed, 2 = FrightenedSpeed, 3 = FrightDotSpeed
	float PlayerController::GetSpeed(unsigned speedIndex)
	{
		// Return the desired speed
		switch (speedIndex)
		{
		case 0:
			return NormalSpeed;
		case 1:
			return NormDotSpeed;
		case 2:
			return FrightendSpeed;
		case 3:
			return FrightDotSpeed;
		default:
			return 0;
		}
	}

	// Called when finished moving to the next tile.
	// Params:
	//   adjacentTiles = An array of adjacent tiles.
	//   emptyCount = How many empty tiles were found.
	void PlayerController::OnTileMove(AdjacentTile adjacentTiles[4], size_t emptyCount)
	{
		UNREFERENCED_PARAMETER(emptyCount);

		Input& input = Input::GetInstance();

		// Get input from the player.
		bool pressed[4] =  {
			input.CheckHeld(upKey),
			input.CheckHeld(leftKey),
			input.CheckHeld(downKey),
			input.CheckHeld(rightKey),
		};

		// Check each tile.
		for (size_t i = 0; i < 4; i++)
		{
			// If the tile is empty and the player is pressing to move in its direction, set the player's direction and return.
			if (adjacentTiles[i].empty && pressed[adjacentTiles[i].direction])
			{
				direction = adjacentTiles[i].direction;
				return;
			}
		}
	}

	// Called when met with an intersection after finishing moving to the next tile.
	// Params:
	//   adjacentTiles = An array of adjacent empty tiles.
	//   emptyCount = How many empty tiles were found.
	void PlayerController::OnIntersection(AdjacentTile adjacentTiles[4], size_t emptyCount)
	{
		UNREFERENCED_PARAMETER(adjacentTiles);
		UNREFERENCED_PARAMETER(emptyCount);
	}
}
