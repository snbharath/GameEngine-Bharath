/*
* General platform independent input handling will be done here
* Game Engine GEInputComponent.cpp
*/

#include "GEInputComponent.h"
#include "GEInputKeyDefines.h"

using namespace GE;

const int GEInput::GetPressedKey(GEWindow* GeWindow, int presed)
{


#ifdef DIRECT3D
	
	
	// Define Direct X input handling stuff here


#elif OPENGL

	//return glfwGetKey(, GE_KEY_ESCAPE);

#else
	// placeholder for future if any may be Vulkan ??? 
#endif
	return 0;

}