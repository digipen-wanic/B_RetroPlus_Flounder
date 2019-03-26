//------------------------------------------------------------------------------
//
// File Name:	ColliderRectangle.cpp
// Author(s):	David Cohen (david.cohen)
// Project:		BetaFramework
// Course:		WANIC VGP2
//
// Copyright � 2018 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "ColliderRectangle.h"

// Systems
#include <DebugDraw.h>
#include <Graphics.h>
#include "Intersection2D.h"
#include "GameObject.h"
#include "Parser.h"

// Components
#include "Transform.h"
#include "ColliderCircle.h"

//------------------------------------------------------------------------------

namespace
{
	// Helper function to keep a value within a range.
	// Params:
	//   value = The value to loop.
	//   min = The lower bounds of the value.
	//   max = The upper bounds of the value.
	// Returns:
	//   value, adjusted to be within the range of min-max.
	float Loop(float value, float min, float max)
	{
		while (value < min)
			value += (max - min);
		while (value > max)
			value -= (max - min);
		return value;
	}
}

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Constructor for rectangle collider.
// Params:
//   extents = The rectangle's extents (half-width, half-height).
ColliderRectangle::ColliderRectangle(const Vector2D& extents) : Collider(ColliderTypeRectangle), extents(extents)
{
}

// Clone an collider and return a pointer to the cloned object.
// Returns:
//   A pointer to a collider.
Component* ColliderRectangle::Clone() const
{
	return new ColliderRectangle(*this);
}

// Loads object data from a file.
// Params:
//   parser = The parser for the file.
void ColliderRectangle::Deserialize(Parser& parser)
{
	Collider::Deserialize(parser);

	parser.ReadVariable("extents", extents);
}

// Saves object data to a file.
// Params:
//   parser = The parser for the file.
void ColliderRectangle::Serialize(Parser& parser) const
{
	Collider::Serialize(parser);

	parser.WriteVariable("extents", extents);
}

// Debug drawing for colliders.
void ColliderRectangle::Draw()
{
	// Draw the rectangle using DebugDraw.
	DebugDraw& debugDraw = DebugDraw::GetInstance();

	if (AlmostEqual(Loop(transform->GetRotation(), -M_PI_F / 4.0f, M_PI_F / 4.0f), 0.0f))
	{
		Vector2D tempExtents = extents;

		// Check if angles are +/- 90 degrees instead of 0 or 180 degrees, and rotate them if so.
		if (!AlmostEqual(Loop(transform->GetRotation(), -M_PI_F / 2.0f, M_PI_F / 2.0f), 0.0f))
			std::swap(tempExtents.x, tempExtents.y);

		debugDraw.AddRectangle(transform->GetTranslation(), tempExtents, Graphics::GetInstance().GetCurrentCamera(), Colors::Green);
	}
	else
	{
		// Get the corners of the rectangle in object space.
		Vector2D points[4];
		GetOBBCorners(*this, points);

		// Transform the corners into world space.
		for (unsigned i = 0; i < 4; i++)
			points[i] = transform->GetMatrix() * points[i];

		// Draw the rectangle using the world space corners.
		debugDraw.AddLineToStrip(points[0], points[1], Colors::Green);
		debugDraw.AddLineToStrip(points[1], points[2], Colors::Green);
		debugDraw.AddLineToStrip(points[2], points[3], Colors::Green);
		debugDraw.AddLineToStrip(points[3], points[0], Colors::Green);
		debugDraw.EndLineStrip(Graphics::GetInstance().GetCurrentCamera());
	}
}

// Get the rectangle collider's extents (half-width, half-height).
// Returns:
//	 The extents of the rectangle.
const Vector2D& ColliderRectangle::GetExtents() const
{
	return extents;
}

// Set the rectangle collider's extents (half-width, half-height).
// Params:
//   extents = The new extents of the rectangle.
void ColliderRectangle::SetExtents(const Vector2D& extents_)
{
	extents = extents_;
}

// Check for collision between a rectangle and another arbitrary collider.
// Params:
//	 other = Reference to the second collider component.
// Returns:
//	 Return the results of the collision check.
bool ColliderRectangle::IsCollidingWith(const Collider& other) const
{
	// The translation of the other collider.
	Vector2D otherTranslation = static_cast<Transform*>(other.GetOwner()->GetComponent<Transform>())->GetTranslation();

	// The BoundingRectangle for this collider.
	BoundingRectangle rectangle = BoundingRectangle(transform->GetTranslation(), extents);

	// Normalize the angles to within -PI/2, +PI/2
	float angle1 = transform->GetRotation();

	// Calculate whether the angles are axis-aligned (multiples of 90 degrees)
	bool angle1AxisAligned = AlmostEqual(Loop(angle1, -M_PI_F / 4.0f, M_PI_F / 4.0f), 0.0f);

	// Check if angles are +/- 90 degrees instead of 0 or 180 degrees, and rotate them if so.
	if (!AlmostEqual(Loop(angle1, -M_PI_F / 2.0f, M_PI_F / 2.0f), 0.0f))
	{
		std::swap(rectangle.extents.x, rectangle.extents.y);
		rectangle.left = rectangle.center.x - rectangle.extents.x;
		rectangle.top = rectangle.center.y + rectangle.extents.y;
		rectangle.right = rectangle.center.x + rectangle.extents.x;
		rectangle.bottom = rectangle.center.y - rectangle.extents.y;

		angle1 = 0.0f;
	}

	switch (other.GetType())
	{
	case ColliderTypePoint:
	{
		if (angle1AxisAligned)
		{
			// Check if the point is intersecting the rectangle.
			return PointRectangleIntersection(otherTranslation, rectangle);
		}
		else
		{
			// Check if the point is intersecting the oriented bounding box.
			return OBBPointIntersection(*this, otherTranslation);
		}
	}
	case ColliderTypeCircle:
	{
		// Interpret the other collider as a circle collider for ease of access.
		const ColliderCircle& otherCircle = static_cast<const ColliderCircle&>(other);

		if (angle1AxisAligned)
		{
			// Check if the circle is intersecting the rectangle.
			return RectangleCircleIntersection(rectangle, Circle(otherTranslation, otherCircle.GetRadius()));
		}
		else
		{
			// Check if the circle is intersecting the oriented bounding box.
			return OBBCircleIntersection(*this, Circle(otherTranslation, otherCircle.GetRadius()));
		}
	}
	case ColliderTypeRectangle:
	{
		// Interpret the other collider as a rectangle collider for ease of access.
		const ColliderRectangle& otherRectangle = static_cast<const ColliderRectangle&>(other);

		// Normalize the angles to within -PI/2, +PI/2
		float angle2 = other.transform->GetRotation();

		// Calculate whether the angles are axis-aligned (multiples of 90 degrees)
		bool angle2AxisAligned = AlmostEqual(Loop(angle2, -M_PI_F / 4.0f, M_PI_F / 4.0f), 0.0f);

		if (angle1AxisAligned && angle2AxisAligned)
		{
			BoundingRectangle otherBoundingRectangle(otherTranslation, otherRectangle.GetExtents());

			// Check if angles are +/- 90 degrees instead of 0 or 180 degrees, and rotate them if so.
			if (!AlmostEqual(Loop(angle2, -M_PI_F / 2.0f, M_PI_F / 2.0f), 0.0f))
			{
				std::swap(otherBoundingRectangle.extents.x, otherBoundingRectangle.extents.y);
				otherBoundingRectangle.left = otherBoundingRectangle.center.x - otherBoundingRectangle.extents.x;
				otherBoundingRectangle.top = otherBoundingRectangle.center.y + otherBoundingRectangle.extents.y;
				otherBoundingRectangle.right = otherBoundingRectangle.center.x + otherBoundingRectangle.extents.x;
				otherBoundingRectangle.bottom = otherBoundingRectangle.center.y - otherBoundingRectangle.extents.y;

				angle2 = 0.0f;
			}

			// Check if the other rectangle is intersecting the rectangle.
			return RectangleRectangleIntersection(rectangle, otherBoundingRectangle);
		}
		else
		{
			// Check if the other oriented bounding box is intersecting the oriented bounding box.
			return OBBOBBIntersection(*this, otherRectangle);
		}
	}
	}

	return other.IsCollidingWith(*this);
}

//------------------------------------------------------------------------------
