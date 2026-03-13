#pragma once

#include "Entity.h"

namespace Exodus
{
	class ScriptableEntity
	{
	public:
		template<typename T>
		T& GetComponent()
		{
			return m_Entity.GetComponent<T>();
		}
	protected:
		virtual void OnCreate() {};
		virtual void OnDestroy() {};
		virtual void OnUpdate(Timestep ts) {};

	protected:
		Entity m_Entity;
		friend class Scene;
	};
}