#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <iostream>
#include <thread>

using namespace std;

void processInput()//GLFWwindow* window)
{
	/*if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);*/
}


int main()
{
	std::thread t1(processInput);

	//Render loop
	while (!glfwWindowShouldClose(window))
	{
		//handle input
		processInput(window);

		//render come stuff like color
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//glfw swap buffers and poll IO events(pressed, released, mouse moved etc...)
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	//terminate all previouslt allocated resources
	
	return 0;
}