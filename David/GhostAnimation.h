//------------------------------------------------------------------------------
//
// File Name:	GhostAnimation.h
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

#include <Vector2D.h> // Vector2D

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Forward Declarations:
//------------------------------------------------------------------------------

class Transform;
class Animation;

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

namespace Behaviors
{
	class BaseAI;

	class GhostAnimation : public Component
	{
	public:
		//------------------------------------------------------------------------------
		// Public Functions:
		//------------------------------------------------------------------------------

		// Default constructor
		GhostAnimation();

		// Clone a component and return a pointer to the cloned component.
		// Returns:
		//   A pointer to a dynamically allocated clone of the component.
		Component* Clone() const override;

		// Initialize this component (happens at object creation).
		void Initialize() override;

		// Loads object data from a file.
		// Params:
		//   parser = The parser for the file.
		virtual void Deserialize(Parser& parser) override;

		// Saves object data to a file.
		// Params:
		//   parser = The parser for the file.
		virtual void Serialize(Parser& parser) const override;

		// Fixed update function for this component.
		// Params:
		//   dt = The (fixed) change in time since the last step.
		void Update(float dt) override;

		// Freezes the animation on a blank frame.
		// Params:
		//   time = How long the animation should stay frozen.
		void FreezeBlank(float time);

		// Freezes the animation on the current frame.
		// Params:
		//   time = How long the animation should stay frozen.
		void FreezeCurrent(float time);

	private:
		//------------------------------------------------------------------------------
		// Private Functions:
		//------------------------------------------------------------------------------

		// Choose the correct state based on velocity.
		void ChooseNextState();

		// Change states and start the appropriate animation.
		void ChangeCurrentState();

		//------------------------------------------------------------------------------
		// Private Structures:
		//------------------------------------------------------------------------------

		enum State
		{
			StateSpawn = 0,
			StateMoveRight,
			StateMoveLeft,
			StateMoveDown,
			StateMoveUp,
			StateEyesRight,
			StateEyesLeft,
			StateEyesDown,
			StateEyesUp,
			StateFrightened,
			StateFrightenedEnd,
			StateFrozen,

			STATE_MAX
		};

		//------------------------------------------------------------------------------
		// Private Variables:
		//------------------------------------------------------------------------------

		// Animation variables
		unsigned moveRightStart;
		unsigned moveLeftStart;
		unsigned moveDownStart;
		unsigned moveUpStart;
		unsigned moveLength;
		unsigned eyesRightStart;
		unsigned eyesLeftStart;
		unsigned eyesDownStart;
		unsigned eyesUpStart;
		unsigned frightenedStart;
		unsigned frightenedLength;
		unsigned frightenedEndStart;
		unsigned frightenedEndLength;
		unsigned blankStart;

		// Animation state
		State currentState;
		State nextState;

		// Components
		Transform* transform;
		Animation* animation;
		BaseAI* baseAI;

		// Other variables.
		bool deathQueued;
		bool frozenQueued;
		bool frozenBlank;
		float frozenTime;

		friend class BaseAI;
	};
}

//------------------------------------------------------------------------------
