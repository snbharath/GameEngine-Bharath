#pragma once

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

//its a singleton Game Engine window class
namespace GE
{
	//abstract class definition for Render window creation
	class GEWindow
	{
	private:

	public:

		/*
		* Initialize the Game engine window to start the rendering of frames
		*/
		virtual int InitWindow() = 0;

		/*
		* Create a Rendering window here
		* This creates a render window of Either OpenGL or DirectX
		*/
		virtual int CreateGEWindow() = 0;

		/*
		* close and terminate the Game Engine window
		*/
		virtual void CloseGEWindow() = 0;

	};
}