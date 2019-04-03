//------------------------------------------------------------------------------
//
// File Name:	SpriteText.h
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

#include "Sprite.h"

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

// You are free to change the contents of this structure as long as you do not
//   change the public interface declared in the header.
class SpriteText : public Sprite
{
public:
	//------------------------------------------------------------------------------
	// Public Structures:
	//------------------------------------------------------------------------------

	enum class Alignment
	{
		CENTER = 0,
		TOP = 1,
		RIGHT = 2,
		BOTTOM = 3,
		LEFT = 4
	};

	//------------------------------------------------------------------------------
	// Public Functions:
	//------------------------------------------------------------------------------

	// Default constructor.
	SpriteText();

	// Constructor.
	// Params:
	//   text = The text to display.
	SpriteText(const char* text);

	// Clone the sprite, returning a dynamically allocated copy.
	Component* Clone() const override;

	// Draw a sprite (Sprite can be textured or untextured).
	void Draw() override;

	// Write object data to file
	// Params:
	//   parser = The parser that is writing this object to a file.
	void Serialize(Parser& parser) const override;

	// Read object data from a file
	// Params:
	//   parser = The parser that is reading this object's data from a file.
	void Deserialize(Parser& parser) override;

	// Sets the string to display.
	// Params:
	//   text = New string to display.
	void SetText(const char* text);

	// Get the current string being desplayed.
	const std::string& GetText() const;

	// Gets the current horizontal alignment.
	Alignment GetHorizontalAlignment() const;

	// Sets the current horizontal alignment.
	void SetHorizontalAlignment(Alignment alignment);

	// Gets the current vertical alignment.
	Alignment GetVerticalAlignment() const;

	// Sets the current vertical alignment.
	void SetVerticalAlignment(Alignment alignment);

	// Calculates the width of the current text when displayed.
	// Returns:
	//   The width of the current text.
	float GetWidth() const;

	// Calculates the height of the current text when displayed.
	// Returns:
	//   The height of the current text.
	float GetHeight() const;

private:
	//------------------------------------------------------------------------------
	// Private Variables:
	//------------------------------------------------------------------------------

	// The text this component should draw.
	std::string text;

	// Properties (save to/load from file)

	// The alignment of the text (where the origin of the transform is).
	Alignment horizontalAlignment;
	Alignment verticalAlignment;
};

//------------------------------------------------------------------------------
