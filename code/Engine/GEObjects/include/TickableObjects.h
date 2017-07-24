/*
* TickablObjects.h file
*/

#pragma once

#include <list>


namespace GE
{
	// tickable object base abstract class
	class TickableObjectBase
	{
		/*
		* This Tick/Update  funtion will be executed for every frame. This pure virtual function should be overridden by the inheriting class
		* It actually facilitates the functionality of the game loop
		* @param - DeltaTime Time since last call or time since last frame
		*/
		virtual void Tick(float DeltaTime) = 0;

		/*
		* This pure virtual function will be used to check if a game object is ticking or not
		*/
		virtual bool IsTickable() const = 0;
	};


	//Abstruct class which will facilitate the ticking of the game object
	class TickableObject : public TickableObjectBase
	{
	private:
		/*
		* @TODO - come up with an efficient method to do the the "containt(obj)"
		* I'm going to have a list of all objects created. I know that this is in-efficient like for checking if an
		* element is already present or not. I'm going to be using find method to check that. I know that it takes O(n) for this 
		* operation to perform. I'm going to consider this operation until I implement my own methods to do that.
		*/
		static std::list<TickableObject*> TickableObjects;

		static bool b_IsTickingObjects;

	public:

		TickableObject();

		virtual ~TickableObject();

		static void TickObjects();

		static bool GetIsTickingObjects() { b_IsTickingObjects; }

		static void PauseTickingOfAllobjects() { b_IsTickingObjects = false; }

		static void REsumeTickingOfAllObjects() { b_IsTickingObjects = true; }
	};
}