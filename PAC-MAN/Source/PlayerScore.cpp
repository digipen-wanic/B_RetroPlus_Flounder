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

	// Sets the score.
	void PlayerScore::SetScore(unsigned score_)
	{
		score = score_;
	}

	// Gets the score.
	unsigned PlayerScore::GetScore() const
	{
		return score;
	}

	// Increases dots by 1.
	void PlayerScore::IncreaseDots()
	{
		++dots;
	}

	// Sets the number of dots.
	void PlayerScore::SetDots(unsigned dots_)
	{
		dots = dots_;
	}

	// Gets the number of dots.
	unsigned PlayerScore::GetDots() const
	{
		return dots;
	}
}
