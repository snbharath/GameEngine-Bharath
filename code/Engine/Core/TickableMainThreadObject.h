/*
* Game Engine - TickableMainThreadObject.h
* This is a Tickable main thread object
* Potential use cases would be the main render thread, netowrking and game critical components
*/

#pragma once

#include <vector>
#include "Object.h"

namespace GE
{
	//Tickable main thread objects
	class TickableMainThreadObjectBase : public Object
	{
	public:

		/*
		* This Tick/Update  funtion will be executed for every frame. This pure virtual function should be overridden by the inheriting class
		* It actually facilitates the functionality of the game loop
		* @param - DeltaTime Time since last call or time since last frame
		*/
		virtual void Tick(const double& DeltaTime) = 0;

		/*
		* This pure virtual function will be used to check if a game object is ticking or not
		*/
		virtual bool IsTickable() const = 0;
	};

	class TickableMainThreadObject : public TickableMainThreadObjectBase
	{
	private:
		
		/*
		* @TODO - come up with an efficient method to do the the "containt(obj)"
		* I'm going to have a list of all objects created. I know that this is in-efficient like for checking if an
		* element is already present or not. I'm going to be using find method to check that. I know that it takes O(n) for this
		* operation to perform. I'm going to consider this operation until I implement my own methods to do that.
		*/
		static std::vector<TickableMainThreadObject*> s_TickableMainThreadObjectsList;

		static bool s_bIsTickingObjects;

	public:

		//Constructor
		TickableMainThreadObject();

		//Destructor
		virtual ~TickableMainThreadObject();

		//This can be considered as a constructor. It'll be executed for every objects that are being created
		virtual void Init();

		// This will be called when this object is being deleted or out of scope. This is for better memory efficiency. This can be overidden / not
		virtual void Destroy();

		//By default this object is always tickable unless any of the derived class overrides it.
		virtual bool IsTickable() const { return true; }

		/*
		* For now I'm gonna use this as a thread that ticks over all the active objects in the game. This implementation is subject
		* to changes to accomodate the efficient delta time changes
		*/
		static void TickObjects(const double& DeltaTime);


		static bool GetIsTickingObjects() { s_bIsTickingObjects; }

		static void PauseTickingOfAllobjects() { s_bIsTickingObjects = false; }

		static void ResumeTickingOfAllObjects() { s_bIsTickingObjects = true; }
	};
}