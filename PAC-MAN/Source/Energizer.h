//------------------------------------------------------------------------------
//
// File Name:	Energizer.h
// Author(s):	A.J. Bussman (anthony.bussman)
// Project:		BetaFramework
// Course:		WANIC VGP2 2018-2019
//
// Copyright © 2018 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------
#pragma once

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------
#include "Component.h"

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------
namespace Behaviors
{
	class Energizer : public Component
	{
	public:
		//------------------------------------------------------------------------------
		// Public Functions:
		//------------------------------------------------------------------------------

		// Constructor
		Energizer();

		// Destructor
		~Energizer();

		// Sets the ghosts to frightened and sets Pac-Man's speed
		void IsEaten();
	};
}