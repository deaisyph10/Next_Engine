#include "window.h"
#include <stdexcept>
#include "GLFW/glfw3.h"
#include "glad/glad.h"
#include "log.h"


namespace Exodus
{
	Window::Window(int width, int height, const char* title)
	{
		if (!glfwInit())
		{
			throw std::runtime_error("Failed to initialize GLFW");
		}
		window = glfwCreateWindow(width, height, title, nullptr, nullptr);
		if (!window)
		{
			glfwTerminate();
			throw std::runtime_error("Failed to create GLFW window");
		}
		
	}
	Window::~Window()
	{
	}

	bool Window::shouldClose() const
	{
		return false;
	}

	void Window::Clear(float r, float g, float b, float a)
	{
		glClearColor(1.1f, 1.1f, 0.1f, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	}


}