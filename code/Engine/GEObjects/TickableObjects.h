/*
* Game Engine - TickablObjects.h file
* Tickable objects implementation of the game thread other objects
*/

#pragma once

#include <vector>
#include "Object.h"

namespace GE
{
	// tickable object base abstract class
	class TickableObjectBase : public Object
	{
	public:
		/*
		* This Tick/Update  funtion will be executed for every frame. This pure virtual function should be overridden by the inheriting class
		* It actually facilitates the functionality of the game loop
		* @param - DeltaTime Time since last call or time since last frame
		*/
		virtual void Tick(double DeltaTime) = 0;

		/*
		* This pure virtual function will be used to check if a game object is ticking or not
		*/
		virtual bool IsTickable() const = 0;
	};


	//Abstract class which will facilitate the ticking of the game object
	class TickableObject : public TickableObjectBase
	{
	private:
		/*
		* @TODO - come up with an efficient method to do the the "containt(obj)"
		* I'm going to have a list of all objects created. I know that this is in-efficient like for checking if an
		* element is already present or not. I'm going to be using find method to check that. I know that it takes O(n) for this 
		* operation to perform. I'm going to consider this operation until I implement my own methods to do that.
		*/
		static std::vector<TickableObject*> s_TickableObjectsList;

		static bool s_bIsTickingObjects;

	public:

		//Constructor
		TickableObject();

		//Destructor
		virtual ~TickableObject() ;

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
		static void TickObjects(double DeltaTime);


		static bool GetIsTickingObjects() { s_bIsTickingObjects; }

		static void PauseTickingOfAllobjects() { s_bIsTickingObjects = false; }

		static void ResumeTickingOfAllObjects() { s_bIsTickingObjects = true; }
	};
}