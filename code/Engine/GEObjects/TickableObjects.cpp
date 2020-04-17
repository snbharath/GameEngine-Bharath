/*
* TickableObjects.cpp
*/

#include "TickableObjects.h"

/*
* @TODO - remove
* make sure you remove this include and implement your own one
*/
#include <algorithm>

using namespace GE;

std::vector<TickableObject*> TickableObject::s_TickableObjectsList;

//Constructor creates and adds the created object to the list for ticking
TickableObject::TickableObject()
{
	if (std::find(s_TickableObjectsList.begin(), s_TickableObjectsList.end(), this) == s_TickableObjectsList.end())
	{
		s_TickableObjectsList.push_back(this);
	}

	Init();
}

TickableObject::~TickableObject()
{
	Destroy();
	//s_TickableObjectsList.remove(this);
}

//Init function definition.. so far nothing
void TickableObject::Init()
{
	// add anything here to initialize during construction
}

//Destroys an object 
void TickableObject::Destroy()
{
	// code to deallocate if anything is allocated before
}


void TickableObject::TickObjects(double DeltaTime)
{
	for (std::vector<TickableObject*>::const_iterator iterator = s_TickableObjectsList.begin(); iterator != s_TickableObjectsList.end(); ++iterator)
	{
		// Just iterate over all ticks in here so that it enables all the active objects for processing
		if ((*iterator)->IsTickable())
		{
			(*iterator)->Tick(DeltaTime);
		}
	}
}