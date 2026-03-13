#pragma once
#include "entt/entt.hpp"
#include "Core/Timestep.h"
#include <renderer/EditorCamera.h>
#include <core/UUID.h>
#include <core.h>

template<typename T>
using Ref = std::shared_ptr<T>;

class b2World;



namespace Exodus {
	class EditorCamera; // Forward declaration
	class Entity;
	class Scene
	{
	public:
		Scene();
		~Scene();

		static Ref<Scene> Copy(Ref<Scene> other);

		entt::registry& GetRegistry() { return m_Registry; }
		
		void OnUpdateEditor(Timestep ts, EditorCamera& camera);
		void OnUpdateRuntime(Timestep ts);
		void OnUpdateSimulation(Timestep ts, EditorCamera& camera);
		
		void OnRuntimeStart();
		void OnRuntimeStop();

		void OnSimulationStart();
		void OnSimulationStop();

		Entity CreateEntity(const std::string& name);
		Entity CreateEntity_UUID(const std::string& name, UUID uuid);

		void DestroyEntity(Entity entity);
		
		void OnViewportResize(uint32_t width, uint32_t height);
		void SetPrimaryCamera(Entity cameraEntity);
		void ClearPrimaryCamera();

		void InitPhysics2D();
		void ShutdownPhysics2D();

		void RenderScene(EditorCamera& camera);

		void DuplicateEntity(Entity entity);
		
		Entity GetPrimaryCameraEntity();
		
		template<typename... Components>
		auto GetAllEntitiesWith()
		{
			return m_Registry.view<Components...>();
		}

		entt::registry m_Registry;
	private:
		uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;
		//std::vector <Entity> m_Entities;
		template<typename T>
		void OnComponentAdded(Entity entity, T& component);

		b2World* m_PhysicsWorld = nullptr;
		

		friend class Entity;
		friend class SceneHierarchyPanel;
		friend class SceneSerializer;
		//friend class b2World;

	};
}
