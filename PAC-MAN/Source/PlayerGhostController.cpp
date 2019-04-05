//------------------------------------------------------------------------------
//
// File Name:	PlayerGhostController.cpp
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

#include "PlayerGhostController.h"

// Systems
#include <GameObject.h>
#include <Parser.h>
#include <Input.h>

// Components
#include <Transform.h>
#include <SpriteTilemap.h>
#include "PlayerScore.h"
#include "PlayerController.h"

#define IncreaseSpeedRate 0.05f // percent of PAC-MAN
#define GhostBaseSpeed 0.75f // percent of PAC-MAN

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
	PlayerGhostController::PlayerGhostController() : BaseAI(0), upKey(VK_UP), leftKey(VK_LEFT), downKey(VK_DOWN), rightKey(VK_RIGHT), oldDirection(Direction::UP)
	{
	}

	// Clone a component and return a pointer to the cloned component.
	// Returns:
	//   A pointer to a dynamically allocated clone of the component.
	Component* PlayerGhostController::Clone() const
	{
		return new PlayerGhostController(*this);
	}

	// Write object data to file
	// Params:
	//   parser = The parser that is writing this object to a file.
	void PlayerGhostController::Serialize(Parser & parser) const
	{
		BaseAI::Serialize(parser);

		parser.WriteVariable("upKey", upKey);
		parser.WriteVariable("leftKey", leftKey);
		parser.WriteVariable("downKey", downKey);
		parser.WriteVariable("rightKey", rightKey);
	}

	// Read object data from a file
	// Params:
	//   parser = The parser that is reading this object's data from a file.
	void PlayerGhostController::Deserialize(Parser & parser)
	{
		BaseAI::Deserialize(parser);

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
	void PlayerGhostController::SetKeyBinds(unsigned up, unsigned left, unsigned down, unsigned right)
	{
		upKey = up;
		leftKey = left;
		downKey = down;
		rightKey = right;
	}

	//------------------------------------------------------------------------------
	// Protected Functions:
	//------------------------------------------------------------------------------

	// Called when finished moving to the next tile.
	// Params:
	//   adjacentTiles = An array of adjacent tiles.
	//   emptyCount = How many empty tiles were found.
	void PlayerGhostController::OnTileMove(AdjacentTile adjacentTiles[4], size_t emptyCount)
	{
		BaseAI::OnTileMove(adjacentTiles, emptyCount);

		Vector2D tile = GetNewTile();

		// Make sure the player is not dead or in the ghost house.
		if (!(tile.x >= 10.0f && tile.x <= 17.0f && tile.y >= 12.0f && tile.y <= 16.0f) && !IsDead())
		{
			Input& input = Input::GetInstance();

				// Get input from the player.
				bool pressed[4] = {
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
					oldDirection = adjacentTiles[i].direction;
					break;
				}
			}

			direction = oldDirection;
		}
		else
		{
			oldDirection = Direction::UP;
		}
	}

	// Called when the AI should choose a target.
	// Params:
	//   adjacentTiles = An array of adjacent empty tiles.
	//   emptyCount = How many empty tiles were found.
	void PlayerGhostController::OnTarget(AdjacentTile adjacentTiles[4], size_t emptyCount)
	{
		UNREFERENCED_PARAMETER(adjacentTiles);
		UNREFERENCED_PARAMETER(emptyCount);
	}
}
