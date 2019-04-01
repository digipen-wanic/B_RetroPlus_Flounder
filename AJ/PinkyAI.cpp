//------------------------------------------------------------------------------
//
// File Name:	PinkyAI.cpp
// Author(s):	A.J. Bussman (anthony.bussman)
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
#include "PinkyAI.h"

#include "GameObject.h"
#include "Transform.h"
#include "PlayerController.h"

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

namespace Behaviors
{
	//------------------------------------------------------------------------------
	// Public Functions:
	//------------------------------------------------------------------------------

	// Initialize this component (happens at object creation).
	void PinkyAI::Initialize()
	{
		// Set scatterTarget to Upper-Left Corner
		scatterTarget = Vector2D(2.0f, -2.0f);
	}

	//------------------------------------------------------------------------------
	// Protected Functions:
	//------------------------------------------------------------------------------

	// Called when the AI should choose a target.
	// Params:
	//   adjacentTiles = An array of adjacent empty tiles.
	//   emptyCount = How many empty tiles were found.
	void PinkyAI::OnTarget(AdjacentTile adjacentTiles[4], size_t emptyCount)
	{
		UNREFERENCED_PARAMETER(adjacentTiles);
		UNREFERENCED_PARAMETER(emptyCount);

		// Set Scatter Target (Upper-Left Corner)
		if (mode == SCATTER)
			target = scatterTarget;
		// Set Chase Target (4 infront of Player Position)
		else if (mode == CHASE)
		{
			// Set target to player
			target = player->GetComponent<Transform>()->GetTranslation();
			// Add 4 * Direction Vector of PAC-MAN
			target += player->GetComponent<PlayerController>()->GetDirectionVector(4);
		}
	}
}