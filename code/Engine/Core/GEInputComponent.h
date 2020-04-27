/*
* provides a layer of abstraction for input handling across multiple platforms
* Game Engine - GEInputManager.h
*/

#pragma once

namespace GE
{
	//Forward declare
	class GEWindow;

	class GEInput
	{
	public: 
		
		//this function is used to check if a key is pressed or not! 
		static const int GetPressedKey(GEWindow*, int presed);
	};
}