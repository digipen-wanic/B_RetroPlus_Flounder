//------------------------------------------------------------------------------
//
// File Name:	BlinkyAI.cpp
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
#include "BlinkyAI.h"

#include "GameObject.h"
#include "Transform.h"

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
	void BlinkyAI::Initialize()
	{
		// Set scatterTarget to Upper-Right Corner
		scatterTarget = Vector2D(25.0f, -2.0f);
	}

	//------------------------------------------------------------------------------
	// Protected Functions:
	//------------------------------------------------------------------------------

	// Called when the AI should choose a target.
	// Params:
	//   adjacentTiles = An array of adjacent empty tiles.
	//   emptyCount = How many empty tiles were found.
	void BlinkyAI::OnTarget(AdjacentTile adjacentTiles[4], size_t emptyCount)
	{
		UNREFERENCED_PARAMETER(adjacentTiles);
		UNREFERENCED_PARAMETER(emptyCount);

		// Set Scatter Target (Upper-Right Corner)
		if (mode == SCATTER)
			target = scatterTarget;
		// Set Chase Target (Player Position)
		else if (mode == CHASE)
			target = player->GetComponent<Transform>()->GetTranslation();
	}
}