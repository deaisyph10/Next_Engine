#pragma once
#include "log.h"
#include "Core.h"
#include "Scene/Scene.h"
#include "Scene/Entity.h"
#include <glm/fwd.hpp> // Fixes C2061, C2653, VCIC001 for glm::vec3
#include <string>      // Fixes VCIC001 for std::string

namespace Exodus
{
	class SceneHierarchyPanel
	{
	public:
		SceneHierarchyPanel() = default;
		SceneHierarchyPanel(const Ref<Scene>& scene);

		void SetContext(const Ref<Scene>& scene);
		void ClearSelection() { m_SelectionContext = {}; }

		void OnImGuiRender();

		Entity GetSelectedEntity() const { return m_SelectionContext; }
		void SetSelectedEntity(Entity entity);

	private:
		template<typename T>
		void DisplayAddComponentEntry(const std::string& entryName);

		// Fix C2548: add default value for 'column'
		void DrawVec3Control(const std::string& name, glm::vec3& values, float resetValue = 0.0f, float column = 0.0f); //For TransformComponent
		void DrawEntityNode(Entity entity); //List of entities in the scene
		void DrawComponents(Entity entity); //Components of the selected entity
	protected:

		Ref<Scene> m_Context;
		Entity m_SelectionContext;

	};
}