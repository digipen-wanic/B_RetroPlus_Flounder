//------------------------------------------------------------------------------
//
// File Name:	Level1.cpp
// Author(s):	David Cohen (david.cohen)
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

#include "Level1.h"

// Systems
#include <Texture.h>
#include <SpriteSource.h>
#include <GameObject.h>
#include <MeshHelper.h>
#include <Space.h>
#include <GameObjectManager.h>
#include <Input.h>
#include <Mesh.h>
#include <GameObjectFactory.h>
#include <Tilemap.h>

// Components
#include <MeshHelper.h>
#include <Transform.h>
#include <Physics.h>
#include <SpriteTilemap.h>
#include "GridMovement.h"

// Levels
#include "MainMenu.h"

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

namespace Levels
{
	//------------------------------------------------------------------------------
	// Public Functions:
	//------------------------------------------------------------------------------

	// Creates an instance of Level 1.
	Level1::Level1() : Level("Level1"), columnsMap(4), rowsMap(3)
	{
	}

	// Load the resources associated with Level 1.
	void Level1::Load()
	{
		GameObjectFactory& objectFactory = GameObjectFactory::GetInstance();
		GameObjectManager& objectManager = GetSpace()->GetObjectManager();
		ResourceManager& resourceManager = GetSpace()->GetResourceManager();

		// Create a new quad mesh for the sprite.
		resourceManager.GetMesh("Quad");

		// Load the circle texture and sprite source.
		resourceManager.GetSpriteSource("Circle.png");

		// Load the archetypes from their files.
		objectManager.AddArchetype(*objectFactory.CreateObject("Circle", resourceManager.GetMesh("Quad"), resourceManager.GetSpriteSource("Circle.png")));
		objectManager.AddArchetype(*objectFactory.CreateObject("Point", resourceManager.GetMesh("Quad"), resourceManager.GetSpriteSource("Circle.png")));
		objectManager.AddArchetype(*objectFactory.CreateObject("ControllableRectangle", resourceManager.GetMesh("Quad")));
		objectManager.AddArchetype(*objectFactory.CreateObject("PAC-MAN", resourceManager.GetMesh("Quad"), resourceManager.GetSpriteSource("Circle.png")));
		objectManager.AddArchetype(*objectFactory.CreateObject("Ghost", resourceManager.GetMesh("Quad")));

		// Load the tilemap.
		dataMap = Tilemap::CreateTilemapFromFile("Assets/Levels/Level2.txt");
		if (dataMap == nullptr)
		{
			std::cout << "Error loading map!" << std::endl;
		}
		else
		{
			// Create a new quad mesh for the sprite tilemap.
			resourceManager.GetMesh("Map", columnsMap, rowsMap);

			// Load the tilemap texture and sprite source.
			resourceManager.GetSpriteSource("Tilemap.png", columnsMap, rowsMap);

			// Create the tilemap and add it to the object manager.
			GameObject* tilemap = objectFactory.CreateObject("Tilemap", resourceManager.GetMesh("Map"), resourceManager.GetSpriteSource("Tilemap.png"));
			tilemap->GetComponent<SpriteTilemap>()->SetTilemap(dataMap);
			objectManager.AddArchetype(*tilemap);
		}
	}

	// Initialize the memory associated with Level 1.
	void Level1::Initialize()
	{
		GameObjectManager& objectManager = GetSpace()->GetObjectManager();

		// Add various physics objects to the scene.

		// Tilemap.
		GameObject* tilemap = new GameObject(*objectManager.GetArchetypeByName("Tilemap"));
		objectManager.AddObject(*tilemap);

		// PAC-MAN.
		GameObject* pacMan = new GameObject(*objectManager.GetArchetypeByName("PAC-MAN"));
		pacMan->GetComponent<Behaviors::GridMovement>()->SetTilemap(dataMap, tilemap->GetComponent<SpriteTilemap>());
		pacMan->GetComponent<Transform>()->SetTranslation(Vector2D(100.0f, 100.0f));
		objectManager.AddObject(*pacMan);

		// Ghost.
		GameObject* ghost = new GameObject(*objectManager.GetArchetypeByName("Ghost"));
		//ghost->GetComponent<Behaviors::GridMovement>()->SetTilemap(dataMap, tilemap->GetComponent<SpriteTilemap>());
		ghost->GetComponent<Transform>()->SetTranslation(Vector2D(400.0f, 100.0f));
		objectManager.AddObject(*ghost);

		//Energizer Pellet
		GameObject* energizerPellet = new GameObject(*objectManager.GetArchetypeByName("EnergizerPellet"));
		energizerPellet->GetComponent<Transform>()->SetTranslation(Vector2D(500, 100));
		objectManager.AddObject(*energizerPellet);

		GameObject* energizerPellet2 = new GameObject(*objectManager.GetArchetypeByName("EnergizerPellet"));
		energizerPellet->GetComponent<Transform>()->SetTranslation(Vector2D(500, 100));
		objectManager.AddObject(*energizerPellet2);

		GameObject* energizerPellet3 = new GameObject(*objectManager.GetArchetypeByName("EnergizerPellet"));
		energizerPellet->GetComponent<Transform>()->SetTranslation(Vector2D(500, 100));
		objectManager.AddObject(*energizerPellet3);

		GameObject* energizerPellet4 = new GameObject(*objectManager.GetArchetypeByName("EnergizerPellet"));
		energizerPellet->GetComponent<Transform>()->SetTranslation(Vector2D(500, 100));
		objectManager.AddObject(*energizerPellet4);

		Vector2D theTile = Vector2D(0, 0);
		for (int y = 0; y < 36; y++)
		{
			for (int x = 0; x < 28; x++)
			{
				Vector2D epPos1 = energizerPellet->GetComponent<Transform>()->GetTranslation();
				Vector2D epPos2 = energizerPellet2->GetComponent<Transform>()->GetTranslation();
				Vector2D epPos3 = energizerPellet3->GetComponent<Transform>()->GetTranslation();
				Vector2D epPos4 = energizerPellet4->GetComponent<Transform>()->GetTranslation();
				theTile = Vector2D(static_cast<float>(x), static_cast<float>(y));
				if (tilemap->GetComponent<Tilemap>()->GetCellValue(x,y) == 0 && !AlmostEqual(epPos1, theTile)
					&& !AlmostEqual(epPos2, theTile) && !AlmostEqual(epPos3, theTile) && !AlmostEqual(epPos4, theTile))
				{
					GameObject* pellet = new GameObject(*objectManager.GetArchetypeByName("Pellet"));
					pellet->GetComponent<Transform>()->SetTranslation(Vector2D(static_cast<float>(x), static_cast<float>(y)));
					objectManager.AddObject(*pellet);
				}
			}
		}
	}
	// Update Level 1.
	// Params:
	//	 dt = Change in time (in seconds) since the last game loop.
	void Level1::Update(float dt)
	{
		UNREFERENCED_PARAMETER(dt);

		Input& input = Input::GetInstance();

		// Handle level switching.
		if (input.CheckTriggered('1'))
		{
			GetSpace()->SetLevel<MainMenu>();
		}
		else if (input.CheckTriggered('2'))
		{
			GetSpace()->RestartLevel();
		}
	}

	// Unload the resources associated with Level 1.
	void Level1::Unload()
	{
		// Free all allocated memory.
		delete dataMap;
	}
}

//----------------------------------------------------------------------------