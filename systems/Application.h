#pragma once
#include "window.h"
#include "Event/Event.h"
#include <string>
#include <iostream>
#include "layer.h"
#include "LayerStack.h"
#include "Event/ApplicationEvent.h"
#include "ImGui/ImGuiLayer.h"
#include "OrthographicCameraController.h"
#include <memory>


namespace Exodus
{
	class Application
	{
	public:
		Application();
		~Application();

		void close();
		void run();
		void OnEvent(Event& e);
		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);
		static Application& Get() { return *s_Instance; }
		Window& GetWindow() { return *m_Window; }

	private:
		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);

		// Initialize member variables in the declaration
		unsigned int m_VA = 0, m_VB = 0, m_IB = 0;
		
		bool isRunning = false;
		bool m_Minimized = false;
		float m_LastFrameTime = 0.0f;
		static Application* s_Instance;
		
		std::unique_ptr<Window> m_Window;
		std::unique_ptr<ImGuiLayer> m_ImGuiLayer;
		OrthographicCameraController m_CameraController;
		LayerStack m_LayerStack;

	};
}