//------------------------------------------------------------------------------
//
// File Name:	Level2.h
// Author(s):	David Cohen (david.cohen)
// Project:		BetaFramework
// Course:		WANIC VGP2 2018-2019
//
// Copyright � 2018 DigiPen (USA) Corporation.
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

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

namespace Levels
{
	class Level2 : public Level
	{
	public:
		//------------------------------------------------------------------------------
		// Public Functions:
		//------------------------------------------------------------------------------

		// Creates an instance of Level 2.
		Level2();

		// Load the resources associated with Level 2.
		void Load() override;

		// Initialize the memory associated with Level 2.
		void Initialize() override;

		// Update Level 2.
		// Params:
		//	 dt = Change in time (in seconds) since the last game loop.
		void Update(float dt) override;

		// Destroy objects associated with level 2.
		void Shutdown() override;

		// Unload the resources associated with Level 2.
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
		unsigned startLives;
		unsigned lives;
		unsigned oldScore;
		unsigned oldDots;
		SoundManager* soundManager;
		std::vector<Vector2D> energizerPositions;
		std::vector<Vector2D> dotPositions;
	};
}

//----------------------------------------------------------------------------
