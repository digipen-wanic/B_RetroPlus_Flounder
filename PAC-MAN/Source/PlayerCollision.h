//------------------------------------------------------------------------------
//
// File Name:	PlayerCollision.h
// Author(s):	David Cohen (david.cohen)
// Project:		PAC-MAN
// Course:		WANIC VGP2 2018-2019
//
// Copyright © 2018 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

#include "Component.h" // base class

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Forward Declarations:
//------------------------------------------------------------------------------

class Transform;
class Tilemap;
class SpriteTilemap;

namespace Behaviors
{
	class PlayerScore;
}

namespace FMOD
{
	class Channel;
}

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

namespace Behaviors
{
	class PlayerCollision : public Component
	{
	public:
		//------------------------------------------------------------------------------
		// Public Functions:
		//------------------------------------------------------------------------------

		// Constructor
		PlayerCollision();

		// Initialize this component (happens at object creation).
		void Initialize() override;

		// Clone a component and return a pointer to the cloned component.
		// Returns:
		//   A pointer to a dynamically allocated clone of the component.
		Component* Clone() const override;

		// Updates components using a fixed timestep (usually just physics)
		// Params:
		//	 dt = A fixed change in time, usually 1/60th of a second.
		void FixedUpdate(float dt) override;

		// Sets the tilemap used for the grid.
		// Params:
		//   tilemap = The tilemap.
		//   spriteTilemap = The sprite tilemap.
		void SetTilemap(Tilemap* tilemap, SpriteTilemap* spriteTilemap);

		// Called when the player dies.
		void OnDeath();

	private:
		//------------------------------------------------------------------------------
		// Private Variables:
		//------------------------------------------------------------------------------

		// The tilemap used for the grid.
		Tilemap* tilemap;
		SpriteTilemap* spriteTilemap;

		// Components
		Transform* transform;
		PlayerScore* playerScore;

		// Other variables
		int ghostStreak;
		bool oddConsumable;
		FMOD::Channel* energizerSound;
		FMOD::Channel* ghostDeathSound;
	};
}

//------------------------------------------------------------------------------
