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

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

namespace Behaviors
{
	//------------------------------------------------------------------------------
	// Public Functions:
	//------------------------------------------------------------------------------

	// Constructor.
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
	Component* PlayerScore::Clone() const
	{
		return new PlayerScore(*this);
	}

	// Increases score by the given amount.
	// Params:
	//   amount = The amount to increase the score by.
	void PlayerScore::IncreaseScore(unsigned increaseAmount)
	{
		score += increaseAmount;
	}

	// Gets the score.
	unsigned int PlayerScore::GetScore() const
	{
		return score;
	}

	// Increases dots by the given amount.
	void PlayerScore::IncreaseDots()
	{
		++dots;
	}

	// Gets the number of dots.
	unsigned PlayerScore::GetDots() const
	{
		return dots;
	}
}
