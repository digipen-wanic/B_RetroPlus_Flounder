//------------------------------------------------------------------------------
//
// File Name:	MainMenu.cpp
// Author(s):	David Cohen (david.cohen)
// Project:		BetaFramework
// Course:		WANIC VGP2 2018-2019
//
// Copyright © 2018 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "MainMenu.h"

// Systems
#include <Texture.h>
#include "SpriteSource.h"
#include "GameObject.h"
#include "MeshHelper.h"
#include "Space.h"
#include "GameObjectManager.h"
#include <Input.h>
#include <Mesh.h>
#include <GameObjectFactory.h>

// Components
#include "MeshHelper.h"
#include "Transform.h"
#include "Physics.h"
#include <SpriteText.h>

// Levels
#include "Level1.h"
#include "Level2.h"

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

namespace Levels
{
	//------------------------------------------------------------------------------
	// Public Functions:
	//------------------------------------------------------------------------------

	// Creates an instance of Main Menu.
	MainMenu::MainMenu() : Level("MainMenu")
	{
	}

	// Load the resources associated with Main Menu.
	void MainMenu::Load()
	{
		
	}

	// Initialize the memory associated with Main Menu.
	void MainMenu::Initialize()
	{
		
	}

	// Update Main Menu.
	// Params:
	//	 dt = Change in time (in seconds) since the last game loop.
	void MainMenu::Update(float dt)
	{
		UNREFERENCED_PARAMETER(dt);

		Input& input = Input::GetInstance();

		// Handle level switching.
		if (input.CheckTriggered('1'))
		{
			GetSpace()->SetLevel<Level1>();
		}
		else if (input.CheckTriggered('2'))
		{
			GetSpace()->SetLevel<Level2>();
		}
		else if (input.CheckTriggered('0'))
		{
			GetSpace()->RestartLevel();
		}
	}

	// Unload the resources associated with Main Menu.
	void MainMenu::Unload()
	{
	}
}

//----------------------------------------------------------------------------
