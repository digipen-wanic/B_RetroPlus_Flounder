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
#include "stdafx.h"
#include "PlayerController.h"
#include "Parser.h"
#include "Input.h"

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------
namespace Behaviors
{
	PlayerController::PlayerController()
		: Component("PlayerController"), upKey('W'), leftKey('A'), downKey('S'), rightKey('D')
	{
	}

	void PlayerController::Serialize(Parser & parser) const
	{
		parser.WriteVariable("upKey", upKey);
		parser.WriteVariable("leftKey", leftKey);
		parser.WriteVariable("downKey", downKey);
		parser.WriteVariable("rightKey", rightKey);
	}

	void PlayerController::Deserialize(Parser & parser)
	{
		parser.ReadVariable("upKey", upKey);
		parser.ReadVariable("leftKey", leftKey);
		parser.ReadVariable("downKey", downKey);
		parser.ReadVariable("rightKey", rightKey);
	}

	void PlayerController::SetKeyBinds(unsigned int up, unsigned int left, unsigned int down, unsigned int right)
	{
		upKey = up;
		leftKey = left;
		downKey = down;
		rightKey = right;
	}

	void PlayerController::OnTileMove(AdjacentTile adjacentTiles[4], size_t emptyCount)
	{
		UNREFERENCED_PARAMETER(emptyCount);
		Input& input = Input::GetInstance();
		bool pressed[4] = 
		{
			input.CheckHeld(upKey),
			input.CheckHeld(leftKey),
			input.CheckHeld(downKey),
			input.CheckHeld(rightKey),
		};
		for (size_t i = 0; i < 4; i++)
		{
			if (adjacentTiles[i].empty && pressed[adjacentTiles[i].direction])
			{
				direction = adjacentTiles[i].direction;
				return;
			}
		}
	}

	void PlayerController::OnIntersection(AdjacentTile adjacentTiles[4], size_t emptyCount)
	{
		UNREFERENCED_PARAMETER(adjacentTiles);
		UNREFERENCED_PARAMETER(emptyCount);
	}
}
