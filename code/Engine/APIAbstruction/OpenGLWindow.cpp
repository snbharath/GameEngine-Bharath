#ifdef OPENGL

#include "OpenGLWindow.h"


using namespace GE;

OpenGLWindow* OpenGLWindow::s_pInstance = nullptr;

OpenGLWindow::OpenGLWindow()
	:m_pCreatedWindowInstance(nullptr)
{
}

OpenGLWindow::~OpenGLWindow()
{
	glfwTerminate(); 
	delete s_pInstance;
}

int OpenGLWindow::InitWindow()
{
	// GLFW initialization
	int retval = glfwInit();

	//Choose GLFW Major and Minor VErsion
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);  // this is needed for the Max OS X

	return retval;
}

void OpenGLWindow::OpenGLFrameBuffersizeCallBack()
{
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_WIDTH);
}

int OpenGLWindow::CreateGEWindow()
{
	m_pCreatedWindowInstance = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "GameEngine - by Bharath", NULL, NULL);

	if (m_pCreatedWindowInstance != NULL)
	{
		glfwMakeContextCurrent(m_pCreatedWindowInstance);
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			return 1; // Load all OpenGL function pointers failed
		}
		return 0;
	}
	else
		return -1;
}

void OpenGLWindow::CloseGEWindow()
{
	glfwSetWindowShouldClose(m_pCreatedWindowInstance, true);

	glfwTerminate();
	return;
}


OpenGLWindow* OpenGLWindow::GetInstance()
{
	if (!s_pInstance)
		s_pInstance = new OpenGLWindow();

	return s_pInstance;
}

#endif // OPENGL

