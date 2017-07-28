#pragma once

/*
* Game Engine - Renderer.h
* Base Renderer class. This provides an abstruction to which renderer to use
*/
namespace GE
{
	/*
	* An abstruct class which gives the functionality for rendering stuff on to the screen.
	* Classes that are going to inherit this will have to implement the functonalities provided in this area.
	* All those classes that will inherit this class will be a singleton classes
	*/
	class GERenderer 
	{
	private:
		// placeholder for future variables

	protected:
		// Forces the Derived classes to be a singleton classes
		GERenderer() {};

	public:
		virtual void InitRenderer() = 0;

		virtual void CreateRenderer() = 0;

		virtual void CloseGEWindow() = 0;
	};
}