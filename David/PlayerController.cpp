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
	PlayerController::PlayerController() : upKey('W'), leftKey('A'), downKey('S'), rightKey('D'),
		normalSpeed(0.0f), normDotSpeed(0.0f), frightenedSpeed(0.0f), frightDotSpeed(0.0f),
		ateDot(false), energizerTimer(0.0f)
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
		parser.WriteVariable("normalSpeed", normalSpeed);
		parser.WriteVariable("normDotSpeed", normDotSpeed);
		parser.WriteVariable("FrightenedSpeed", frightenedSpeed);
		parser.WriteVariable("frightDotSpeed", frightDotSpeed);
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
		parser.ReadVariable("normalSpeed", normalSpeed);
		parser.ReadVariable("normDotSpeed", normDotSpeed);
		parser.ReadVariable("frightenedSpeed", frightenedSpeed);
		parser.ReadVariable("frightDotSpeed", frightDotSpeed);
	}

	// Updates components using a fixed timestep (usually just physics)
	// Params:
	//     dt = A fixed change in time, usually 1/60th of a second.
	void PlayerController::FixedUpdate(float dt)
	{
		GridMovement::FixedUpdate(dt);

		energizerTimer -= dt;
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
	//   0 = normalSpeed, 1 = normDotSpeed, 2 = FrightenedSpeed, 3 = frightDotSpeed
	float PlayerController::GetSpeed(unsigned speedIndex)
	{
		// Return the desired speed
		switch (speedIndex)
		{
		case 0:
			return normalSpeed;
		case 1:
			return normDotSpeed;
		case 2:
			return frightenedSpeed;
		case 3:
			return frightDotSpeed;
		default:
			return 0;
		}
	}

	// Called when the player eats a dot.
	void PlayerController::AteDot()
	{
		ateDot = true;
	}

	// Called when the player eats an energizer.
	void PlayerController::AteEnergizer()
	{
		energizerTimer = 6.0f;
	}

	// Called when finished moving to the next tile.
	// Params:
	//   adjacentTiles = An array of adjacent tiles.
	//   emptyCount = How many empty tiles were found.
	void PlayerController::OnTileMove(AdjacentTile adjacentTiles[4], size_t emptyCount)
	{
		UNREFERENCED_PARAMETER(emptyCount);

		if (ateDot)
		{
			ateDot = false;

			if (energizerTimer >= 0.0f)
			{
				SetSpeed(frightDotSpeed);
			}
			else
			{
				SetSpeed(normDotSpeed);
			}
		}
		else
		{
			if (energizerTimer >= 0.0f)
			{
				SetSpeed(frightenedSpeed);
			}
			else
			{
				SetSpeed(normalSpeed);
			}
		}

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
