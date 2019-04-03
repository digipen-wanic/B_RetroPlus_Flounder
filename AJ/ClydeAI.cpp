//------------------------------------------------------------------------------
//
// File Name:	ClydeAI.cpp
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

#include "ClydeAI.h"

// Systems
#include <GameObject.h>

// Components
#include <Transform.h>
#include <SpriteTilemap.h>
#include "PlayerScore.h"
#include "spriteTileMap.h"

const float MaxDistanceSqrd = 64.0f;

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
	ClydeAI::ClydeAI() : BaseAI(80)
	{
	}

	// Clone a component and return a pointer to the cloned component.
	// Returns:
	//   A pointer to a dynamically allocated clone of the component.
	Component* ClydeAI::Clone() const
	{
		return new ClydeAI(*this);
	}

	//------------------------------------------------------------------------------
	// Protected Functions:
	//------------------------------------------------------------------------------

	// Called when the AI should choose a target.
	// Params:
	//   adjacentTiles = An array of adjacent empty tiles.
	//   emptyCount = How many empty tiles were found.
	void ClydeAI::OnTarget(AdjacentTile adjacentTiles[4], size_t emptyCount)
	{
		UNREFERENCED_PARAMETER(adjacentTiles);
		UNREFERENCED_PARAMETER(emptyCount);

		if (mode == CHASE)
		{
			// Get Player Position
			Vector2D playerPos = GetSpriteTilemap()->WorldToTile(player->GetComponent<Transform>()->GetTranslation());
			// Get distance squared between Clyde and PAC-MAN
			float distSqToPacman = GetNewTile().DistanceSquared(playerPos);

			// If Clyde is further than 8 tiles from PAC-MAN 
			if (distSqToPacman >= MaxDistanceSqrd)
				target = playerPos;
			else
				target = scatterTarget;
		}
	}
}
