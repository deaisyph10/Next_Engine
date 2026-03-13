#include "Application.h"
#include "event/event.h"
#include "core/Timestep.h"
#include "GLFW/glfw3.h"
#include "../../NexTApp/Startup.cpp"
#include "glad/glad.h"
#include "renderer/RenderCommands.h"
#include "ImGui/ImGuiLayer.h"
#include "renderer/Renderer.h"
#include "renderer/Renderer2D.h"
#include "input.h"
#include "log.h"
#include "scripting/ScriptEngine.h"
#include <string>
#include <functional>


namespace Exodus
{
#define BIND_EVENT_FN(fn) std::bind(&Application::fn, this, std::placeholders::_1)
	
	Application* Application::s_Instance = nullptr;
	
	Application::Application()
		: m_CameraController(1280.0f / 720.0f, true),
		  m_VA(0), m_VB(0), m_IB(0)
	{
	
		EX_CORE_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;
		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(EX_BIND_EVENT_FN(Application::OnEvent));
	
		Renderer::Init();
		ScriptEngine::Init();
	}
	
	Application::~Application()
	{
		ScriptEngine::Shutdown();
		Renderer::Shutdown();
	}

	void Application::close()
	{
		isRunning = false;
	}
	void Application::run()
	{
		isRunning = true;		
		while (isRunning)
		{
			
			float time = (float)glfwGetTime();
			Timestep timestep = time - m_LastFrameTime;
			m_LastFrameTime = time;

			// call OnUpdate on layers
			if (!m_Minimized)
			{
				for (Layer* layer : m_LayerStack)
					layer->OnUpdate(timestep);
			}
			// call ImGui rendering on layers
			m_ImGuiLayer->Begin();
			for (Layer* layer : m_LayerStack)
				layer->OnImguiRender();
			m_ImGuiLayer->End();
			m_Window->OnUpdate();
		}

	}

	void Application::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));
		dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(OnWindowResize));

		//EX_CORE_TRACE("Event: {0}", e.ToString());

		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin(); )
		{
			(*--it)->OnEvent(e);
			if (e.m_Handled)
				break;
		}
	}

	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* layer)
	{
		m_LayerStack.PushOverlay(layer);
		layer->OnAttach();
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		isRunning = false;
		return false;
	}

	bool Application::OnWindowResize(WindowResizeEvent& e)
	{
		return false;
	}
}