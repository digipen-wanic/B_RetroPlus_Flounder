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
#include "PlayerScore.h"
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
	PlayerCollision::PlayerCollision() : Component("PlayerCollision"), tilemap(nullptr), spriteTilemap(nullptr), transform(nullptr), oddConsumable(true)
	{
	}

	// Initialize this component (happens at object creation).
	void PlayerCollision::Initialize()
	{
		transform = GetOwner()->GetComponent<Transform>();
		playerScore = GetOwner()->GetComponent<PlayerScore>();
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

		for (auto it = enemies.begin(); it != enemies.end(); ++it)
		{
			Vector2D enemyTile = FloorVector2D(spriteTilemap->WorldToTile((*it)->GetComponent<Transform>()->GetTranslation()));

			// Check if the player and this enemy are on the same tile.
			if (AlmostEqual(playerTile, enemyTile))
			{
				if ((*it)->GetComponent<BaseAI>()->IsFrightened())
				{
					// Eat the enemy.
					playerScore->IncreaseScore(200); // TODO: ADD STREAKS
				}
				else
				{
					OnDeath();
				}
			}
		}

		std::vector<GameObject*> gameObjects;
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
				if (oddConsumable == true)
				{
					Engine::GetInstance().GetModule<SoundManager>()->PlaySound("eatDot1.wav");
					oddConsumable = false;
				}
				else
				{
					Engine::GetInstance().GetModule<SoundManager>()->PlaySound("eatDot2.wav");
					oddConsumable = true;
				}
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

				// Set all enemies to the frightened state.
				for (auto it2 = enemies.begin(); it2 != enemies.end(); ++it2)
				{
					(*it2)->GetComponent<BaseAI>()->SetFrightened();
				}
			}
		}
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

		// Freeze player.
		GetOwner()->GetComponent<GridMovement>()->SetFrozen(true);

		// Freeze ghosts.
		for (auto it = enemies.begin(); it != enemies.end(); ++it)
		{
			(*it)->GetComponent<GridMovement>()->SetFrozen(true);
		}

		// Play death sound.
		Engine::GetInstance().GetModule<SoundManager>()->PlaySound("deathofpacfinal.wav");

		// Play death animation.
		playerAnimation->OnDeath();
	}
}

//------------------------------------------------------------------------------
