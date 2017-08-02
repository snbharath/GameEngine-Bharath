/*
* Game Engine - TickableMainThreadObject.cpp
*/

#include "TickableMainThreadObject.h"

/*
* @TODO - remove
* make sure you remove this include and implement your own one
*/
#include <algorithm>

using namespace GE;

std::list<TickableMainThreadObject*> TickableMainThreadObject::TickableMainThreadObjectsList;

//Constructor creates and adds the created object to the list for ticking
TickableMainThreadObject::TickableMainThreadObject()
{
	if (std::find(TickableMainThreadObjectsList.begin(), TickableMainThreadObjectsList.end(), this) == TickableMainThreadObjectsList.end())
	{
		TickableMainThreadObjectsList.push_back(this);
	}

	this->Init();
}

TickableMainThreadObject::~TickableMainThreadObject()
{
	this->Destroy();

	TickableMainThreadObjectsList.remove(this);
}

//Init function definition.. so far nothing
void TickableMainThreadObject::Init()
{
	// add anything here to initialize during construction
}

//Destroys an object 
void TickableMainThreadObject::Destroy()
{
	// code to deallocate if anything is allocated before
}


void TickableMainThreadObject::TickObjects(const double& DeltaTime)
{
	for (std::list<TickableMainThreadObject*>::const_iterator iterator = TickableMainThreadObjectsList.begin(); iterator != TickableMainThreadObjectsList.end(); ++iterator)
	{
		// Just iterate over all ticks in here so that it enables all the active objects for processing
		if ((*iterator)->IsTickable())
		{
			(*iterator)->Tick(DeltaTime);
		}
	}
}