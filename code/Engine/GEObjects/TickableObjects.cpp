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
	if (std::find(TickableObjects.begin(), TickableObjects.end(), this) == TickableObjects.end())
	{
		TickableObjects.push_back(this);
	}
}


TickableObject::~TickableObject()
{
	TickableObjects.remove(this);
}