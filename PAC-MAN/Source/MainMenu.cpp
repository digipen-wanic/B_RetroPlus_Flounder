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
		GameObjectFactory& objectFactory = GameObjectFactory::GetInstance();
		GameObjectManager& objectManager = GetSpace()->GetObjectManager();
		ResourceManager& resourceManager = GetSpace()->GetResourceManager();

		// Create a new quad mesh for the sprite.
		resourceManager.GetMesh("Quad");
		resourceManager.GetSpriteSource("Code New Roman.png", 12, 8);
		resourceManager.GetMesh("FontAtlas", 12, 8);

		// Load the archetypes from their files.
		objectManager.AddArchetype(*objectFactory.CreateObject("Rectangle", resourceManager.GetMesh("Quad")));
		objectManager.AddArchetype(*objectFactory.CreateObject("ControllableRectangle", resourceManager.GetMesh("FontAtlas"), resourceManager.GetSpriteSource("Code New Roman.png")));
	}

	// Initialize the memory associated with Main Menu.
	void MainMenu::Initialize()
	{
		GameObjectManager& objectManager = GetSpace()->GetObjectManager();

		// Add various physics objects to the scene.

		// Rectangles.
		GameObject* rectangle = new GameObject(*objectManager.GetArchetypeByName("Rectangle"));
		rectangle->GetComponent<Transform>()->SetTranslation(Vector2D(-200.0f, 250.0f));
		rectangle->GetComponent<Transform>()->SetRotation(-M_PI_F / 8.0f);
		rectangle->GetComponent<Physics>()->SetVelocity(Vector2D(50.0f, -75.0f));
		rectangle->GetComponent<Physics>()->SetAngularVelocity(M_PI_F / 2.0f);
		objectManager.AddObject(*rectangle);

		rectangle = new GameObject(*objectManager.GetArchetypeByName("Rectangle"));
		rectangle->GetComponent<Transform>()->SetTranslation(Vector2D(50.0f, -150.0f));
		rectangle->GetComponent<Transform>()->SetRotation(M_PI_F / 8.0f);
		rectangle->GetComponent<Physics>()->SetVelocity(Vector2D(0.0f, 0.0f));
		objectManager.AddObject(*rectangle);

		// Controllable rectangles.
		GameObject* controllableRectangle = new GameObject(*objectManager.GetArchetypeByName("ControllableRectangle"));
		controllableRectangle->GetComponent<SpriteText>()->SetText("ok\nnerd");
		objectManager.AddObject(*controllableRectangle);
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
			GetSpace()->RestartLevel();
		}
		else if (input.CheckTriggered('2'))
		{
			GetSpace()->SetLevel<Level1>();
		}
	}

	// Unload the resources associated with Main Menu.
	void MainMenu::Unload()
	{
	}
}

//----------------------------------------------------------------------------
