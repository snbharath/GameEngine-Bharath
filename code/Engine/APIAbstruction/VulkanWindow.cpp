#if defined(VULKAN)

#include "VulkanWindow.h"

using namespace GE;

VulkanWindow* VulkanWindow::s_pInstance = nullptr;

//----------------------------------------------------------------------

VulkanWindow::VulkanWindow()
	:m_pGLFWwindow(nullptr)
{
}

//----------------------------------------------------------------------

VulkanWindow::~VulkanWindow()
{
	glfwTerminate();
	delete s_pInstance;
}

//----------------------------------------------------------------------

bool VulkanWindow::InitWindow()
{
	// GLFW initialization
	int retval = glfwInit();

	//Choose GLFW Major and Minor VErsion
	//glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	//glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

	return retval;
}

//----------------------------------------------------------------------

bool VulkanWindow::CreateGEWindow()
{
	m_pGLFWwindow = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Vulkan Window - Bharath", NULL, NULL);

	// if failed to create a glfw window then return
	if (m_pGLFWwindow == nullptr)
	{
		TerminateGEWindow();
		return false;
	}

	glfwMakeContextCurrent(m_pGLFWwindow);

	return true;
}

//----------------------------------------------------------------------

// returns false if it terminates
bool VulkanWindow::RenderFrameUpdate()
{
	if (!glfwWindowShouldClose(m_pGLFWwindow))
	{
		//handle input
		ProcessInput(m_pGLFWwindow);
		glfwPollEvents();

		return true;
	}

	return false;
}

//----------------------------------------------------------------------

void VulkanWindow::ProcessInput(GLFWwindow* pWindow)
{
	if (glfwGetKey(pWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		CloseGEWindow();
		TerminateGEWindow();
	}
}

//----------------------------------------------------------------------

void VulkanWindow::CloseGEWindow()
{
	glfwSetWindowShouldClose(m_pGLFWwindow, true);
}

//----------------------------------------------------------------------

void VulkanWindow::TerminateGEWindow()
{
	glfwTerminate();
}

//----------------------------------------------------------------------

VulkanWindow* VulkanWindow::GetInstance()
{
	if (!s_pInstance)
	{
		s_pInstance = new VulkanWindow();
	}

	return s_pInstance;
}

//----------------------------------------------------------------------

#endif // VULKAN