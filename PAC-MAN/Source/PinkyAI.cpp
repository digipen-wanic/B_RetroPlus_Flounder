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

// Systems
#include <GameObject.h>
#include <Transform.h>

// Components
#include <SpriteTilemap.h>
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

	// Default constructor
	PinkyAI::PinkyAI() : BaseAI(0)
	{
	}

	// Clone a component and return a pointer to the cloned component.
	// Returns:
	//   A pointer to a dynamically allocated clone of the component.
	Component* PinkyAI::Clone() const
	{
		return new PinkyAI(*this);
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

		// Set Chase Target (4 infront of Player Position)
		if (mode == CHASE)
		{
			// Set target to player
			target = GetSpriteTilemap()->WorldToTile(player->GetComponent<Transform>()->GetTranslation());

			// Add 4 * Direction Vector of PAC-MAN
			target += player->GetComponent<PlayerController>()->GetDirectionVector(4);
		}
	}
}