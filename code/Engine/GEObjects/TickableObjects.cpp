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

//Constructor creates and adds the created object to the list for ticking
TickableObject::TickableObject()
{
	if (std::find(TickableObjectList.begin(), TickableObjectList.end(), this) == TickableObjectList.end())
	{
		TickableObjectList.push_back(this);
	}

	this->Init();
}

TickableObject::~TickableObject()
{
	this->Destroy();

	TickableObjectList.remove(this);
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


void TickableObject::TickObjects(const double& DeltaTime)
{
	for (std::list<TickableObject*>::const_iterator iterator = TickableObjectList.begin(); iterator != TickableObjectList.end(); ++iterator)
	{
		// Just iterate over all ticks in here so that it enables all the active objects for processing
		if ((*iterator)->IsTickable())
		{
			(*iterator)->Tick(DeltaTime);
		}
	}
}