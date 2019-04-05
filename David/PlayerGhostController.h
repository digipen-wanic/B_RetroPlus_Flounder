//------------------------------------------------------------------------------
//
// File Name:	PlayerGhostController.h
// Author(s):	David Cohen (david.cohen)
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

	class PlayerGhostController : public BaseAI
	{
	public:
		//------------------------------------------------------------------------------
		// Public Functions:
		//------------------------------------------------------------------------------

		// Default constructor.
		PlayerGhostController();

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

		// Set the keys for player.
		// Params:
		//   up = Virtual keycode for the up keybind.
		//   left = Virtual keycode for the left keybind.
		//   down = Virtual keycode for the down keybind.
		//   right = Virtual keycode for the right keybind.
		void SetKeyBinds(unsigned up, unsigned left, unsigned down, unsigned right);

	protected:
		//------------------------------------------------------------------------------
		// Protected Functions:
		//------------------------------------------------------------------------------

		// Called when finished moving to the next tile.
		// Params:
		//   adjacentTiles = An array of adjacent tiles.
		//   emptyCount = How many empty tiles were found.
		virtual void OnTileMove(AdjacentTile adjacentTiles[4], size_t emptyCount) override;

		// Called when the AI should choose a target.
		// Params:
		//   adjacentTiles = An array of adjacent empty tiles.
		//   emptyCount = How many empty tiles were found.
		virtual void OnTarget(AdjacentTile adjacentTiles[4], size_t emptyCount) override;

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

		// Other variables
		Direction oldDirection;
	};
}