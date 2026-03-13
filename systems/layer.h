#pragma once
#include "Core.h"
#include "Core/Timestep.h"
#include <string>
#include "Event/Event.h"

#ifndef EXODUS_API
#define EXODUS_API
#endif

namespace Exodus
{
	class EXODUS_API Layer
	{
	public:
		Layer(const std::string& name = "Layer");
		virtual ~Layer();
		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate(Timestep ts) {}
		virtual void OnImguiRender() {}
		virtual void OnEvent(Event& event) {}
		inline const std::string& GetName() const { return m_DebugName; }
	protected:
		std::string m_DebugName;
	public:

		Layer() = default;
	};
}