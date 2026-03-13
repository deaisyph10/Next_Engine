#pragma once
#include "GLFW/glfw3.h"
#include "Event/Event.h"
#include <functional>
#include <string>

namespace Exodus
{
	struct WindowProps
	{
		std::string Title;
		unsigned int Width;
		unsigned int Height;
		WindowProps(const std::string& title = "NexT Engine",
			unsigned int width = 1280,
			unsigned int height = 720)
			: Title(title), Width(width), Height(height)
		{
		}
	};

	class Window
	{
	public:
		Window(int width, int height, const char* title);
		Window() = default;
		~Window();

		bool shouldClose() const;

		using EventCallbackFn = std::function<void(Event&)>;

		virtual void OnUpdate() = 0;

		void Clear(float r, float g, float b, float a);

		virtual unsigned int GetWidth() const = 0;
		virtual unsigned int GetHeight() const = 0;

		// Window attributes 	
		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() const = 0;

		virtual void* GetNativeWindow() const = 0;

		static Window* Create(const WindowProps& props = WindowProps());

	private:
		GLFWwindow* window;
	};
}