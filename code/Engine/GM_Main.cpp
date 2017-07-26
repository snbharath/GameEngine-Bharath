#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <iostream>
#include <thread>

#include "GEWindowManager.h"

using namespace std;
using namespace GE;

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}


int main()
{
	//std::thread t1(processInput);

	
	
	//Render loop
	while (!glfwWindowShouldClose(    reinterpret_cast<OpenGLWindow*>(GEWindowManager::Get()->GetWindowInstance())->GetGLFWCreatedWindowInstance()   ))
	{
		//handle input
		processInput(reinterpret_cast<OpenGLWindow*>(GEWindowManager::Get()->GetWindowInstance())->GetGLFWCreatedWindowInstance());

		//render come stuff like color
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//glfw swap buffers and poll IO events(pressed, released, mouse moved etc...)
		glfwSwapBuffers(reinterpret_cast<OpenGLWindow*>(GEWindowManager::Get()->GetWindowInstance())->GetGLFWCreatedWindowInstance());
		glfwPollEvents();
	}

	//terminate all previouslt allocated resources
	
	return 0;
}