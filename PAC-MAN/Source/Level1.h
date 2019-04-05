//------------------------------------------------------------------------------
//
// File Name:	Level1.h
// Author(s):	David Cohen (david.cohen)
// Project:		BetaFramework
// Course:		WANIC VGP2 2018-2019
//
// Copyright © 2018 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

#include <Level.h>

#include <vector>
#include <Vector2D.h>

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Forward References:
//------------------------------------------------------------------------------

class Texture;
class SpriteSource;
class Mesh;
class SpriteSource;
class GameObject;
class Tilemap;
class SoundManager;

namespace FMOD
{
	class Channel;
}

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

namespace Levels
{
	class Level1 : public Level
	{
	public:
		//------------------------------------------------------------------------------
		// Public Functions:
		//------------------------------------------------------------------------------

		// Creates an instance of Level 1.
		Level1();

		// Load the resources associated with Level 1.
		void Load() override;

		// Initialize the memory associated with Level 1.
		void Initialize() override;

		// Update Level 1.
		// Params:
		//	 dt = Change in time (in seconds) since the last game loop.
		void Update(float dt) override;

		// Destroy objects associated with level 1.
		void Shutdown() override;

		// Unload the resources associated with Level 1.
		void Unload() override;

	private:
		//------------------------------------------------------------------------------
		// Private Functions:
		//------------------------------------------------------------------------------

		// Resets the energizer and dot position lists.
		void PopulateDots();

		//------------------------------------------------------------------------------
		// Private Variables:
		//------------------------------------------------------------------------------

		// Tilemap
		Tilemap* dataMap;
		unsigned columnsMap;
		unsigned rowsMap;

		// Energizer
		unsigned columnsEnergizer;
		unsigned rowsEnergizer;

		// PAC-MAN
		unsigned columnsPacMan;
		unsigned rowsPacMan;

		// Ghosts
		unsigned columnsGhost;
		unsigned rowsGhost;

		// Other variables.

		// Misc timers
		float readyTimer;
		bool startedAmbience;
		float winTimer;

		// Variables to persist between restarts
		bool gameOver;
		unsigned startLives;
		unsigned lives;
		unsigned oldScore;
		unsigned oldDots;
		unsigned highScore;
		unsigned blinkyWave;
		float blinkyWaveTimer;
		unsigned pinkyWave;
		float pinkyWaveTimer;
		unsigned inkyWave;
		float inkyWaveTimer;
		unsigned clydeWave;
		float clydeWaveTimer;

		// Fruit spawn timers
		int fruitSpawnAmount;
		float fruitDeathTimer;
		bool fruitAlive;

		// Sound manager
		SoundManager* soundManager;
		FMOD::Channel* ambience;

		// Persistent locations of energizers & dots
		std::vector<Vector2D> energizerPositions;
		std::vector<Vector2D> dotPositions;

		// Game objects
		GameObject* readyText;
		GameObject* scoreText;
		GameObject* highScoreText;
		GameObject* pacMan;
		GameObject* fruit;
		GameObject* lifeIcons[2];
	};
}

//----------------------------------------------------------------------------
