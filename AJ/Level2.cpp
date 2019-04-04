//------------------------------------------------------------------------------
//
// File Name:	Level2.cpp
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
#include <Transform.h>
#include <Physics.h>
#include <SpriteTilemap.h>
#include "GridMovement.h"
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

	// Creates an instance of Level 1.
	Level2::Level2() : Level("Level2"), columnsMap(8), rowsMap(5), startLives(3), lives(startLives), soundManager(nullptr), energizerPositions(), dotPositions()
	{
	}

	// Load the resources associated with Level 1.
	void Level2::Load()
	{
		//objectManager.AddArchetype(*objectFactory.CreateObject("KingGhost", resourceManager.GetMesh("Quad"), resourceManager.GetSpriteSource("Circle.png")));
	}

	// Initialize the memory associated with Level 1.
	void Level2::Initialize()
	{
		// KingGhost
		//GameObject* kingGhost = new GameObject(*objectManager.GetArchetypeByName("KingGhost"));
		//kingGhost->GetComponent<Behaviors::GridMovement>()->SetTilemap(dataMap, spriteTilemap);
		//kingGhost->GetComponent<Transform>()->SetTranslation(spriteTilemap->TileToWorld(Vector2D(13.5f, 11.0f)));
		//objectManager.AddObject(*kingGhost);
	}

	// Update Level 1.
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

	// Destroy objects associated with level 1.
	void Level2::Shutdown()
	{
	}

	// Unload the resources associated with Level 1.
	void Level2::Unload()
	{
	}

	//------------------------------------------------------------------------------
	// Private Functions:
	//------------------------------------------------------------------------------

	// Resets the energizer and dot position lists.
	void Level2::PopulateDots()
	{
	}
}

//----------------------------------------------------------------------------
