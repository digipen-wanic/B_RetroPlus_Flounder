//------------------------------------------------------------------------------
//
// File Name:	BaseAI.cpp
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

#include "BaseAI.h"

// Systems
#include <GameObject.h>
#include <Space.h>
#include <GameObjectManager.h>
#include <SpriteTilemap.h>
#include "PlayerScore.h"

// Components
#include <Transform.h>

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

namespace Behaviors
{
	//------------------------------------------------------------------------------
	// Public Functions:
	//------------------------------------------------------------------------------

	// Constructor
	// Params:
	//   dotsLeftToLeave = How many dots the player must eat before the ghost moves.
	BaseAI::BaseAI(unsigned dotsLeftToLeave) : hasMoved(false), dotsLeftToLeave(dotsLeftToLeave), forceReverse(false), target(), mode(SCATTER), wave(1)
	{
	}

	// Initialize this component (happens at object creation).
	void BaseAI::Initialize()
	{
		GridMovement::Initialize();
		SetFrozen(true);

		player = GetOwner()->GetSpace()->GetObjectManager().GetObjectByName("PAC-MAN");
	}

	// Updates components using a fixed timestep (usually just physics)
	// Params:
	//	 dt = A fixed change in time, usually 1/60th of a second.
	void BaseAI::FixedUpdate(float dt)
	{
		if (!hasMoved)
		{
			if (player->GetComponent<PlayerScore>()->GetDots() >= dotsLeftToLeave)
			{
				SetFrozen(false);
				hasMoved = true;
			}
		}

		GridMovement::FixedUpdate(dt);
	}

	// Sets the ghost to the frightened state.
	void BaseAI::SetFrightened()
	{
		mode = FRIGHTENED;
	}

	// Returns whether the ghost is in the frightened state.
	bool BaseAI::IsFrightened() const
	{
		return mode == FRIGHTENED;
	}

	//------------------------------------------------------------------------------
	// Protected Functions:
	//------------------------------------------------------------------------------

	// Called when finished moving to the next tile.
	// Params:
	//   adjacentTiles = An array of adjacent tiles.
	//   emptyCount = How many empty tiles were found.
	void BaseAI::OnTileMove(AdjacentTile adjacentTiles[4], size_t emptyCount)
	{
		UNREFERENCED_PARAMETER(emptyCount);

		if (forceReverse)
		{
			// Invert direction.
			direction = static_cast<Direction>((direction + DIRECTION_MAX / 2) % DIRECTION_MAX);
			forceReverse = false;
		}
		else
		{
			OnTarget(adjacentTiles, emptyCount);

			AdjacentTile closest;
			float closestDistance = 999.0f;

			for (size_t i = 0; i < 4; i++)
			{
				if (adjacentTiles[i].empty && !AlmostEqual(adjacentTiles[i].pos, GetOldTile()))
				{
					float distance = adjacentTiles[i].pos.Distance(target);
					if (distance < closestDistance)
					{
						closestDistance = distance;
						closest = adjacentTiles[i];
					}
				}
			}

			direction = closest.direction;
		}
	}

	// Called when met with an intersection after finishing moving to the next tile.
	// Params:
	//   adjacentTiles = An array of adjacent empty tiles.
	//   emptyCount = How many empty tiles were found.
	void BaseAI::OnIntersection(AdjacentTile adjacentTiles[4], size_t emptyCount)
	{
		UNREFERENCED_PARAMETER(adjacentTiles);
		UNREFERENCED_PARAMETER(emptyCount);

		// If we have a reverse forced, we'll let OnTileMove handle this one.
		if (forceReverse)
			return;
	}
}

//------------------------------------------------------------------------------
