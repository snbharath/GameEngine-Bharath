#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <iostream>

using namespace std;

//Whenever the window is resized this function will get called
void FrameBufferSizeCallBack(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}


void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}


int main()
{
	
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, FrameBufferSizeCallBack);
	//Load all OGL function pointers
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		cout << "Falied to initialize GLAD" << endl;
		return -1;
	}


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
	glfwTerminate();
	return 0;
}