//------------------------------------------------------------------------------
//
// File Name:	BlinkyAI.h
// Author(s):	A.J. Bussman (anthony.bussman)
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

#include "BaseAI.h"

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

namespace Behaviors
{

	class BlinkyAI : public BaseAI
	{
	public:
		//------------------------------------------------------------------------------
		// Public Functions:
		//------------------------------------------------------------------------------

		// Default constructor.
		BlinkyAI();

		// Clone a component and return a pointer to the cloned component.
		// Returns:
		//   A pointer to a dynamically allocated clone of the component.
		Component* Clone() const override;

		// Updates components using a fixed timestep (usually just physics)
		// Params:
		//     dt = A fixed change in time, usually 1/60th of a second.
		void FixedUpdate(float dt);

	protected:
		//------------------------------------------------------------------------------
		// Protected Functions:
		//------------------------------------------------------------------------------

		// Called when the AI should choose a target.
		// Params:
		//   adjacentTiles = An array of adjacent empty tiles.
		//   emptyCount = How many empty tiles were found.
		virtual void OnTarget(AdjacentTile adjacentTiles[4], size_t emptyCount) override;

	private:
		//------------------------------------------------------------------------------
		// Private Variables:
		//------------------------------------------------------------------------------

		bool isElroy;
	};
}