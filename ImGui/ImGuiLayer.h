#pragma once
#include "Layer.h"
#include "Event/ApplicationEvent.h"
#include "Event/KeyEvent.h"
#include "Event/MouseEvent.h"
#include "core.h"
#include "Event/Event.h"

namespace Exodus
{
	class EXODUS_API ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		virtual void OnAttach()override;
		virtual void OnDetach()override;
		virtual void OnImguiRender() override;

		void SetDarkThemeColors();
		void Begin();
		void End();
	private:
		float m_Time = 0.0f;

	};
};

