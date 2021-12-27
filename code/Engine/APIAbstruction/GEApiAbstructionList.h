// Type defs for platform independence

#if defined (OPENGL)
#include "OpenGLWindow.h"
#elif defined (VULKAN)
#include "VulkanWindow.h"
#endif
namespace GE
{
#if defined (OPENGL)
	typedef OpenGLWindow CWindow;
#elif defined (VULKAN)
	typedef VulkanWindow CWindow;
#elif defined(DIRECT3D)
	// define if created...
#endif
}