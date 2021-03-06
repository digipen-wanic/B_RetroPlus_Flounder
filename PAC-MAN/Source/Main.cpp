//------------------------------------------------------------------------------
//
// File Name:	Main.cpp
// Author(s):	Jeremy Kings (j.kings)
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

// BetaFramework Engine
#include <Engine.h>

// Engine modules
#include <Space.h>
#include <SoundManager.h>

// Initial game state
#include "MainMenu.h"

// Game object factory
#include <GameObjectFactory.h>

// Components
#include "ColorChange.h"
#include "MonkeyAnimation.h"
#include "MonkeyMovement.h"
#include "PlayerShip.h"
#include "ScreenWrap.h"
#include "TimedDeath.h"
#include "PlayerProjectile.h"
#include "Asteroid.h"
#include "MouseFollow.h"
#include "PlayerController.h"
#include "PlayerScore.h"
#include "PlayerCollision.h"
#include "PlayerAnimation.h"
#include "BlinkyAI.h"
#include "InkyAI.h"
#include "PinkyAI.h"
#include "ClydeAI.h"
#include "GhostAnimation.h"
#include "PlayerGhostController.h"

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Main function
int WINAPI WinMain(_In_ HINSTANCE instance, _In_opt_ HINSTANCE prevInstance, _In_ LPSTR command_line, _In_ int show)
{
	// Enable memory leak checking
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	_CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_DEBUG);

	// Unused parameters
	UNREFERENCED_PARAMETER(prevInstance);
	UNREFERENCED_PARAMETER(command_line);
	UNREFERENCED_PARAMETER(show);
	UNREFERENCED_PARAMETER(instance);

	GameObjectFactory& objectFactory = GameObjectFactory::GetInstance();

	// Register components with the game object factory so game objects can be fully deserialized.
	objectFactory.RegisterComponent<Behaviors::ColorChange>();
	objectFactory.RegisterComponent<Behaviors::MonkeyAnimation>();
	objectFactory.RegisterComponent<Behaviors::MonkeyMovement>();
	objectFactory.RegisterComponent<Behaviors::PlayerShip>();
	objectFactory.RegisterComponent<Behaviors::ScreenWrap>();
	objectFactory.RegisterComponent<Behaviors::TimedDeath>();
	objectFactory.RegisterComponent<Behaviors::PlayerProjectile>();
	objectFactory.RegisterComponent<Behaviors::Asteroid>();
	objectFactory.RegisterComponent<Behaviors::MouseFollow>();
	objectFactory.RegisterComponent<Behaviors::PlayerController>();
	objectFactory.RegisterComponent<Behaviors::PlayerScore>();
	objectFactory.RegisterComponent<Behaviors::PlayerCollision>();
	objectFactory.RegisterComponent<Behaviors::PlayerAnimation>();
	objectFactory.RegisterComponent<Behaviors::BlinkyAI>();
	objectFactory.RegisterComponent<Behaviors::InkyAI>();
	objectFactory.RegisterComponent<Behaviors::PinkyAI>();
	objectFactory.RegisterComponent<Behaviors::ClydeAI>();
	objectFactory.RegisterComponent<Behaviors::GhostAnimation>();
	objectFactory.RegisterComponent<Behaviors::PlayerGhostController>();

	// Create a new space called "GameSpace"
	Space* space = new Space("GameSpace");

	// Set initial level to the main menu.
	space->SetLevel<Levels::MainMenu>();

	// Add additional modules to engine
	Engine::GetInstance().AddModule(space);
	SoundManager* soundManager = new SoundManager();
	soundManager->SetMusicVolume(0.25f);
	soundManager->SetEffectsVolume(0.25f);
	Engine::GetInstance().AddModule(soundManager);
	
	// Game engine goes!
	//Engine::GetInstance().Start(448, 576, 60);
	Engine::GetInstance().Start(672, 864, 60);

	return 0;
}
