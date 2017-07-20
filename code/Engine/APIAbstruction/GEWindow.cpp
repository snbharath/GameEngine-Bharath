#include "GEWindow.h"

using namespace GE;

void GEWindow::InitWindow()
{

#ifdef DIRECT3D
	// Direct 3D initialization goes here

#elif OPENGL
	// GLFW initialization
	glfwInit();

	//Choose GLFW Major and Minor VErsion
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);  // this is needed for the Max OS X
#endif
}


int GEWindow::CreateGEWindow()
{
#ifdef DIRECT3D 
	//Direct 3D Window creation goes here.
#elif OPENGL
	m_window = glfwCreateWindow(800, 600, "GameEngine - by Bharath", NULL, NULL);

	if (m_window == NULL)
	{
		return -1;
	}
	return 0;
#endif
}