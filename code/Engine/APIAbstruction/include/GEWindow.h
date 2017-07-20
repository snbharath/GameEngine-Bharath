#ifdef OPENGL
#include <glad/glad.h>
#include <glfw3.h>
#endif

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

//its a singleton Game Engine window class
namespace GE
{
	class GEWindow
	{
	private:
		//constructor
		GEWindow() {}

		//GLFW window instance
		GLFWwindow* m_window = nullptr;

		// static pointer as its a singleton class
		static GEWindow *s_WindowInstance;

	public:

		void InitWindow();

		/*
		* Create a Rendering window here
		* This creates a render window of Either OpenGL or DirectX
		*/
		int CreateGEWindow();


		static GEWindow *Get()
		{
			if (!s_WindowInstance)
				s_WindowInstance = new GEWindow();

			return s_WindowInstance;
		}
	};
}