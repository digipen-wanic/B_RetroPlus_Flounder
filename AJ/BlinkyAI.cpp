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

// Systems
#include <GameObject.h>

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
	BlinkyAI::BlinkyAI() : BaseAI(0), isElroy(false)
	{
	}

	// Clone a component and return a pointer to the cloned component.
	// Returns:
	//   A pointer to a dynamically allocated clone of the component.
	Component* BlinkyAI::Clone() const
	{
		return new BlinkyAI(*this);
	}

	// Updates components using a fixed timestep (usually just physics)
	// Params:
	//     dt = A fixed change in time, usually 1/60th of a second.
	void BlinkyAI::FixedUpdate(float dt)
	{
		// Call BaseAI's FixedUpdate
		BaseAI::FixedUpdate(dt);

		if (mode == CHASE)
		{
			float pacmanBaseSpeed = player->GetComponent<PlayerController>()->GetSpeed(0);

			// Check if 10 pellets are left
			if (player->GetComponent<PlayerScore>()->GetDots() >= 230)
			{
				// Set isElroy to true
				isElroy = true;
				// Increase speed by another 5%
				GetOwner()->GetComponent<GridMovement>()->SetSpeed((GhostBaseSpeed + 2 * IncreaseSpeedRate) * pacmanBaseSpeed);
			}
			// Check if 20 pellets are left
			else if (player->GetComponent<PlayerScore>()->GetDots() >= 220)
			{
				// Set isElroy to true
				isElroy = true;
				// Increase speed by 5%
				GetOwner()->GetComponent<GridMovement>()->SetSpeed((GhostBaseSpeed + IncreaseSpeedRate) * pacmanBaseSpeed);
			}
		}
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

		// Set Chase Target (Player Position)
		if (mode == CHASE || isElroy)
			target = GetSpriteTilemap()->WorldToTile(player->GetComponent<Transform>()->GetTranslation());
	}
}
