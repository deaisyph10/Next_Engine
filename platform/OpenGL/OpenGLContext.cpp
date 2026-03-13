#include "OpenGLContext.h"
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include "Log.h"

#ifndef APIENTRY
#define APIENTRY
#endif

namespace Exodus
{
	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
		: m_WindowHandle(windowHandle)
	{
		EX_CORE_ASSERT(windowHandle, "Window handle is null!")
	}
	void OpenGLContext::Init()
	{
		glfwMakeContextCurrent(m_WindowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		EX_CORE_ASSERT(status, "Failed to initialize Glad!");
		//	EX_CORE_INFO("OpenGL Renderer: {0}", glGetString(GL_VENDOR));
		//	EX_CORE_INFO("  Vendor: {0}", glGetString(GL_VENDOR));
		//	EX_CORE_INFO("  Renderer: {0}", glGetString(GL_RENDERER));
		//	EX_CORE_INFO("  Version: {0}", glGetString(GL_VERSION));


	}
	void OpenGLContext::SwapBuffers()
	{
		glfwSwapBuffers(m_WindowHandle);
	}

}