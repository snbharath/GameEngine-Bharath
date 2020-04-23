/*
* class definition of GEWindowManager
* Game engine - GEWindowManager.cpp
*/

#include "GEWindowManager.h"

#if defined (OPENGL)
#include "OpenGLWindow.h"
#elif defined (VULKAN)
#include "VulkanWindow.h"
#endif
#include <assert.h>

namespace GE
{
	GEWindow* WindowManager::GetWindowInstance()
	{
#if defined(DIRECT3D)
		// create an instance Direct X Window
#elif defined (OPENGL)
		auto pWindow = OpenGLWindow::GetInstance();
#elif defined (VULKAN)
		auto pWindow = VulkanWindow::GetInstance();
#else
		// placeholder for any other renderer support if I plan to support
#endif

		if (pWindow == nullptr)
		{
			return nullptr;
		}
		bool isSuccess = pWindow->InitWindow();
		isSuccess &= pWindow->CreateGEWindow();
		if (!isSuccess)
		{
			return nullptr;
		}

		return pWindow;
	}
}
// namespace GE