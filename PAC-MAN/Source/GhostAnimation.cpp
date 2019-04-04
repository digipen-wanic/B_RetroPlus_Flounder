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
		currentState(StateSpawn), nextState(StateSpawn), transform(nullptr), animation(nullptr), baseAI(nullptr), ghostEatenQueued(false), deathQueued(false)
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
		animation->Play(moveUpStart, 1, 4.0f, false);
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
		parser.ReadVariable("eyesRightStart", eyesRightStart);
		parser.ReadVariable("eyesLeftStart", eyesLeftStart);
		parser.ReadVariable("eyesDownStart", eyesDownStart);
		parser.ReadVariable("eyesUpStart", eyesUpStart);
		parser.ReadVariable("frightenedStart", frightenedStart);
		parser.ReadVariable("frightenedLength", frightenedLength);
		parser.ReadVariable("frightenedEndStart", frightenedEndStart);
		parser.ReadVariable("frightenedEndLength", frightenedEndLength);
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
		parser.WriteVariable("eyesRightStart", eyesRightStart);
		parser.WriteVariable("eyesLeftStart", eyesLeftStart);
		parser.WriteVariable("eyesDownStart", eyesDownStart);
		parser.WriteVariable("eyesUpStart", eyesUpStart);
		parser.WriteVariable("frightenedStart", frightenedStart);
		parser.WriteVariable("frightenedLength", frightenedLength);
		parser.WriteVariable("frightenedEndStart", frightenedEndStart);
		parser.WriteVariable("frightenedEndLength", frightenedEndLength);
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

	// Called when the ghost is eaten, freezes the ghost.
	void GhostAnimation::OnGhostEaten()
	{
		ghostEatenQueued = true;
	}

	//------------------------------------------------------------------------------
	// Private Functions:
	//------------------------------------------------------------------------------

	// Choose the correct state based on velocity.
	void GhostAnimation::ChooseNextState()
	{
		// If the spawn animation is still playing, don't do anything.
		if (currentState == State::StateSpawn || currentState == State::StateGhostEaten)
		{
			if (!animation->IsDone())
			{
				baseAI->SetFrozen(true);
				baseAI->direction = GridMovement::Direction::UP;
				return;
			}
		}

		// If the ghost eaten animation should be playing, play it.
		if (ghostEatenQueued)
		{
			ghostEatenQueued = false;
			if (!(currentState == State::StateEyesUp || currentState == State::StateEyesLeft || currentState == State::StateEyesDown || currentState == State::StateEyesRight))
				nextState = State::StateGhostEaten;
			return;
		}

		if (baseAI->IsDead())
		{
			switch (baseAI->direction)
			{
			case GridMovement::Direction::UP:
				nextState = State::StateEyesUp;
				break;
			case GridMovement::Direction::LEFT:
				nextState = State::StateEyesLeft;
				break;
			case GridMovement::Direction::DOWN:
				nextState = State::StateEyesDown;
				break;
			case GridMovement::Direction::RIGHT:
				nextState = State::StateEyesRight;
				break;
			}
		}
		else if (baseAI->IsFrightened())
		{
			if (baseAI->frightTimer <= baseAI->frightenTime - 4.0f)
				nextState = State::StateFrightenedEnd;
			else
				nextState = State::StateFrightened;
		}
		else
		{
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
				animation->Play(moveUpStart, 1, 4.0f, false);
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
				// If the state is changed to the eyes right state, begin playing the eyes right animation.
			case State::StateEyesRight:
				animation->Play(eyesRightStart, 1, 0.1f, true);
				break;
				// If the state is changed to the eyes left state, begin playing the eyes left animation.
			case State::StateEyesLeft:
				animation->Play(eyesLeftStart, 1, 0.1f, true);
				break;
				// If the state is changed to the eyes down state, begin playing the eyes down animation.
			case State::StateEyesDown:
				animation->Play(eyesDownStart, 1, 0.1f, true);
				break;
				// If the state is changed to the eyes up state, begin playing the eyes up animation.
			case State::StateEyesUp:
				animation->Play(eyesUpStart, 1, 0.1f, true);
				break;
				// If the state is changed to the frightened state, begin playing the frightened animation.
			case State::StateFrightened:
				animation->Play(frightenedStart, frightenedLength, 0.1f, true);
				break;
				// If the state is changed to the frightened end state, begin playing the frightened end animation.
			case State::StateFrightenedEnd:
				animation->Play(frightenedEndStart, frightenedEndLength, 0.1f, true);
				break;
				// When a ghost is eaten, freeze the current frame.
			case State::StateGhostEaten:
				animation->Play(animation->GetCurrentFrame(), 1, 0.5f, false);
				break;
			}
		}
	}
}

//------------------------------------------------------------------------------
