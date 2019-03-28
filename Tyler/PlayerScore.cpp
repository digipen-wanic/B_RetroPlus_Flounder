//------------------------------------------------------------------------------
//
// File Name:	PlayerScore.cpp
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
#include "PlayerScore.h"

namespace Behaviors
{
	//constructor
	PlayerScore::PlayerScore()
		: Component("PlayerScore"), score(0), dots(0)
	{
	}

	// Initialize this component (happens at object creation).
	void PlayerScore::Initialize()
	{
		score = 0;
	}

	// Clone a component and return a pointer to the cloned component.
	// Returns:
	//   A pointer to a dynamically allocated clone of the component.
	Component * PlayerScore::Clone() const
	{
		return new PlayerScore(*this);
	}

	//Increases Score by given amount
	//Params:
	//unsigned int that increases score
	void PlayerScore::IncreaseScore(unsigned increaseAmount)
	{
		score += increaseAmount;
	}

	//Gets score
	//Return:
	//Unsigned int which is score
	unsigned int PlayerScore::GetScore(void)
	{
		return score;
	}

	//Gets amount of dots
	//Return:
	//unsigned int that is the amount of dots left
	unsigned PlayerScore::GetAmountDots(void)
	{
		return dots;
	}
}