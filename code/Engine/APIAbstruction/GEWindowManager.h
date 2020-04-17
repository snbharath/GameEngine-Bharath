#pragma once

/*
* This is a manager of the window creation. Will take decision of which window to create based on the solution configuration
* Game engine - GEWindowManager.h
*/

namespace GE
{
	// forward declaration
	class GEWindow;

	//@TODO - inherit the base Object class here
	class WindowManager // gonna be inheriting the base Object class later I implement the Object.h
	{
	public:

		static GEWindow * GetWindowInstance();
	};
}