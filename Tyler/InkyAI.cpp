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

#include "stdafx.h"
#include "InkyAI.h"
#include "Transform.h"
#include "PlayerController.h"
#include "GridMovement.h"
#include "Space.h"

namespace Behaviors
{

	InkyAI::InkyAI()
		: BaseAI(30), pacman(nullptr), blinky(nullptr)
	{
	}

	// Initialize this component (happens at object creation).
	void InkyAI::Initialize()
	{
		pacman = GetOwner()->GetSpace()->GetObjectManager().GetObjectByName("PAC-MAN");
		blinky = GetOwner()->GetSpace()->GetObjectManager().GetObjectByName("Blinky");
		transform = GetOwner()->GetComponent<Transform>();
	}

	// Called when the AI should choose a target.
	// Params:
	//   adjacentTiles = An array of adjacent empty tiles.
	//   emptyCount = How many empty tiles were found.
	void InkyAI::OnTarget(AdjacentTile adjacentTiles[4], size_t emptyCount)
	{
		UNREFERENCED_PARAMETER(emptyCount);
		Vector2D pacmanDir = pacman->GetComponent<GridMovement>()->GetDirectionVector(2);
		Vector2D blinkyVec = blinky->GetComponent<Transform>()->GetTranslation();
		target = pacmanDir + blinkyVec;
	}
}