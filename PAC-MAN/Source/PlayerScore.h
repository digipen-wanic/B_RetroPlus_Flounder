//------------------------------------------------------------------------------
//
// File Name:	PlayerScore.h
// Author(s):	Tyler Miller (miller.t)
// Project:		PAC-MAN
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

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

namespace Behaviors
{
	class PlayerScore : public Component
	{
	public:
		//------------------------------------------------------------------------------
		// Public Functions:
		//------------------------------------------------------------------------------

		// Constructor.
		PlayerScore();

		// Initialize this component (happens at object creation).
		void Initialize() override;

		// Clone a component and return a pointer to the cloned component.
		// Returns:
		//   A pointer to a dynamically allocated clone of the component.
		Component* Clone() const override;

		// Increases score by the given amount.
		// Params:
		//   amount = The amount to increase the score by.
		void IncreaseScore(unsigned amount);

		// Gets the score.
		unsigned int GetScore() const;

		// Increases dots by the given amount.
		// Params:
		//   amount = The amount to increase the dots by.
		void IncreaseDots(unsigned amount);

		// Gets the number of dots.
		unsigned GetDots() const;

	private:
		//------------------------------------------------------------------------------
		// Private variables:
		//------------------------------------------------------------------------------

		// Other variables
		unsigned int score;
		unsigned int dots;
	};
}
