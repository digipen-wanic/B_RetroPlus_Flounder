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

		// Sets the score.
		void SetScore(unsigned score);

		// Gets the score.
		unsigned GetScore() const;

		// Increases dots by 1.
		void IncreaseDots();

		// Sets the number of dots.
		void SetDots(unsigned dots);

		// Gets the number of dots.
		unsigned GetDots() const;

	private:
		//------------------------------------------------------------------------------
		// Private variables:
		//------------------------------------------------------------------------------

		// Other variables
		unsigned score;
		unsigned dots;
	};
}
