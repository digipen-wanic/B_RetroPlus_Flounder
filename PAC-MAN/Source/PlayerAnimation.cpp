//------------------------------------------------------------------------------
//
// File Name:	PlayerAnimation.cpp
// Author(s):	David Cohen (david.cohen)
// Project:		PAC-MAN
// Course:		WANIC VGP2 2018-2019
//
// Copyright � 2018 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "PlayerAnimation.h"

// Systems
#include <GameObject.h>
#include <Space.h>
#include <Parser.h>

// Components
#include <Animation.h>
#include <Physics.h>
#include <Transform.h>
#include "PlayerController.h"

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

namespace Behaviors
{
	//------------------------------------------------------------------------------
	// Public Functions:
	//------------------------------------------------------------------------------

	// Default constructor
	PlayerAnimation::PlayerAnimation() : Component("PlayerAnimation"),
		spawnStart(0), idleStart(0), moveStart(0), moveLength(0), deathStart(0), deathLength(0),
		currentState(StateSpawn), nextState(StateSpawn), animation(nullptr), playerController(nullptr), deathQueued(false)
	{
	}

	// Clone a component and return a pointer to the cloned component.
	// Returns:
	//   A pointer to a dynamically allocated clone of the component.
	Component* PlayerAnimation::Clone() const
	{
		return new PlayerAnimation(*this);
	}

	// Initialize this component (happens at object creation).
	void PlayerAnimation::Initialize()
	{
		// Store the required components for ease of access.
		animation = GetOwner()->GetComponent<Animation>();
		playerController = GetOwner()->GetComponent<PlayerController>();

		// Play the spawn animation.
		animation->Play(spawnStart, 1, 1.0f, false);
	}

	// Loads object data from a file.
	// Params:
	//   parser = The parser for the file.
	void PlayerAnimation::Deserialize(Parser& parser)
	{
		parser.ReadVariable("spawnStart", spawnStart);
		parser.ReadVariable("idleStart", idleStart);
		parser.ReadVariable("moveStart", moveStart);
		parser.ReadVariable("moveLength", moveLength);
		parser.ReadVariable("deathStart", deathStart);
		parser.ReadVariable("deathLength", deathLength);
	}

	// Saves object data to a file.
	// Params:
	//   parser = The parser for the file.
	void PlayerAnimation::Serialize(Parser& parser) const
	{
		parser.WriteVariable("spawnStart", spawnStart);
		parser.WriteVariable("idleStart", idleStart);
		parser.WriteVariable("moveStart", moveStart);
		parser.WriteVariable("moveLength", moveLength);
		parser.WriteVariable("deathStart", deathStart);
		parser.WriteVariable("deathLength", deathLength);
	}

	// Fixed update function for this component.
	// Params:
	//   dt = The (fixed) change in time since the last step.
	void PlayerAnimation::Update(float dt)
	{
		UNREFERENCED_PARAMETER(dt);

		// Choose the next state.
		ChooseNextState();

		// Update the current state if necessary.
		ChangeCurrentState();
	}

	// Called when the player dies.
	void PlayerAnimation::OnDeath()
	{
		deathQueued = true;
	}

	// Returns whether the death animation is currently playing.
	bool PlayerAnimation::IsDying() const
	{
		return currentState == State::StateDeath;
	}

	//------------------------------------------------------------------------------
	// Private Functions:
	//------------------------------------------------------------------------------

	// Choose the correct state based on velocity.
	void PlayerAnimation::ChooseNextState()
	{
		// If the spawn animation is still playing, don't do anything.
		if (currentState == State::StateSpawn && !animation->IsDone())
			return;

		// If the death animation should be playing, play it.
		if (deathQueued)
		{
			nextState = State::StateDeath;
			deathQueued = false;
			return;
		}

		// If the death animation is playing, don't do anything.
		if (currentState == State::StateDeath)
		{
			if (animation->IsDone())
			{
				// Restart level when animation is done.
				GetOwner()->GetSpace()->RestartLevel();
			}
			return;
		}

		if (AlmostEqual(playerController->GetOldTile(), playerController->GetNewTile()))
		{
			nextState = State::StateIdle;
		}
		else
		{
			nextState = State::StateMove;
		}
	}

	// Change states and start the appropriate animation.
	void PlayerAnimation::ChangeCurrentState()
	{
		// Check if the state should change.
		if (currentState != nextState)
		{
			currentState = nextState;
			switch (currentState)
			{
				// If the state is changed to the spawn state, begin playing the spawn animation.
			case State::StateSpawn:
				animation->Play(spawnStart, 1, 1.0f, false);
				break;
				// If the state is changed to the idle state, begin playing the idle animation.
			case State::StateIdle:
				animation->Play(idleStart, 1, 0.0f, true);
				break;
				// If the state is changed to the moving state, begin playing the moving animation.
			case State::StateMove:
				animation->Play(moveStart, moveLength, 0.25f, true);
				break;
				// If the state is changed to the dying state, begin playing the dying animation.
			case State::StateDeath:
				animation->Play(deathStart, deathLength, 0.25f, false);
				break;
			}
		}
	}
}

//------------------------------------------------------------------------------