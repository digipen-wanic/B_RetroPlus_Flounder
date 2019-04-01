//------------------------------------------------------------------------------
//
// File Name:	InkyAI.h
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
#include "Vector2D.h"
#include "BaseAI.h"

namespace Behaviors
{
	class InkyAI : public BaseAI
	{
	public:
		//Constructor
		InkyAI();
		
		// Initialize this component (happens at object creation).
		void Initialize() override;
		
	protected:

		// Called when the AI should choose a target.
		// Params:
		//   adjacentTiles = An array of adjacent empty tiles.
		//   emptyCount = How many empty tiles were found.
		virtual void OnTarget(AdjacentTile adjacentTiles[4], size_t emptyCount) override;
	private:
		GameObject* pacman;
		GameObject* blinky;
		Transform* transform;
	};
}