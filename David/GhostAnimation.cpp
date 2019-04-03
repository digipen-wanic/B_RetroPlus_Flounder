//------------------------------------------------------------------------------
//
// File Name:	GhostAnimation.cpp
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

#include "GhostAnimation.h"

// Systems
#include <GameObject.h>
#include <Space.h>
#include <Parser.h>

// Components
#include <Animation.h>
#include <Physics.h>
#include <Transform.h>
#include "BaseAI.h"

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
	GhostAnimation::GhostAnimation() : Component("GhostAnimation"),
		moveRightStart(0), moveLeftStart(0), moveDownStart(0), moveUpStart(0), moveLength(0),
		currentState(StateSpawn), nextState(StateSpawn), transform(nullptr), animation(nullptr), baseAI(nullptr), deathQueued(false)
	{
	}

	// Clone a component and return a pointer to the cloned component.
	// Returns:
	//   A pointer to a dynamically allocated clone of the component.
	Component* GhostAnimation::Clone() const
	{
		return new GhostAnimation(*this);
	}

	// Initialize this component (happens at object creation).
	void GhostAnimation::Initialize()
	{
		// Store the required components for ease of access.
		transform = GetOwner()->GetComponent<Transform>();
		animation = GetOwner()->GetComponent<Animation>();
		baseAI = GetOwner()->GetComponent<BaseAI>();

		// Play the spawn animation.
		animation->Play(moveUpStart, 1, 1.0f, false);
	}

	// Loads object data from a file.
	// Params:
	//   parser = The parser for the file.
	void GhostAnimation::Deserialize(Parser& parser)
	{
		parser.ReadVariable("moveRightStart", moveRightStart);
		parser.ReadVariable("moveLeftStart", moveLeftStart);
		parser.ReadVariable("moveDownStart", moveDownStart);
		parser.ReadVariable("moveUpStart", moveUpStart);
		parser.ReadVariable("moveLength", moveLength);
	}

	// Saves object data to a file.
	// Params:
	//   parser = The parser for the file.
	void GhostAnimation::Serialize(Parser& parser) const
	{
		parser.WriteVariable("moveRightStart", moveRightStart);
		parser.WriteVariable("moveLeftStart", moveLeftStart);
		parser.WriteVariable("moveDownStart", moveDownStart);
		parser.WriteVariable("moveUpStart", moveUpStart);
		parser.WriteVariable("moveLength", moveLength);
	}

	// Fixed update function for this component.
	// Params:
	//   dt = The (fixed) change in time since the last step.
	void GhostAnimation::Update(float dt)
	{
		UNREFERENCED_PARAMETER(dt);

		// Choose the next state.
		ChooseNextState();

		// Update the current state if necessary.
		ChangeCurrentState();
	}

	//------------------------------------------------------------------------------
	// Private Functions:
	//------------------------------------------------------------------------------

	// Choose the correct state based on velocity.
	void GhostAnimation::ChooseNextState()
	{
		// If the spawn animation is still playing, don't do anything.
		if (currentState == State::StateSpawn)
		{
			if (animation->IsDone())
			{
				//baseAI->SetFrozen(false);
			}
			else
			{
				baseAI->SetFrozen(true);
				baseAI->direction = GridMovement::Direction::UP;
				return;
			}
		}

		switch (baseAI->direction)
		{
		case GridMovement::Direction::UP:
			nextState = State::StateMoveUp;
			break;
		case GridMovement::Direction::LEFT:
			nextState = State::StateMoveLeft;
			break;
		case GridMovement::Direction::DOWN:
			nextState = State::StateMoveDown;
			break;
		case GridMovement::Direction::RIGHT:
			nextState = State::StateMoveRight;
			break;
		}
	}

	// Change states and start the appropriate animation.
	void GhostAnimation::ChangeCurrentState()
	{
		// Check if the state should change.
		if (currentState != nextState)
		{
			currentState = nextState;
			switch (currentState)
			{
				// If the state is changed to the spawn state, begin playing the spawn animation.
			case State::StateSpawn:
				animation->Play(moveUpStart, 1, 1.0f, false);
				break;
				// If the state is changed to the moving right state, begin playing the moving right animation.
			case State::StateMoveRight:
				animation->Play(moveRightStart, moveLength, 0.1f, true);
				break;
				// If the state is changed to the moving left state, begin playing the moving left animation.
			case State::StateMoveLeft:
				animation->Play(moveLeftStart, moveLength, 0.1f, true);
				break;
				// If the state is changed to the moving down state, begin playing the moving down animation.
			case State::StateMoveDown:
				animation->Play(moveDownStart, moveLength, 0.1f, true);
				break;
				// If the state is changed to the moving up state, begin playing the moving up animation.
			case State::StateMoveUp:
				animation->Play(moveUpStart, moveLength, 0.1f, true);
				break;
			}
		}
	}
}

//------------------------------------------------------------------------------
