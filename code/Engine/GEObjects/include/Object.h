#pragma once

/*
* This is the base class of the object of the Game engine
* Game engine - Object.h
*/
#include <string>


class Object
{
private:
	unsigned long int m_ID;

	static unsigned long int ObjectCounter;
public:

	Object()
	{
		ObjectCounter++;
		m_ID = ObjectCounter;
	}

	virtual ~Object() {}

	unsigned long int GetObjectId() { return m_ID; }
	
};

unsigned long int Object::ObjectCounter = 0;