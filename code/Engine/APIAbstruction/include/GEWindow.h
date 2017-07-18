#ifdef OPENGL
#include <glad/glad.h>
#include <glfw3.h>
#endif

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

//its a singleton Game Engine window class
class GEWindow
{
private:
	//constructor
	GEWindow() {}

	// static pointer as its a singleton class
	static GEWindow *s_WindowInstance;

public:

	void InitWindow() const ;

	static GEWindow *GetInstance()
	{
		if (!s_WindowInstance)
			s_WindowInstance = new GEWindow();

		return s_WindowInstance;
	}
};