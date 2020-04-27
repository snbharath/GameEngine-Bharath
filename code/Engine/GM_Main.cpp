#include<iostream>
#include "GEWindowManager.h"

#if defined(OPENGL)
#include "APIAbstruction/OpenGLWindow.h"
#elif defined(VULKAN)
#include "APIAbstruction/VulkanWindow.h"
#endif

using namespace std;
using namespace GE;

int main()
{
	GEWindow * pGameWindow = WindowManager::GetWindowInstance();
	if (pGameWindow == nullptr)
	{
		return -1;
	}

	while (pGameWindow->RenderFrameUpdate())
	{
		// do something after rendering the frame
	}

	return WindowManager::DeleteWindowInstance();
}
