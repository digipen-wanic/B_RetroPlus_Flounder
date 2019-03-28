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



namespace Behaviors
{
	class PlayerScore : public Component
	{
	public:
		//------------------------------------------------------------------------------
		// Public Structures:
		//------------------------------------------------------------------------------

		//constructor
		PlayerScore();

		// Initialize this component (happens at object creation).
		void Initialize() override;

		// Clone a component and return a pointer to the cloned component.
		// Returns:
		//   A pointer to a dynamically allocated clone of the component.
		Component* Clone() const override;

		//Increases Score by given amount
		//Params:
		//unsigned int that increases score
		void IncreaseScore(unsigned increaseAmount);

		//Gets score
		//Return:
		//Unsigned int which is score
		unsigned int GetScore(void);

		//Gets amount of dots
		//Return:
		//unsigned int that is the amount of dots left
		unsigned GetAmountDots(void);

	private:
		//------------------------------------------------------------------------------
		// Private variables:
		//------------------------------------------------------------------------------
		unsigned int score;
		unsigned int dots;
	};
}
