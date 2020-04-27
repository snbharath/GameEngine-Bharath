#pragma once


/*
* This provides a way to create and handle which window to create. This makes the decision of platform based window creation.
* Game Engine - GameEngine.h
*/
#include "GEWindowManager.h"

namespace GE
{
	/*
	* This is also a singleton class which hold an instance of window creation and creates and manager the Application life time
	* We can have multiple instances of this class, since we are concerned about having just an instance singleton would do. 
	* However, We can extend this to have multiple instances later
	* @TODO : read above comments
	*/
	class GameEngine
	{
	private:

		static GameEngine* m_GameEngineInstance;
		GameEngine()
		{
			// initialize whatever you can
		}
	public:

		static GameEngine* Get();


		//This initializes and decides which rendering subsystem to use.
		void InitializeEngine();

		//This is the core of the engine and renders whatever is there in the background or presented to the engine.
		void StartRendering();
	};
}