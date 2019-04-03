//------------------------------------------------------------------------------
//
// File Name:	BaseAI.cpp
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

#include "BaseAI.h"

// Systems
#include <GameObject.h>
#include <Space.h>
#include <GameObjectManager.h>
#include <Parser.h>
#include <Graphics.h>
#include <Camera.h>
#include <DebugDraw.h>

// Components
#include <Transform.h>
#include <SpriteTilemap.h>
#include "PlayerScore.h"
#include "GhostAnimation.h"

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

namespace Behaviors
{
	//------------------------------------------------------------------------------
	// Public Functions:
	//------------------------------------------------------------------------------

	// Constructor
	// Params:
	//   dotsLeftToLeave = How many dots the player must eat before the ghost moves.
	BaseAI::BaseAI(unsigned dotsLeftToLeave) : player(nullptr), target(), scatterTarget(), mode(CHASE), ghostAnimation(nullptr),
		hasMoved(false), dotsLeftToLeave(dotsLeftToLeave), forceReverse(false), isDead(false), wave(0), waveTimer(0.0f), frightenTime(6.0f), frightTimer(0.0f),
		overriddenTiles(), overriddenExclusionTiles()
	{
		// Set waveTime Timers
		waveTime[0] = 7.0f;
		waveTime[1] = 20.0f;
		waveTime[2] = 7.0f;
		waveTime[3] = 20.0f;
		waveTime[4] = 5.0f;
		waveTime[5] = 20.0f;
		waveTime[6] = 5.0f;
		waveTime[7] = -1.0f; // Indefinite time
	}

	// Initialize this component (happens at object creation).
	void BaseAI::Initialize()
	{
		GridMovement::Initialize();
		SetFrozen(true);

		ghostAnimation = GetOwner()->GetComponent<GhostAnimation>();

		player = GetOwner()->GetSpace()->GetObjectManager().GetObjectByName("PAC-MAN");
	}

	// Updates components using a fixed timestep (usually just physics)
	// Params:
	//	 dt = A fixed change in time, usually 1/60th of a second.
	void BaseAI::FixedUpdate(float dt)
	{
		if (!hasMoved && ghostAnimation->currentState != GhostAnimation::State::StateSpawn)
		{
			if (player->GetComponent<PlayerScore>()->GetDots() >= dotsLeftToLeave)
			{
				SetFrozen(false);
				hasMoved = true;
			}
		}

		// Check if current mode isn't Frightened or the last wave
		if (frightTimer >= 0.0f || wave == 7)
		{
			// Set frightTimer to 0
			frightTimer = 0.0f;
			// Reset Speed
			GetOwner()->GetComponent<GridMovement>()->SetSpeed(normSpeed);
			// Increment waveTimer by dt
			waveTimer += dt;

			// Check if the current waveTimer has been reached
			if (waveTimer >= waveTime[wave])
			{
				// Switch between Chase and Scatter mode
				mode == CHASE ? mode = SCATTER : mode = CHASE;
				// Set forceReverse to true
				forceReverse = true;
				// Reset waveTimer
				waveTimer = 0.0f;
				// Increment wave
				++wave;
			}
		}
		else 
		{
			// Decrement frightTimer 
			frightTimer -= dt; 
		}

		GridMovement::FixedUpdate(dt);
	}

	// Write object data to file
	// Params:
	//   parser = The parser that is writing this object to a file.
	void BaseAI::Serialize(Parser& parser) const
	{
		GridMovement::Serialize(parser);

		parser.WriteVariable("normSpeed", normSpeed);
		parser.WriteVariable("frightSpeed", frightSpeed);
		parser.WriteVariable("scatterTarget", scatterTarget);

		parser.WriteVariable("overriddenTilesCount", overriddenTiles.size());
		parser.WriteValue("overriddenTiles : ");
		parser.BeginScope();

		for (auto it = overriddenTiles.begin(); it != overriddenTiles.end(); ++it)
			parser.WriteValue(*it);

		parser.EndScope();

		parser.WriteVariable("overriddenExclusionTilesCount", overriddenExclusionTiles.size());
		parser.WriteValue("overriddenExclusionTiles : ");
		parser.BeginScope();

		for (auto it = overriddenExclusionTiles.begin(); it != overriddenExclusionTiles.end(); ++it)
			parser.WriteValue(*it);

		parser.EndScope();
	}

	// Read object data from a file
	// Params:
	//   parser = The parser that is reading this object's data from a file.
	void BaseAI::Deserialize(Parser& parser)
	{
		GridMovement::Deserialize(parser);

		parser.ReadVariable("normSpeed", normSpeed);
		parser.ReadVariable("frightSpeed", frightSpeed);
		parser.ReadVariable("scatterTarget", scatterTarget);

		size_t overriddenTilesCount;
		parser.ReadVariable("overriddenTilesCount", overriddenTilesCount);
		overriddenTiles.reserve(overriddenTilesCount);

		parser.ReadSkip("overriddenTiles");
		parser.ReadSkip(':');
		parser.ReadSkip("{");

		for (size_t i = 0; i < overriddenTilesCount; i++)
		{
			OverriddenTile overriddenTile;
			parser.ReadValue(overriddenTile);
			overriddenTiles.push_back(overriddenTile);
		}
		parser.ReadSkip("}");

		size_t overriddenExclusionTilesCount;
		parser.ReadVariable("overriddenExclusionTilesCount", overriddenExclusionTilesCount);
		overriddenExclusionTiles.reserve(overriddenExclusionTilesCount);

		parser.ReadSkip("overriddenExclusionTiles");
		parser.ReadSkip(':');
		parser.ReadSkip("{");

		for (size_t i = 0; i < overriddenExclusionTilesCount; i++)
		{
			OverriddenTile overriddenExclusionTile;
			parser.ReadValue(overriddenExclusionTile);
			overriddenExclusionTiles.push_back(overriddenExclusionTile);
		}
		parser.ReadSkip("}");
	}

	// Draw a sprite (Sprite can be textured or untextured).
	void BaseAI::Draw()
	{
		DebugDraw& debugDraw = DebugDraw::GetInstance();
		debugDraw.AddRectangle(GetSpriteTilemap()->TileToWorld(target), Vector2D(8.0f, 8.0f), Graphics::GetInstance().GetCurrentCamera(), Colors::Red);
	}

	// Sets the ghost to the frightened state.
	void BaseAI::SetFrightened()
	{
		mode = FRIGHTENED;
		frightTimer = frightenTime;
		GetOwner()->GetComponent<GridMovement>()->SetSpeed(frightSpeed);
	}

	// Returns whether the ghost is in the frightened state.
	bool BaseAI::IsFrightened() const
	{
		return mode == FRIGHTENED;
	}

	// Marks the enemy as dead.
	void BaseAI::SetDead()
	{
		isDead = true;
	}

	// Returns whether the ghost is dead.
	bool BaseAI::IsDead() const
	{
		return isDead;
	}

	// Adds an overridden direction for a specific tile.
		// Params:
		//   tile = The coordinates of the tile to add an override for.
		//   overriddenDirection = The direction the AI should be forced to move in when encountering the specified tile.
	void BaseAI::AddOverrideTile(Vector2D tile, Direction overriddenDirection)
	{
		overriddenTiles.push_back({ tile, overriddenDirection });
	}

	// Adds an overridden direction exclusion for a specific tile.
	// Params:
	//   tile = The coordinates of the tile to add an override for.
	//   excludedDirection = The direction the AI cannot move in when encountering the specified tile.
	void BaseAI::AddOverrideExclusionTile(Vector2D tile, Direction excludedDirection)
	{
		overriddenExclusionTiles.push_back({ tile, excludedDirection });
	}

	//------------------------------------------------------------------------------
	// Protected Functions:
	//------------------------------------------------------------------------------

	// Called when finished moving to the next tile.
	// Params:
	//   adjacentTiles = An array of adjacent tiles.
	//   emptyCount = How many empty tiles were found.
	void BaseAI::OnTileMove(AdjacentTile adjacentTiles[4], size_t emptyCount)
	{
		UNREFERENCED_PARAMETER(emptyCount);

		if (isDead)
		{
			target = Vector2D(13, 14);

			if (AlmostEqual(GetNewTile(), target))
			{
				isDead = false;
			}

			if (isDead)
			{
				Pathfind(adjacentTiles, emptyCount);
				return;
			}
		}

		// Check all overridden tiles.
		for (auto it = overriddenTiles.begin(); it != overriddenTiles.end(); ++it)
		{
			// Check if this tile is overridden.
			if (AlmostEqual(it->pos, Vector2D(floor(GetNewTile().x), floor(GetNewTile().y))))
			{
				// Set the direction to the overridden direction.
				direction = it->direction;
				return;
			}
		}

		// If the ghost is forced to invert direction, handle that here.
		if (forceReverse)
		{
			// Invert direction.
			direction = static_cast<Direction>((direction + DIRECTION_MAX / 2) % DIRECTION_MAX);
			forceReverse = false;
			return;
		}

		// If we are in frigthened mode, choose random directions.
		if (mode == FRIGHTENED)
		{
			// Choose a random direction that has an empty tile and is not backwards.
			AdjacentTile adjacentTile;
			do
			{
				adjacentTile = adjacentTiles[rand() % 4];
			} while (!adjacentTile.empty || adjacentTile.direction == (direction + DIRECTION_MAX / 2) % DIRECTION_MAX);

			// Set the direction to the tile.
			direction = adjacentTile.direction;
			return;
		}

		// Let child class handle targeting.
		OnTarget(adjacentTiles, emptyCount);

		Pathfind(adjacentTiles, emptyCount);
	}

	// Called when met with an intersection after finishing moving to the next tile.
	// Params:
	//   adjacentTiles = An array of adjacent empty tiles.
	//   emptyCount = How many empty tiles were found.
	void BaseAI::OnIntersection(AdjacentTile adjacentTiles[4], size_t emptyCount)
	{
		UNREFERENCED_PARAMETER(adjacentTiles);
		UNREFERENCED_PARAMETER(emptyCount);
	}

	//------------------------------------------------------------------------------
	// Private Functions:
	//------------------------------------------------------------------------------

	// Handles pathfinding and setting direction.
	// Params:
	//   adjacentTiles = An array of adjacent tiles.
	//   emptyCount = How many empty tiles were found.
	void BaseAI::Pathfind(AdjacentTile adjacentTiles[4], size_t emptyCount)
	{
		UNREFERENCED_PARAMETER(emptyCount);

		AdjacentTile closest;
		float closestDistance = 999.0f;

		// Choose closest tile to move towards.
		for (size_t i = 0; i < 4; i++)
		{
			bool excluded = false;
			for (auto it = overriddenExclusionTiles.begin(); it != overriddenExclusionTiles.end(); ++it)
			{
				if (AlmostEqual(it->pos, Vector2D(floor(GetNewTile().x), floor(GetNewTile().y))) && it->direction == adjacentTiles[i].direction)
				{
					excluded = true;
					break;
				}
			}
			if (excluded)
				continue;

			// Make sure the tile is empty and would not be backtracking.
			bool valid;
			if ((adjacentTiles[i].empty || isDead && GetCellValue(adjacentTiles[i].pos, valid) == 31) && !AlmostEqual(adjacentTiles[i].pos, GetOldTile()))
			{
				float distance = adjacentTiles[i].pos.Distance(target);
				if (distance < closestDistance)
				{
					closestDistance = distance;
					closest = adjacentTiles[i];
				}
			}
		}

		// Move in the direction of the closest tile.
		direction = closest.direction;
	}

	// Insertion operator for OverriddenTile.
	// Params:
	//   os = The output stream.
	//   overriddenTile = The overridden tile.
	// Returns:
	//   A reference to the output stream.
	std::ostream& operator<<(std::ostream& os, const BaseAI::OverriddenTile& overriddenTile)
	{
		return os << "{ " << overriddenTile.pos << ", " << overriddenTile.direction << " }";
	}

	// Extraction operator for OverriddenTile.
	// Params:
	//   is = The input stream.
	//   overriddenTile = The overridden tile.
	// Returns:
	//   A reference to the input stream.
	std::istream& operator>>(std::istream& is, BaseAI::OverriddenTile& overriddenTile)
	{
		int direction;
		is.ignore(10, '{');
		is >> overriddenTile.pos;
		is.ignore(10, ',');
		is >> direction;
		is.ignore(10, '}');
		overriddenTile.direction = static_cast<BaseAI::Direction>(direction);
		return is;
	}
}

//------------------------------------------------------------------------------
