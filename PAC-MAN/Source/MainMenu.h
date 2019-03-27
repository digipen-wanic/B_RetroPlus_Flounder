//------------------------------------------------------------------------------
//
// File Name:	MainMenu.h
// Author(s):	Davids Cohen (david.cohen)
// Project:		BetaFramework
// Course:		WANIC VGP2 2018-2019
//
// Copyright © 2018 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

#include <Level.h>

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Forward References:
//------------------------------------------------------------------------------

class Texture;
class SpriteSource;
class Mesh;
class SpriteSource;
class GameObject;

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

namespace Levels
{
	class MainMenu : public Level
	{
	public:
		//------------------------------------------------------------------------------
		// Public Functions:
		//------------------------------------------------------------------------------

		// Creates an instance of Main Menu.
		MainMenu();

		// Load the resources associated with Main Menu.
		void Load() override;

		// Initialize the memory associated with Main Menu.
		void Initialize() override;

		// Update Main Menu.
		// Params:
		//	 dt = Change in time (in seconds) since the last game loop.
		void Update(float dt) override;

		// Unload the resources associated with Main Menu.
		void Unload() override;
	};
}

//----------------------------------------------------------------------------
