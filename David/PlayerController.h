//------------------------------------------------------------------------------
//
// File Name:	PlayerController.h
// Author(s):	Tyler Miller (miller.t), A.J. Bussman (anthony.bussman)
// Project:		PAC-MAN
// Course:		WANIC VGP2 2018-2019
//
// Copyright © 2019 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

#include "GridMovement.h"

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

namespace Behaviors
{
	//------------------------------------------------------------------------------
	// Forward Declarations:
	//------------------------------------------------------------------------------

	class PlayerCollision;
	class PlayerAnimation;

	class PlayerController : public GridMovement
	{
	public:
		//------------------------------------------------------------------------------
		// Public Functions:
		//------------------------------------------------------------------------------

		// Default constructor.
		PlayerController();

		// Clone a component and return a pointer to the cloned component.
		// Returns:
		//   A pointer to a dynamically allocated clone of the component.
		Component* Clone() const override;

		// Write object data to file
		// Params:
		//   parser = The parser that is writing this object to a file.
		void Serialize(Parser& parser) const override;

		// Read object data from a file
		// Params:
		//   parser = The parser that is reading this object's data from a file.
		void Deserialize(Parser& parser) override;

		// Updates components using a fixed timestep (usually just physics)
		// Params:
		//     dt = A fixed change in time, usually 1/60th of a second.
		void FixedUpdate(float dt);

		// Set the keys for player.
		// Params:
		//   up = Virtual keycode for the up keybind.
		//   left = Virtual keycode for the left keybind.
		//   down = Virtual keycode for the down keybind.
		//   right = Virtual keycode for the right keybind.
		void SetKeyBinds(unsigned up, unsigned left, unsigned down, unsigned right);

		// Returns the desired speed variable
		// Params:
		//	 speedIndex = which speed variable is desired
		//   0 = normalSpeed, 1 = normDotSpeed, 2 = FrightenedSpeed, 3 = frightDotSpeed
		float GetSpeed(unsigned speedIndex);

		// Called when the player eats a dot.
		void AteDot();

		// Called when the player eats an energizer.
		void AteEnergizer();

	protected:

		//------------------------------------------------------------------------------
		// Protected Functions:
		//------------------------------------------------------------------------------

		// Called when finished moving to the next tile.
		// Params:
		//   adjacentTiles = An array of adjacent tiles.
		//   emptyCount = How many empty tiles were found.
		virtual void OnTileMove(AdjacentTile adjacentTiles[4], size_t emptyCount) override;

		// Called when met with an intersection after finishing moving to the next tile.
		// Params:
		//   adjacentTiles = An array of adjacent empty tiles.
		//   emptyCount = How many empty tiles were found.
		virtual void OnIntersection(AdjacentTile adjacentTiles[4], size_t emptyCount)override;

	private:
		//------------------------------------------------------------------------------
		// Private Variables:
		//------------------------------------------------------------------------------

		// Properties

		// Keybinds
		unsigned upKey;
		unsigned leftKey;
		unsigned downKey;
		unsigned rightKey;

		// Speeds for PAC-MAN in different modes
		float normalSpeed;
		float normDotSpeed;
		float frightenedSpeed;
		float frightDotSpeed;

		bool ateDot;
		float energizerTimer;

		friend class PlayerCollision;
		friend class PlayerAnimation;
	};
}

//------------------------------------------------------------------------------

