#include<iostream>
#include "GEWindowManager.h"

#include "APIAbstruction/GEApiAbstructionList.h"

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
