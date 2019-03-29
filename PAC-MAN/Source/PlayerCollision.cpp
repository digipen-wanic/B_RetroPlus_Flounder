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
#include <Space.h>
#include <GameObjectManager.h>
#include <GameObject.h>
#include <Tilemap.h>

// Components
#include <Transform.h>
#include <SpriteTilemap.h>
#include "BaseAI.h"
#include "PlayerScore.h"
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
		return Vector2D(floor(vec.x), floor(vec.y));
	}
}

namespace Behaviors
{
	//------------------------------------------------------------------------------
	// Public Functions:
	//------------------------------------------------------------------------------

	// Constructor
	PlayerCollision::PlayerCollision() : Component("PlayerCollision"), tilemap(nullptr), spriteTilemap(nullptr), transform(nullptr)
	{
	}

	// Initialize this component (happens at object creation).
	void PlayerCollision::Initialize()
	{
		transform = GetOwner()->GetComponent<Transform>();
		playerScore = GetOwner()->GetComponent<PlayerScore>();
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

		objectManager.GetAllObjectsByName("Blinky", enemies);
		objectManager.GetAllObjectsByName("Pinky", enemies);
		objectManager.GetAllObjectsByName("Inky", enemies);
		objectManager.GetAllObjectsByName("Clyde", enemies);

		for (auto it = enemies.begin(); it != enemies.end(); ++it)
		{
			Vector2D enemyTile = FloorVector2D(spriteTilemap->WorldToTile((*it)->GetComponent<Transform>()->GetTranslation()));
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
		objectManager.GetAllObjectsByName("Dots", gameObjects);

		for (auto it = gameObjects.begin(); it != gameObjects.end(); ++it)
		{
			Vector2D dotTile = FloorVector2D(spriteTilemap->WorldToTile((*it)->GetComponent<Transform>()->GetTranslation()));
			if (AlmostEqual(playerTile, dotTile))
			{
				playerScore->IncreaseScore(10);
				playerScore->IncreaseDots();
			}
		}

		gameObjects.clear();
		objectManager.GetAllObjectsByName("Energizer", gameObjects);

		for (auto it = gameObjects.begin(); it != gameObjects.end(); ++it)
		{
			Vector2D energizerTIle = FloorVector2D(spriteTilemap->WorldToTile((*it)->GetComponent<Transform>()->GetTranslation()));
			if (AlmostEqual(playerTile, energizerTIle))
			{
				playerScore->IncreaseScore(50);
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
		// TODO: make player dead
	}
}

//------------------------------------------------------------------------------
