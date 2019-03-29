//------------------------------------------------------------------------------
//
// File Name:	Energizer.cpp
// Author(s):	A.J. Bussman (anthony.bussman)
// Project:		BetaFramework
// Course:		WANIC VGP2 2018-2019
//
// Copyright © 2018 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "Energizer.h"
// Systems
#include "GameObject.h"
#include "Space.h"
#include "GameObjectManager.h"

#include "BaseAI.h"
#include "PlayerController.h"

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------
namespace Behaviors
{
	//------------------------------------------------------------------------------
	// Public Functions:
	//------------------------------------------------------------------------------

	// Constructor
	Energizer::Energizer() : Component("Energizer")
	{
	}

	// Destructor
	Energizer::~Energizer()
	{
	}

	// Sets the ghosts to frightened and sets Pac-Man's speed
	void Energizer::IsEaten()
	{
		// Get each ghost
		std::vector<GameObject*> ghosts;
		ghosts.reserve(4);
		this->GetOwner()->GetSpace()->GetObjectManager().GetAllObjectsByName("Ghost", ghosts);
		// Set every ghost's mode to frightened
		for (GameObject* ghost : ghosts)
			ghost->GetComponent<BaseAI>()->SetFrightened();

		// Get PAC-MAN
		GameObject* pacman = this->GetOwner()->GetSpace()->GetObjectManager().GetObjectByName("PAC-MAN");
		// Set Pac-Man's speed
		pacman->GetComponent<PlayerController>()->SetSpeed(pacman->GetComponent<PlayerController>()->GetSpeed(2));
	}
}