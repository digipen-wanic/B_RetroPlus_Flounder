//------------------------------------------------------------------------------
//
// File Name:	Level2.cpp
// Author(s):	A.J. Bussman (anthony.bussman)
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

#include "Level2.h"

// Systems
#include <Engine.h>
#include <SoundManager.h>
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
#include <Animation.h>
#include <Transform.h>
#include <Physics.h>
#include <SpriteTilemap.h>
#include "GridMovement.h"
#include "PlayerScore.h"
#include "PlayerCollision.h"

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

	// Creates an instance of Level 2.
	Level2::Level2() : Level("Level2"),
		columnsMap(8), rowsMap(5), columnsEnergizer(2), rowsEnergizer(1), columnsPacMan(4), rowsPacMan(4), columnsGhost(3), rowsGhost(4),
		startLives(3), lives(0), oldScore(0), oldDots(0),
		soundManager(nullptr), energizerPositions(), dotPositions()
	{
	}

	// Load the resources associated with Level 2.
	void Level2::Load()
	{
		// Load sound effects.
		soundManager = Engine::GetInstance().GetModule<SoundManager>();
		soundManager->AddEffect("deathofpacfinal.wav");

		GameObjectFactory& objectFactory = GameObjectFactory::GetInstance();
		GameObjectManager& objectManager = GetSpace()->GetObjectManager();
		ResourceManager& resourceManager = GetSpace()->GetResourceManager();

		// Create a new quad mesh for the sprite.
		resourceManager.GetMesh("Quad");

		// Load the circle texture and sprite source.
		resourceManager.GetSpriteSource("Circle.png");

		// Load the archetypes from their files.
		objectManager.AddArchetype(*objectFactory.CreateObject("Dot", resourceManager.GetMesh("Quad"), resourceManager.GetSpriteSource("Circle.png")));
		objectManager.AddArchetype(*objectFactory.CreateObject("Energizer", resourceManager.GetMesh("Quad"), resourceManager.GetSpriteSource("Circle.png")));
		objectManager.AddArchetype(*objectFactory.CreateObject("PAC-MAN", resourceManager.GetMesh("Quad"), resourceManager.GetSpriteSource("Circle.png")));
		objectManager.AddArchetype(*objectFactory.CreateObject("KingGhost", resourceManager.GetMesh("Quad"), resourceManager.GetSpriteSource("Circle.png")));
		objectManager.AddArchetype(*objectFactory.CreateObject("Blinky", resourceManager.GetMesh("Quad")));
		objectManager.AddArchetype(*objectFactory.CreateObject("Pinky", resourceManager.GetMesh("Quad")));
		objectManager.AddArchetype(*objectFactory.CreateObject("Inky", resourceManager.GetMesh("Quad")));
		objectManager.AddArchetype(*objectFactory.CreateObject("Clyde", resourceManager.GetMesh("Quad")));

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
			resourceManager.GetSpriteSource("ArtTileSet.png", columnsMap, rowsMap);

			// Create the tilemap and add it to the object manager.
			GameObject* tilemap = objectFactory.CreateObject("Tilemap", resourceManager.GetMesh("Map"), resourceManager.GetSpriteSource("ArtTileSet.png"));
			tilemap->GetComponent<SpriteTilemap>()->SetTilemap(dataMap);
			objectManager.AddArchetype(*tilemap);
		}

		lives = startLives;
	}

	// Initialize the memory associated with Level 2.
	void Level2::Initialize()
	{
		GameObjectManager& objectManager = GetSpace()->GetObjectManager();

		// Tilemap.
		GameObject* tilemap = new GameObject(*objectManager.GetArchetypeByName("Tilemap"));
		objectManager.AddObject(*tilemap);

		SpriteTilemap* spriteTilemap = tilemap->GetComponent<SpriteTilemap>();

		// PAC-MAN.
		GameObject* pacMan = new GameObject(*objectManager.GetArchetypeByName("PAC-MAN"));
		pacMan->GetComponent<Behaviors::GridMovement>()->SetTilemap(dataMap, spriteTilemap);
		pacMan->GetComponent<Behaviors::PlayerCollision>()->SetTilemap(dataMap, spriteTilemap);
		pacMan->GetComponent<Transform>()->SetTranslation(spriteTilemap->TileToWorld(Vector2D(13.5f, 23.0f)));

		// If there are no energizers or dots, place them.
		if (lives == 0)
		{
			lives = startLives;
			PopulateDots();
		}
		else
		{
			Behaviors::PlayerScore* playerScore = pacMan->GetComponent<Behaviors::PlayerScore>();
			playerScore->SetScore(oldScore);
			playerScore->SetDots(oldDots);
		}

		// Add all energizers & dots.

		for (auto it = energizerPositions.begin(); it != energizerPositions.end(); ++it)
		{
			// Create energizer at position.
			GameObject* energizerPellet = new GameObject(*objectManager.GetArchetypeByName("Energizer"));
			energizerPellet->GetComponent<Transform>()->SetTranslation(*it);
			objectManager.AddObject(*energizerPellet);
			energizerPellet->GetComponent<Animation>()->Play(0, 2, 0.125f, true);
		}

		for (auto it = dotPositions.begin(); it != dotPositions.end(); ++it)
		{
			// Create dot at position.
			GameObject* Pellet = new GameObject(*objectManager.GetArchetypeByName("Dot"));
			Pellet->GetComponent<Transform>()->SetTranslation(*it);
			objectManager.AddObject(*Pellet);
		}

		// KingGhost
		GameObject* kingGhost = new GameObject(*objectManager.GetArchetypeByName("KingGhost"));
		kingGhost->GetComponent<Behaviors::GridMovement>()->SetTilemap(dataMap, spriteTilemap);
		kingGhost->GetComponent<Transform>()->SetTranslation(spriteTilemap->TileToWorld(Vector2D(13.5f, 11.0f)));
		objectManager.AddObject(*kingGhost);

		// Add PAC-MAN here so it draws over everything else.
		objectManager.AddObject(*pacMan);

		--lives;
	}

	// Update Level 2.
	// Params:
	//	 dt = Change in time (in seconds) since the last game loop.
	void Level2::Update(float dt)
	{
		UNREFERENCED_PARAMETER(dt);

		Input& input = Input::GetInstance();

		// Handle level switching.
		if (input.CheckTriggered('0'))
		{
			GetSpace()->SetLevel<MainMenu>();
		}
		else if (input.CheckTriggered('2'))
		{
			GetSpace()->RestartLevel();
		}
	}

	// Destroy objects associated with level 2.
	void Level2::Shutdown()
	{
		GameObjectManager& objectManager = GetSpace()->GetObjectManager();

		// Gather all energizers.
		std::vector<GameObject*> energizers;
		objectManager.GetAllObjectsByName("Energizer", energizers);

		// Store their positions.
		energizerPositions.clear();
		for (auto it = energizers.begin(); it != energizers.end(); ++it)
		{
			energizerPositions.push_back((*it)->GetComponent<Transform>()->GetTranslation());
		}

		// Gather all dots.
		std::vector<GameObject*> dots;
		objectManager.GetAllObjectsByName("Dot", dots);

		// Store their positions.
		dotPositions.clear();
		for (auto it = dots.begin(); it != dots.end(); ++it)
		{
			dotPositions.push_back((*it)->GetComponent<Transform>()->GetTranslation());
		}

		Behaviors::PlayerScore* playerScore = objectManager.GetObjectByName("PAC-MAN")->GetComponent<Behaviors::PlayerScore>();
		oldScore = playerScore->GetScore();
		oldDots = playerScore->GetDots();
	}

	// Unload the resources associated with Level 2.
	void Level2::Unload()
	{
		// Free all allocated memory.
		delete dataMap;

		// Unload all sounds.
		soundManager->Shutdown();
	}

	//------------------------------------------------------------------------------
	// Private Functions:
	//------------------------------------------------------------------------------

	// Resets the energizer and dot position lists.
	void Level2::PopulateDots()
	{
		energizerPositions.clear();
		dotPositions.clear();

		SpriteTilemap* spriteTilemap = GetSpace()->GetObjectManager().GetObjectByName("Tilemap")->GetComponent<SpriteTilemap>();

		// Helper lambda to fill in an area with dots.
		// Params:
		//   min = The top left corner of the rectangle to fill.
		//   max = The bottom right corner of the rectangle to fill.
		auto Fill = [&](Vector2D min, Vector2D max)
		{
			// Fill area specified by min and max.
			for (float x = min.x; x <= max.x; x++)
			{
				for (float y = min.y; y <= max.y; y++)
				{
					if (dataMap->GetCellValue(static_cast<unsigned>(x), static_cast<unsigned>(y)) != 0)
						continue;

					Vector2D pos = spriteTilemap->TileToWorld(Vector2D(x, y));

					bool skip = false;

					// Make sure there are no duplicates.
					for (auto it = dotPositions.begin(); it != dotPositions.end(); ++it)
					{
						if (AlmostEqual(pos, *it))
						{
							skip = true;
							break;
						}
					}

					if (skip)
						continue;

					// Make sure it is not overlapping an energizer.
					for (auto it = energizerPositions.begin(); it != energizerPositions.end(); ++it)
					{
						if (AlmostEqual(pos, *it))
						{
							skip = true;
							break;
						}
					}

					if (skip)
						continue;

					dotPositions.push_back(pos);
				}
			}
		};

		// Add the energizers.
		energizerPositions.push_back(spriteTilemap->TileToWorld(Vector2D(1, 3)));
		energizerPositions.push_back(spriteTilemap->TileToWorld(Vector2D(26, 3)));
		energizerPositions.push_back(spriteTilemap->TileToWorld(Vector2D(1, 23)));
		energizerPositions.push_back(spriteTilemap->TileToWorld(Vector2D(26, 23)));

		// Add the dots.
		Fill(Vector2D(1, 1), Vector2D(26, 8));
		Fill(Vector2D(6, 9), Vector2D(6, 19));
		Fill(Vector2D(21, 9), Vector2D(21, 19));
		Fill(Vector2D(1, 20), Vector2D(26, 29));
	}
}

//----------------------------------------------------------------------------
