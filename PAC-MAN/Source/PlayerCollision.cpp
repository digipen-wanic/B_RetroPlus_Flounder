//------------------------------------------------------------------------------
//
// File Name:	PlayerCollision.cpp
// Author(s):	David Cohen (david.cohen)
// Project:		PAC-MAN
// Course:		WANIC VGP2 2018-2019
//
// Copyright © 2018 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "PlayerCollision.h"

// Systems
#include <Engine.h>
#include <SoundManager.h>
#include <Space.h>
#include <GameObjectManager.h>
#include <GameObject.h>
#include <Tilemap.h>

// Components
#include <Transform.h>
#include <SpriteTilemap.h>
#include "BaseAI.h"
#include "GhostAnimation.h"
#include "PlayerScore.h"
#include "PlayerController.h"
#include "PlayerAnimation.h"
#include "Energizer.h"

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

namespace
{
	// Helper function to round down a Vector2D.
	// Params:
	//   vec = The vector to round.
	// Returns:
	//   The rounded vector.
	Vector2D FloorVector2D(Vector2D vec)
	{
		return Vector2D(floor(vec.x + 0.5f), floor(vec.y + 0.5f));
	}
}

namespace Behaviors
{
	//------------------------------------------------------------------------------
	// Public Functions:
	//------------------------------------------------------------------------------

	// Constructor
	PlayerCollision::PlayerCollision() : Component("PlayerCollision"), tilemap(nullptr), spriteTilemap(nullptr), transform(nullptr), ghostStreak(0), oddConsumable(false), energizerSound(nullptr), ghostDeathSound(nullptr)
	{
	}

	// Initialize this component (happens at object creation).
	void PlayerCollision::Initialize()
	{
		transform = GetOwner()->GetComponent<Transform>();
		playerScore = GetOwner()->GetComponent<PlayerScore>();
		playerController = GetOwner()->GetComponent<PlayerController>();

		ghostDeathSound = Engine::GetInstance().GetModule<SoundManager>()->PlaySound("GhostRunningToHouse.wav");
		ghostDeathSound->setPaused(true);
		energizerSound = Engine::GetInstance().GetModule<SoundManager>()->PlaySound("Energized.wav");
		energizerSound->setPaused(true);
	}

	// Clone a component and return a pointer to the cloned component.
	// Returns:
	//   A pointer to a dynamically allocated clone of the component.
	Component* PlayerCollision::Clone() const
	{
		return new PlayerCollision(*this);
	}

	// Updates components using a fixed timestep (usually just physics)
	// Params:
	//	 dt = A fixed change in time, usually 1/60th of a second.
	void PlayerCollision::FixedUpdate(float dt)
	{
		UNREFERENCED_PARAMETER(dt);

		Vector2D playerTile = FloorVector2D(spriteTilemap->WorldToTile(transform->GetTranslation()));

		std::vector<GameObject*> enemies;

		GameObjectManager& objectManager = GetOwner()->GetSpace()->GetObjectManager();

		// Gather enemies.
		objectManager.GetAllObjectsByName("Blinky", enemies);
		objectManager.GetAllObjectsByName("Pinky", enemies);
		objectManager.GetAllObjectsByName("Inky", enemies);
		objectManager.GetAllObjectsByName("Clyde", enemies);
		objectManager.GetAllObjectsByName("KingGhost", enemies);

		for (auto it = enemies.begin(); it != enemies.end(); ++it)
		{
			Vector2D enemyTile = FloorVector2D(spriteTilemap->WorldToTile((*it)->GetComponent<Transform>()->GetTranslation()));

			// Check if the player and this enemy are on the same tile.
			if (AlmostEqual(playerTile, enemyTile))
			{
				BaseAI* baseAI = (*it)->GetComponent<BaseAI>();
				if (!baseAI->IsDead())
				{
					if (baseAI->IsFrightened())
					{
						// Eat the enemy.

						// Calculate the score to be added based on the current streak.
						int score = 0;
						switch (ghostStreak)
						{
						case 0:
							score = 200;
							break;
						case 1:
							score = 400;
							break;
						case 2:
							score = 800;
							break;
						default:
							score = 1600;
							break;
						}

						GameObject* scoreObject = new GameObject(*objectManager.GetArchetypeByName("Bonus" + std::to_string(score)));
						scoreObject->GetComponent<Transform>()->SetTranslation(transform->GetTranslation());
						objectManager.AddObject(*scoreObject);

						playerScore->IncreaseScore(score);
						baseAI->SetDead();
						Engine::GetInstance().GetModule<SoundManager>()->PlaySound("EatGhost.wav");

						// Let all ghosts know that a ghost was eaten.
						for (auto it2 = enemies.begin(); it2 != enemies.end(); ++it2)
						{
							(*it2)->GetComponent<GhostAnimation>()->FreezeCurrent(0.5f);
						}

						(*it)->GetComponent<GhostAnimation>()->FreezeBlank(0.5f);

						// Let the player know that a ghost was eaten.
						GetOwner()->GetComponent<PlayerAnimation>()->OnGhostEaten();

						++ghostStreak;
					}
					else
					{
						OnDeath();
					}
				}
			}
		}

		std::vector<GameObject*> gameObjects;
		objectManager.GetAllObjectsByName("Fruit", gameObjects);

		for (auto it = gameObjects.begin(); it != gameObjects.end(); ++it)
		{
			Vector2D fruit = FloorVector2D(spriteTilemap->WorldToTile((*it)->GetComponent<Transform>()->GetTranslation()));
			if (AlmostEqual(playerTile, fruit))
			{
				playerScore->IncreaseScore(100);
				(*it)->Destroy();
				Engine::GetInstance().GetModule<SoundManager>()->PlaySound("EatFruit.wav");

				GameObject* scoreObject = new GameObject(*objectManager.GetArchetypeByName("Bonus100"));
				scoreObject->GetComponent<Transform>()->SetTranslation(transform->GetTranslation());
				objectManager.AddObject(*scoreObject);
			}
		}

		gameObjects.clear();
		objectManager.GetAllObjectsByName("Dot", gameObjects);

		for (auto it = gameObjects.begin(); it != gameObjects.end(); ++it)
		{
			Vector2D dotTile = FloorVector2D(spriteTilemap->WorldToTile((*it)->GetComponent<Transform>()->GetTranslation()));

			// Check if the player and this dot are on the same tile.
			if (AlmostEqual(playerTile, dotTile))
			{
				// Add score and destroy the dot.
				playerScore->IncreaseScore(10);
				playerScore->IncreaseDots();
				(*it)->Destroy();

				if (oddConsumable)
				{
					Engine::GetInstance().GetModule<SoundManager>()->PlaySound("EatDot1.wav");
				}
				else
				{
					Engine::GetInstance().GetModule<SoundManager>()->PlaySound("EatDot2.wav");
				}

				playerController->AteDot();

				oddConsumable = !oddConsumable;
			}
		}

		gameObjects.clear();
		objectManager.GetAllObjectsByName("Energizer", gameObjects);

		for (auto it = gameObjects.begin(); it != gameObjects.end(); ++it)
		{
			Vector2D energizerTIle = FloorVector2D(spriteTilemap->WorldToTile((*it)->GetComponent<Transform>()->GetTranslation()));

			// Check if the player and this energizer are on the same tile.
			if (AlmostEqual(playerTile, energizerTIle))
			{
				// Add score and destroy the energizer.
				playerScore->IncreaseScore(50);
				(*it)->Destroy();
				playerController->AteEnergizer();

				// Reset the ghost streak.
				ghostStreak = 0;

				// Set all enemies to the frightened state.
				for (auto it2 = enemies.begin(); it2 != enemies.end(); ++it2)
				{
					(*it2)->GetComponent<BaseAI>()->SetFrightened();
				}
			}
		}

		bool enemyDead = false;
		bool enemyFrightened = false;

		for (auto it = enemies.begin(); it != enemies.end(); ++it)
		{
			BaseAI* baseAI = (*it)->GetComponent<BaseAI>();
			if (baseAI->IsDead())
				enemyDead = true;
			if (baseAI->IsFrightened())
				enemyFrightened = true;
		}

		if (enemyDead)
		{
			ghostDeathSound->setPaused(false);
			energizerSound->setPaused(true);
		}
		else if (enemyFrightened)
		{
			ghostDeathSound->setPaused(true);
			energizerSound->setPaused(false);
		}
		else
		{
			ghostDeathSound->setPaused(true);
			energizerSound->setPaused(true);
		}

		if (GetOwner()->GetComponent<PlayerAnimation>()->IsDying())
		{
			ghostDeathSound->setPaused(true);
			energizerSound->setPaused(true);
		}

		if (playerController->direction != oldDirection)
		{
			oddConsumable = !oddConsumable;
		}

		oldDirection = playerController->direction;
	}

	// Sets the tilemap used for the grid.
	// Params:
	//   tilemap = The tilemap.
	//   spriteTilemap = The sprite tilemap.
	void PlayerCollision::SetTilemap(Tilemap* tilemap_, SpriteTilemap* spriteTilemap_)
	{
		tilemap = tilemap_;
		spriteTilemap = spriteTilemap_;
	}

	// Called when the player dies.
	void PlayerCollision::OnDeath()
	{
		PlayerAnimation* playerAnimation = GetOwner()->GetComponent<PlayerAnimation>();

		// If the player is already dying, don't do anything.
		if (playerAnimation->IsDying())
			return;

		std::vector<GameObject*> enemies;

		GameObjectManager& objectManager = GetOwner()->GetSpace()->GetObjectManager();

		// Gather enemies.
		objectManager.GetAllObjectsByName("Blinky", enemies);
		objectManager.GetAllObjectsByName("Pinky", enemies);
		objectManager.GetAllObjectsByName("Inky", enemies);
		objectManager.GetAllObjectsByName("Clyde", enemies);
		objectManager.GetAllObjectsByName("KingGhost", enemies);

		// Freeze player.
		GetOwner()->GetComponent<GridMovement>()->SetFrozen(true);

		// Freeze ghosts.
		for (auto it = enemies.begin(); it != enemies.end(); ++it)
		{
			(*it)->GetComponent<GhostAnimation>()->FreezeCurrent(0.5f);
		}

		// Play death animation.
		playerAnimation->OnDeath();
	}
}

//------------------------------------------------------------------------------
