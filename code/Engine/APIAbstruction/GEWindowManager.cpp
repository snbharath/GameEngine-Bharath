/*
* class definition of GEWindowManager
* Game engine - GEWindowManager.cpp
*/

#include "GEWindowManager.h"
#include "OpenGLWindow.h"

#include <assert.h>

namespace GE
{
	GEWindow* WindowManager::GetWindowInstance()
	{
#ifdef DIRECT3D
		// create an instance Direct X Window
#elif OPENGL
		{
			auto pWindow = OpenGLWindow::GetInstance();
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
#else
		// placeholder for any other renderer support if I plan to support
#endif
	}

}
// namespace GE