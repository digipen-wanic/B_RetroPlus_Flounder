//------------------------------------------------------------------------------
//
// File Name:	BetaLowEngine.h
// Author(s):	Jeremy Kings (j.kings)
// Project:		BetaFramework
// Course:		WANIC VGP2 2018-2019
//
// Copyright © 2019 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

// Dependencies
#ifdef _M_IX86
	#ifdef _DEBUG
		#pragma comment(lib, "BetaLow_x86_D.lib")
	#else
		#pragma comment(lib, "BetaLow_x86.lib")
	#endif
#else
	#ifdef _DEBUG
		#pragma comment(lib, "BetaLow_x64_D.lib")
	#else
		#pragma comment(lib, "BetaLow_x64.lib")
	#endif
#endif

// Objects
#include <BetaObject.h>

// Systems
#include <Engine.h>
#include <FrameRateController.h>

//------------------------------------------------------------------------------
