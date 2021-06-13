/*
* class definition of GEWindowManager
* Game engine - GEWindowManager.cpp
*/

#include "GEWindowManager.h"

#include "GEApiAbstructionList.h"

#include <assert.h>

namespace GE
{
	GEWindow* WindowManager::GetWindowInstance()
	{
		auto pWindow = CWindow::GetInstance();

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

	bool WindowManager::DeleteWindowInstance()
	{
		bool result = false;
		result = CWindow::DeleteInstance();

		return result;
	}
}
// namespace GE