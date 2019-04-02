//------------------------------------------------------------------------------
//
// File Name:	InkyAI.cpp
// Author(s):	Tyler Miller (miller.t)
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

#include "InkyAI.h"

// Systems
#include <Space.h>

// Components
#include <Transform.h>
#include <SpriteTilemap.h>
#include "PlayerController.h"
#include "GridMovement.h"

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
	InkyAI::InkyAI() : BaseAI(30), blinky(nullptr)
	{
	}

	// Clone a component and return a pointer to the cloned component.
	// Returns:
	//   A pointer to a dynamically allocated clone of the component.
	Component* InkyAI::Clone() const
	{
		return new InkyAI(*this);
	}

	// Initialize this component (happens at object creation).
	void InkyAI::Initialize()
	{
		BaseAI::Initialize();

		blinky = GetOwner()->GetSpace()->GetObjectManager().GetObjectByName("Blinky");
	}

	//------------------------------------------------------------------------------
	// Protected Functions:
	//------------------------------------------------------------------------------

	// Called when the AI should choose a target.
	// Params:
	//   adjacentTiles = An array of adjacent empty tiles.
	//   emptyCount = How many empty tiles were found.
	void InkyAI::OnTarget(AdjacentTile adjacentTiles[4], size_t emptyCount)
	{
		UNREFERENCED_PARAMETER(adjacentTiles);
		UNREFERENCED_PARAMETER(emptyCount);

		Vector2D pacmanDir = player->GetComponent<GridMovement>()->GetDirectionVector(2);
		Vector2D blinkyVec = blinky->GetComponent<Transform>()->GetTranslation();
		target = pacmanDir + blinkyVec;
	}
}
