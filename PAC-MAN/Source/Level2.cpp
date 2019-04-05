//------------------------------------------------------------------------------
//
// File Name:	Level2.cpp
// Author(s):	David Cohen (david.cohen) & Tyler Miller (miller.t)
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
#include <MeshHelper.h>
#include <Random.h>

// Components
#include <Transform.h>
#include <Physics.h>
#include <SpriteTilemap.h>
#include <Animation.h>
#include "GridMovement.h"
#include "BaseAI.h"
#include "PlayerScore.h"
#include "PlayerCollision.h"
#include "PlayerAnimation.h"
#include <SpriteText.h>

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
		columnsMap(8), rowsMap(5), columnsEnergizer(4), columnsDot(2), rowsDot(1), rowsEnergizer(1), columnsPacMan(4), rowsPacMan(4), columnsGhost(5), rowsGhost(5),
		musicTimer(0.0f), musicPlayed(false), musicIntroPlayed(false), readyTimer(0.0f),
		gameOver(false), startLives(3), lives(0), oldScore(0), oldDots(0), highScore(0),
		/*blinkyWave(0), blinkyWaveTimer(0.0f), pinkyWave(0), pinkyWaveTimer(0.0f),
		inkyWave(0), inkyWaveTimer(0.0f), clydeWave(0), clydeWaveTimer(0.0f),*/
		kingGhostWave(0), kingGhostWaveTimer(0.0f),
		fruitSpawnAmount(0), fruitDeathTimer(0), fruitAlive(false),
		soundManager(nullptr), energizerPositions(), dotPositions(),
		readyText(nullptr), scoreText(nullptr), pacMan(nullptr), fruit(nullptr)
	{
	}

	// Load the resources associated with Level 2.
	void Level2::Load()
	{
		// Load sound effects.
		soundManager = Engine::GetInstance().GetModule<SoundManager>();
		soundManager->AddEffect("Start.wav");
		soundManager->AddEffect("PacManDeath.wav");
		soundManager->AddEffect("EatDot1.wav");
		soundManager->AddEffect("EatDot2.wav");
		soundManager->AddEffect("EatFruit.wav");
		soundManager->AddEffect("EatGhost.wav");
		soundManager->AddEffect("ExtraLife.wav");
		soundManager->AddEffect("GhostRunningToHouse.wav", true);
		soundManager->AddEffect("Energized.wav", true);
		soundManager->AddEffect("PlusMusicIntro.wav");
		soundManager->AddMusic("PlusMusicLoop.wav");

		// If we played the menu sound in the menu level, it would get cut off as the level unloaded, so we're playing it here instead.
		soundManager->PlaySound("Start.wav");

		GameObjectFactory& objectFactory = GameObjectFactory::GetInstance();
		GameObjectManager& objectManager = GetSpace()->GetObjectManager();
		ResourceManager& resourceManager = GetSpace()->GetResourceManager();

		// Create a new quad mesh for the sprite.
		resourceManager.GetMesh("Quad");
		resourceManager.GetMesh("Dot", columnsDot, rowsDot);
		resourceManager.GetMesh("Energizer", columnsEnergizer, rowsEnergizer);
		resourceManager.GetMesh("PacMan", columnsPacMan, rowsPacMan);
		//resourceManager.GetMesh("Blinky", columnsGhost, rowsGhost);
		//resourceManager.GetMesh("Pinky", columnsGhost, rowsGhost);
		//resourceManager.GetMesh("Inky", columnsGhost, rowsGhost);
		//resourceManager.GetMesh("Clyde", columnsGhost, rowsGhost);
		resourceManager.GetMesh("KingGhost", columnsGhost, rowsGhost);

		// Create a new quad mesh for the sprite.
		resourceManager.GetSpriteSource("Missile_Command.png", 12, 8);
		resourceManager.GetMesh("FontAtlas", 12, 8);

		// Load the circle texture and sprite source.
		resourceManager.GetSpriteSource("++Dot.png", columnsDot, rowsDot);
		resourceManager.GetSpriteSource("++Energizer.png", columnsEnergizer, rowsEnergizer);
		resourceManager.GetSpriteSource("++PacMan.png", columnsPacMan, rowsPacMan);
		//resourceManager.GetSpriteSource("Blinky.png", columnsGhost, rowsGhost);
		//resourceManager.GetSpriteSource("Pinky.png", columnsGhost, rowsGhost);
		//resourceManager.GetSpriteSource("Inky.png", columnsGhost, rowsGhost);
		//resourceManager.GetSpriteSource("Clyde.png", columnsGhost, rowsGhost);
		resourceManager.GetSpriteSource("++KingGhost.png", columnsGhost, rowsGhost);
		resourceManager.GetSpriteSource("++Cherry.png");
		resourceManager.GetSpriteSource("Bonus100.png");
		resourceManager.GetSpriteSource("Bonus200.png");
		resourceManager.GetSpriteSource("Bonus400.png");
		resourceManager.GetSpriteSource("Bonus800.png");
		resourceManager.GetSpriteSource("Bonus1600.png");
		resourceManager.GetSpriteSource("PacManIcon.png");

		// Load the archetypes from their files.
		objectManager.AddArchetype(*objectFactory.CreateObject("Dot", resourceManager.GetMesh("Dot"), resourceManager.GetSpriteSource("++Dot.png")));
		objectManager.AddArchetype(*objectFactory.CreateObject("Energizer", resourceManager.GetMesh("Energizer"), resourceManager.GetSpriteSource("++Energizer.png")));
		objectManager.AddArchetype(*objectFactory.CreateObject("PAC-MAN", resourceManager.GetMesh("PacMan"), resourceManager.GetSpriteSource("++PacMan.png")));
		//objectManager.AddArchetype(*objectFactory.CreateObject("Blinky", resourceManager.GetMesh("Blinky"), resourceManager.GetSpriteSource("Blinky.png")));
		//objectManager.AddArchetype(*objectFactory.CreateObject("Pinky", resourceManager.GetMesh("Pinky"), resourceManager.GetSpriteSource("Pinky.png")));
		//objectManager.AddArchetype(*objectFactory.CreateObject("Inky", resourceManager.GetMesh("Inky"), resourceManager.GetSpriteSource("Inky.png")));
		//objectManager.AddArchetype(*objectFactory.CreateObject("Clyde", resourceManager.GetMesh("Clyde"), resourceManager.GetSpriteSource("Clyde.png")));
		objectManager.AddArchetype(*objectFactory.CreateObject("KingGhost", resourceManager.GetMesh("KingGhost"), resourceManager.GetSpriteSource("++KingGhost.png")));
		objectManager.AddArchetype(*objectFactory.CreateObject("HUDText", resourceManager.GetMesh("FontAtlas"), resourceManager.GetSpriteSource("Missile_Command.png")));
		objectManager.AddArchetype(*objectFactory.CreateObject("Fruit", resourceManager.GetMesh("Quad"), resourceManager.GetSpriteSource("++Cherry.png")));
		objectManager.AddArchetype(*objectFactory.CreateObject("Bonus100", resourceManager.GetMesh("Quad"), resourceManager.GetSpriteSource("Bonus100.png")));
		objectManager.AddArchetype(*objectFactory.CreateObject("Bonus200", resourceManager.GetMesh("Quad"), resourceManager.GetSpriteSource("Bonus200.png")));
		objectManager.AddArchetype(*objectFactory.CreateObject("Bonus400", resourceManager.GetMesh("Quad"), resourceManager.GetSpriteSource("Bonus400.png")));
		objectManager.AddArchetype(*objectFactory.CreateObject("Bonus800", resourceManager.GetMesh("Quad"), resourceManager.GetSpriteSource("Bonus800.png")));
		objectManager.AddArchetype(*objectFactory.CreateObject("Bonus1600", resourceManager.GetMesh("Quad"), resourceManager.GetSpriteSource("Bonus1600.png")));
		objectManager.AddArchetype(*objectFactory.CreateObject("Life", resourceManager.GetMesh("Quad"), resourceManager.GetSpriteSource("PacManIcon.png")));

		// Load the tilemap.
		dataMap = Tilemap::CreateTilemapFromFile("Assets/Levels/Level1.txt");
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

		std::fstream highScoreFile("Assets/highScore.txt", std::ios::in);
		if (highScoreFile.good())
		{
			highScoreFile >> highScore;
		}
	}

	// Initialize the memory associated with Level 2.
	void Level2::Initialize()
	{
		GameObjectManager& objectManager = GetSpace()->GetObjectManager();

		// Check if there are any lives remaining.
		gameOver = lives == 0;

		// Tilemap.
		GameObject* tilemap = new GameObject(*objectManager.GetArchetypeByName("Tilemap"));
		objectManager.AddObject(*tilemap);

		SpriteTilemap* spriteTilemap = tilemap->GetComponent<SpriteTilemap>();

		if (lives == startLives)
		{
			// Place energizers and dots.
			PopulateDots();
		}

		// Add various HUD elements.
		readyText = new GameObject(*objectManager.GetArchetypeByName("HUDText"));
		readyText->GetComponent<Transform>()->SetTranslation(spriteTilemap->TileToWorld(Vector2D(14.0f, 17.25f)));
		readyText->GetComponent<SpriteText>()->SetColor(Colors::Yellow);
		readyText->GetComponent<SpriteText>()->SetHorizontalAlignment(SpriteText::Alignment::CENTER);
		readyText->GetComponent<SpriteText>()->SetText("READY!");
		objectManager.AddObject(*readyText);
		readyTimer = 4.0f;

		scoreText = new GameObject(*objectManager.GetArchetypeByName("HUDText"));
		scoreText->GetComponent<Transform>()->SetTranslation(spriteTilemap->TileToWorld(Vector2D(6.75f, -1.5f)));
		scoreText->GetComponent<SpriteText>()->SetText(std::to_string(oldScore).c_str());
		objectManager.AddObject(*scoreText);

		highScoreText = new GameObject(*objectManager.GetArchetypeByName("HUDText"));
		highScoreText->GetComponent<Transform>()->SetTranslation(spriteTilemap->TileToWorld(Vector2D(16.75f, -1.5f)));
		highScoreText->GetComponent<SpriteText>()->SetText(std::to_string(oldScore).c_str());
		objectManager.AddObject(*highScoreText);

		GameObject* highScoreLabelText = new GameObject(*objectManager.GetArchetypeByName("HUDText"));
		highScoreLabelText->GetComponent<Transform>()->SetTranslation(spriteTilemap->TileToWorld(Vector2D(18.75f, -2.5f)));
		highScoreLabelText->GetComponent<SpriteText>()->SetText("HIGH SCORE");
		objectManager.AddObject(*highScoreLabelText);

		lifeIcons[0] = new GameObject(*objectManager.GetArchetypeByName("Life"));
		lifeIcons[0]->GetComponent<Transform>()->SetTranslation(spriteTilemap->TileToWorld(Vector2D(2.5f, 31.5f)));
		lifeIcons[0]->GetComponent<Transform>()->SetScale(Vector2D(-40.0f, 40.0f));
		objectManager.AddObject(*lifeIcons[0]);
		lifeIcons[1] = new GameObject(*objectManager.GetArchetypeByName("Life"));
		lifeIcons[1]->GetComponent<Transform>()->SetTranslation(spriteTilemap->TileToWorld(Vector2D(4.5f, 31.5f)));
		lifeIcons[1]->GetComponent<Transform>()->SetScale(Vector2D(-40.0f, 40.0f));
		objectManager.AddObject(*lifeIcons[1]);

		if (!gameOver)
		{
			// PAC-MAN.
			pacMan = new GameObject(*objectManager.GetArchetypeByName("PAC-MAN"));
			pacMan->GetComponent<Behaviors::GridMovement>()->SetTilemap(dataMap, spriteTilemap);
			pacMan->GetComponent<Behaviors::PlayerCollision>()->SetTilemap(dataMap, spriteTilemap);
			pacMan->GetComponent<Transform>()->SetTranslation(spriteTilemap->TileToWorld(Vector2D(13.5f, 23.0f)));

			// Restore the player's score, assuming they had lives left.
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
			energizerPellet->GetComponent<Animation>()->Play(0, 4, 0.0625f, true);
		}

		for (auto it = dotPositions.begin(); it != dotPositions.end(); ++it)
		{
			// Create dot at position.
			GameObject* dot = new GameObject(*objectManager.GetArchetypeByName("Dot"));
			dot->GetComponent<Transform>()->SetTranslation(*it);
			objectManager.AddObject(*dot);
			dot->GetComponent<Animation>()->Play(0, 2, 0.125f, true);
		}

		if (!gameOver)
		{
			// Ghosts.
			/*GameObject* blinky = new GameObject(*objectManager.GetArchetypeByName("Blinky"));
			blinky->GetComponent<Behaviors::GridMovement>()->SetTilemap(dataMap, tilemap->GetComponent<SpriteTilemap>());
			blinky->GetComponent<Behaviors::BaseAI>()->SetWaveProgress(blinkyWave, blinkyWaveTimer);
			blinky->GetComponent<Transform>()->SetTranslation(spriteTilemap->TileToWorld(Vector2D(13.5f, 11.0f)));
			objectManager.AddObject(*blinky);

			GameObject* pinky = new GameObject(*objectManager.GetArchetypeByName("Pinky"));
			pinky->GetComponent<Behaviors::GridMovement>()->SetTilemap(dataMap, tilemap->GetComponent<SpriteTilemap>());
			pinky->GetComponent<Behaviors::BaseAI>()->SetWaveProgress(pinkyWave, pinkyWaveTimer);
			pinky->GetComponent<Transform>()->SetTranslation(spriteTilemap->TileToWorld(Vector2D(13.5f, 14.0f)));
			objectManager.AddObject(*pinky);

			GameObject* inky = new GameObject(*objectManager.GetArchetypeByName("Inky"));
			inky->GetComponent<Behaviors::GridMovement>()->SetTilemap(dataMap, tilemap->GetComponent<SpriteTilemap>());
			inky->GetComponent<Behaviors::BaseAI>()->SetWaveProgress(inkyWave, inkyWaveTimer);
			inky->GetComponent<Transform>()->SetTranslation(spriteTilemap->TileToWorld(Vector2D(11.5f, 14.0f)));
			objectManager.AddObject(*inky);

			GameObject* clyde = new GameObject(*objectManager.GetArchetypeByName("Clyde"));
			clyde->GetComponent<Behaviors::GridMovement>()->SetTilemap(dataMap, tilemap->GetComponent<SpriteTilemap>());
			clyde->GetComponent<Behaviors::BaseAI>()->SetWaveProgress(clydeWave, clydeWaveTimer);
			clyde->GetComponent<Transform>()->SetTranslation(spriteTilemap->TileToWorld(Vector2D(15.5f, 14.0f)));
			objectManager.AddObject(*clyde);*/

			GameObject* kingGhost = new GameObject(*objectManager.GetArchetypeByName("KingGhost"));
			kingGhost->GetComponent<Behaviors::GridMovement>()->SetTilemap(dataMap, tilemap->GetComponent<SpriteTilemap>());
			kingGhost->GetComponent<Behaviors::BaseAI>()->SetWaveProgress(kingGhostWave, kingGhostWaveTimer);
			kingGhost->GetComponent<Transform>()->SetTranslation(spriteTilemap->TileToWorld(Vector2D(15.5f, 14.0f)));
			objectManager.AddObject(*kingGhost);

			// Add PAC-MAN here so it draws over everything else.
			objectManager.AddObject(*pacMan);

			// Re-initialize all ghosts so they can find the player object since it was added after them.
			/*blinky->Initialize();
			pinky->Initialize();
			inky->Initialize();
			clyde->Initialize();*/
			kingGhost->Initialize();

			--lives;
		}
		else
		{
			readyText->GetComponent<Transform>()->SetTranslation(spriteTilemap->TileToWorld(Vector2D(13.75f, 17.25f)));
			readyText->GetComponent<SpriteText>()->SetText("GAME OVER");
			readyText->GetComponent<SpriteText>()->SetColor(Colors::Red);
		}
	}

	// Update Level 2.
	// Params:
	//	 dt = Change in time (in seconds) since the last game loop.
	void Level2::Update(float dt)
	{
		UNREFERENCED_PARAMETER(dt);

		musicTimer -= dt;
		if (musicTimer <= 0 && !musicPlayed)
		{
			soundManager->PlaySound("PlusMusicIntro.wav");
			musicPlayed = true;
			musicTimer = 6;
		}
		if (!musicIntroPlayed && musicTimer <= 0)
		{
			soundManager->SetMusicVolume(1);
			soundManager->PlaySound("PlusMusicLoop.wav");
			musicIntroPlayed = true;
		}

		// Update score & high score in HUD
		if (!gameOver)
		{
			unsigned score = pacMan->GetComponent<Behaviors::PlayerScore>()->GetScore();

			if (highScore < score)
				highScore = score;

			scoreText->GetComponent<SpriteText>()->SetText(std::to_string(score).c_str());
		}

		highScoreText->GetComponent<SpriteText>()->SetText(std::to_string(highScore).c_str());

		// Handle ready text
		readyTimer -= dt;
		if (readyTimer <= 0.0f)
		{
			readyText->GetComponent<SpriteText>()->SetAlpha(0.0f);

			if (gameOver)
			{
				// Switch to main menu once text disappears.
				GetSpace()->SetLevel<MainMenu>();
			}
		}
		else
		{
			readyText->GetComponent<SpriteText>()->SetAlpha(1.0f);
		}

		// Handle life icons
		for (unsigned i = 0; i < startLives - 1; i++)
		{
			if (i < lives)
				lifeIcons[i]->GetComponent<Sprite>()->SetAlpha(1.0f);
			else
				lifeIcons[i]->GetComponent<Sprite>()->SetAlpha(0.0f);
		}

		if (!gameOver)
		{
			GameObjectManager& objectManager = GetSpace()->GetObjectManager();

			// Handle fruit spawning
			if (fruitAlive)
			{
				fruitDeathTimer -= dt;
				if (fruitDeathTimer <= 0 && fruit != nullptr)
				{
					fruit->Destroy();
					fruitAlive = false;
				}
			}

			// First fruit
			if (pacMan->GetComponent<Behaviors::PlayerScore>()->GetDots() >= 70 && fruitSpawnAmount == 0)
			{
				fruitDeathTimer = RandomRange(9.0f, 10.0f);
				fruit = new GameObject(*objectManager.GetArchetypeByName("Fruit"));
				objectManager.AddObject(*fruit);
				++fruitSpawnAmount;
				fruitAlive = true;
			}

			// Second fruit
			if (pacMan->GetComponent<Behaviors::PlayerScore>()->GetDots() >= 170 && fruitSpawnAmount == 1)
			{
				fruitDeathTimer = RandomRange(9.0f, 10.0f);
				fruit = new GameObject(*objectManager.GetArchetypeByName("Fruit"));
				objectManager.AddObject(*fruit);
				++fruitSpawnAmount;
				fruitAlive = true;
			}
		}
	}

	// Destroy objects associated with Level 2.
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

		if (!gameOver)
		{
			Behaviors::PlayerScore* playerScore = pacMan->GetComponent<Behaviors::PlayerScore>();
			oldScore = playerScore->GetScore();
			oldDots = playerScore->GetDots();

			/*objectManager.GetObjectByName("Blinky")->GetComponent<Behaviors::BaseAI>()->GetWaveProgress(blinkyWave, blinkyWaveTimer);
			objectManager.GetObjectByName("Pinky")->GetComponent<Behaviors::BaseAI>()->GetWaveProgress(pinkyWave, pinkyWaveTimer);
			objectManager.GetObjectByName("Inky")->GetComponent<Behaviors::BaseAI>()->GetWaveProgress(inkyWave, inkyWaveTimer);
			objectManager.GetObjectByName("Clyde")->GetComponent<Behaviors::BaseAI>()->GetWaveProgress(clydeWave, clydeWaveTimer);*/
			objectManager.GetObjectByName("KingGhost")->GetComponent<Behaviors::BaseAI>()->GetWaveProgress(kingGhostWave, kingGhostWaveTimer);
		}
	}

	// Unload the resources associated with Level 2.
	void Level2::Unload()
	{
		// Free all allocated memory.
		delete dataMap;

		// Unload all sounds.
		soundManager->Shutdown();

		std::fstream highScoreFile("Assets/highScore.txt", std::ios::out);
		if (highScoreFile.good())
		{
			highScoreFile << highScore;
		}
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
					// If there is a solid tile at this coordinate or it is overlapping PAC-MAN, skip this coordinate.
					if (dataMap->GetCellValue(static_cast<unsigned>(x), static_cast<unsigned>(y)) != 0 || (x >= 13.0f && x <= 14.0f && y == 23.0f))
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
