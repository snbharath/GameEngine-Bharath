#if defined(OPENGL)

#include "OpenGLWindow.h"


using namespace GE;

OpenGLWindow* OpenGLWindow::s_pInstance = nullptr;

//----------------------------------------------------------------------

OpenGLWindow::OpenGLWindow()
	:m_pGLFWwindow(nullptr)
{
}

//----------------------------------------------------------------------

OpenGLWindow::~OpenGLWindow()
{
	TerminateGEWindow();
}

//----------------------------------------------------------------------

bool OpenGLWindow::InitWindow()
{
	// GLFW initialization
	int retval = glfwInit();

	//Choose GLFW Major and Minor VErsion
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	return retval;
}

//----------------------------------------------------------------------

void OpenGLWindow::OpenGLFrameBufferResizeCallBack(GLFWwindow* pGlfwWindow, int width, int height)
{
	glViewport(0, 0, width, height);
}

//----------------------------------------------------------------------

bool OpenGLWindow::CreateGEWindow()
{
	m_pGLFWwindow = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "OpenGL Window - Bharath", nullptr, nullptr);
	
	// if failed to create a glfw window then return
	if (m_pGLFWwindow == nullptr)
	{
		TerminateGEWindow();
		return false;
	}

	glfwMakeContextCurrent(m_pGLFWwindow);
	glfwSetFramebufferSizeCallback(m_pGLFWwindow, OpenGLWindow::OpenGLFrameBufferResizeCallBack);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		return false; // Load all OpenGL function pointers failed
	}
	return true;
}

//----------------------------------------------------------------------

// returns false if it terminates
bool OpenGLWindow::RenderFrameUpdate()
{
	if(!glfwWindowShouldClose(m_pGLFWwindow))
	{
		//handle input
		ProcessInput(m_pGLFWwindow);

		//render come stuff like color
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//glfw swap buffers and poll IO events(pressed, released, mouse moved etc...)
		glfwSwapBuffers(m_pGLFWwindow);
		glfwPollEvents();

		return true;
	}

	return false;
}

//----------------------------------------------------------------------

void OpenGLWindow::ProcessInput(GLFWwindow * pWindow)
{
	if (glfwGetKey(pWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		CloseGEWindow();
		TerminateGEWindow();
	}
}

//----------------------------------------------------------------------

void OpenGLWindow::CloseGEWindow()
{
	glfwSetWindowShouldClose(m_pGLFWwindow, true);
}

//----------------------------------------------------------------------

void OpenGLWindow::TerminateGEWindow()
{
	glfwDestroyWindow(m_pGLFWwindow);
	glfwTerminate();
}

//----------------------------------------------------------------------

OpenGLWindow* OpenGLWindow::GetInstance()
{
	if (!s_pInstance)
		s_pInstance = new OpenGLWindow();

	return s_pInstance;
}

//----------------------------------------------------------------------

bool OpenGLWindow::DeleteInstance()
{
	if (s_pInstance == nullptr)
	{
		return false;
	}

	delete s_pInstance;
	s_pInstance = nullptr;
	return true;
}

//----------------------------------------------------------------------

#endif // OPENGL

