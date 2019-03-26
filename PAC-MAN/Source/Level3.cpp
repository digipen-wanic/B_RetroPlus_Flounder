//------------------------------------------------------------------------------
//
// File Name:	Level3.cpp
// Author(s):	David Wong (david.wongcascante)
// Project:		BetaFramework
// Course:		WANIC VGP2 2018-2019
//
// Copyright � 2018 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "Level3.h"

// Archetypes
#include "Archetypes.h"

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
#include "MouseFollow.h"

// Levels
#include "Level1.h"
#include "Level2.h"
#include "Level4.h"
#include "Level5.h"

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

namespace Levels
{
	//------------------------------------------------------------------------------
	// Public Functions:
	//------------------------------------------------------------------------------

	// Creates an instance of Level 3.
	Level3::Level3() : Level("Level3")
	{
	}

	// Load the resources associated with Level 3.
	void Level3::Load()
	{
		ResourceManager& resourceManager = GetSpace()->GetResourceManager();
		// Create the quad mesh
		resourceManager.GetMesh("Quad");
		// Create the sprite sources
		resourceManager.GetSpriteSource("Hexagon.png");
		resourceManager.GetSpriteSource("Odd_Diamond.png");
	}

	// Initialize the memory associated with Level 3.
	void Level3::Initialize()
	{
		// Get the resource manager
		ResourceManager& resourceManager = GetSpace()->GetResourceManager();

		//GameObject* convex = Archetypes::CreateConvexObject1(resourceManager.GetMesh("Quad"), resourceManager.GetSpriteSource("Odd_Diamond.png"));
		//GameObject* followedConvex = Archetypes::CreateConvexObject2(resourceManager.GetMesh("Quad"), resourceManager.GetSpriteSource("Hexagon.png"));
		// Create the convex object
		GameObject* followedConvex = GameObjectFactory::GetInstance().CreateObject("Convex2", resourceManager.GetMesh("Quad"), 
			resourceManager.GetSpriteSource("Hexagon.png"));

		GameObject* convex = GameObjectFactory::GetInstance().CreateObject("Convex1", resourceManager.GetMesh("Quad"), 
			resourceManager.GetSpriteSource("Odd_Diamond.png"));

		GameObject* rectangle = GameObjectFactory::GetInstance().CreateObject("Rectangle", resourceManager.GetMesh("Quad"));

		GameObject* circle = GameObjectFactory::GetInstance().CreateObject("Circle", resourceManager.GetMesh("Quad"), 
			resourceManager.GetSpriteSource("Circle.png"));

		GameObject* point = GameObjectFactory::GetInstance().CreateObject("Point", resourceManager.GetMesh("Quad"), resourceManager.GetSpriteSource("Circle.png"));

		rectangle->GetComponent<Transform>()->SetTranslation(Vector2D(200, -150));
		circle->GetComponent<Transform>()->SetTranslation(Vector2D(0, 200));
		convex->GetComponent<Transform>()->SetTranslation(Vector2D(-200, -150));
		//followedConvex->GetComponent<Transform>()->SetScale(Vector2D(500, 500));

		followedConvex->AddComponent(new Behaviors::MouseFollow);

		GameObjectManager& manager = GetSpace()->GetObjectManager();
		manager.AddObject(*convex);
		manager.AddObject(*followedConvex);
		manager.AddObject(*rectangle);
		manager.AddObject(*circle);
		manager.AddObject(*point);
	}

	// Update Level 3.
	// Params:
	//	 dt = Change in time (in seconds) since the last game loop.
	void Level3::Update(float dt)
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
		else if (input.CheckTriggered('3'))
		{
			GetSpace()->RestartLevel();
		}
		else if (input.CheckTriggered('4'))
		{
			GetSpace()->SetLevel<Level4>();
		}
		else if (input.CheckTriggered('5'))
		{
			GetSpace()->SetLevel<Level5>();
		}
	}

	// Unload the resources associated with Level 3.
	void Level3::Unload()
	{
	}
}

//----------------------------------------------------------------------------
