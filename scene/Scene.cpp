#include "Scene.h"
#include "Component.h"
#include "renderer/Renderer2D.h"
#include "Entity.h"
#include <core/TimeStep.h>
#include <renderer/EditorCamera.h>
#include <Camera.h>
#include "core/UUID.h"
#include "ScriptableEntity.h"

#include <entt/entt.hpp>
#include <glm/fwd.hpp>
#include <cstdint>
#include <string>

// Box2D includes
#include <box2d/b2_world.h>
#include <box2d/b2_body.h>
#include <box2d/b2_fixture.h>
#include <box2d/b2_polygon_shape.h>
#include <box2d/b2_circle_shape.h>
#include <core.h>
#include <vector>

namespace Exodus {

	Scene::Scene()
	{

	}

	Scene::~Scene()
	{
		delete m_PhysicsWorld;
	}

	// ======================================================================================== UTILITY FUNCTIONS
	
	static void DoMath(const glm::mat4 transform)
	{
	}

	static b2BodyType GetB2BodyType(Rigidbody2DComponent::BodyType type)
	{
		switch (type)
		{
			case Rigidbody2DComponent::BodyType::Static:  return b2_staticBody;
			case Rigidbody2DComponent::BodyType::Dynamic: return b2_dynamicBody;
			case Rigidbody2DComponent::BodyType::Kinematic: return b2_kinematicBody;
		}
		return b2_staticBody; // Default case
	}

	void Scene::OnViewportResize(uint32_t width, uint32_t height)
	{
		m_ViewportWidth = width;
		m_ViewportHeight = height;

		auto view = m_Registry.view<CameraComponent>();
		for (auto entity : view)
		{
			auto& camera = view.get<CameraComponent>(entity);
			if (!camera.Primary)
				camera.Camera.SetViewportSize(width, height);
		}
	}

	template<typename Component>
	static void CopyComponent(entt::registry& dst, entt::registry& src, const std::unordered_map<UUID, entt::entity> entityMap)
	{
		auto view = src.view<Component>();
		for (auto e : view)
		{
			UUID uuid = src.get<IDComponent>(e).ID;
			entt::entity DstEnttID = entityMap.at(uuid);

			auto& component = src.get<Component>(e);
			dst.emplace_or_replace<Component>(DstEnttID, component);
		}
	}

	template<typename Component>
	static void CopyComponentifExists(Entity dst, Entity src)
	{
		if (src.HasComponent<Component>())
		{
			dst.AddOrReplaceComponent<Component>(src.GetComponent<Component>());
		}
	}

	// ======================================================================================== ENTITY API

	Entity Scene::CreateEntity(const std::string& name)
	{
		return CreateEntity_UUID(name, UUID());
	}

	Entity Scene::CreateEntity_UUID(const std::string& name, UUID uuid)
	{
		Entity entity = { m_Registry.create(), this };
		entity.AddComponent<IDComponent>(uuid);
		entity.AddComponent<TransformComponent>();
		auto& tag = entity.AddComponent<TagComponent>();
		tag.Tag = name.empty() ? "Entity" : name;
		return entity;
	}

	void Scene::DestroyEntity(Entity entity)
	{
		m_Registry.destroy (static_cast<entt::entity>(entity));

	}

	void Scene::DuplicateEntity(Entity entity)
	{
		Entity newEntity = CreateEntity(entity.GetName() + " Copy");

		CopyComponentifExists<TransformComponent>(newEntity, entity);
		CopyComponentifExists<SpriteRendererComponent>(newEntity, entity);
		CopyComponentifExists<CircleRendererComponent>(newEntity, entity);
		CopyComponentifExists<CameraComponent>(newEntity, entity);
		CopyComponentifExists<NativeScriptComponent>(newEntity, entity);
		CopyComponentifExists<Rigidbody2DComponent>(newEntity, entity);
		CopyComponentifExists<BoxCollider2DComponent>(newEntity, entity);
		CopyComponentifExists<CircleCollider2DComponent>(newEntity, entity);
	}
	Ref<Scene> Scene::Copy(Ref<Scene> other)
	{

		Ref<Scene> newScene = CreateRef<Scene>();
		newScene->m_ViewportWidth = other->m_ViewportWidth;
		newScene->m_ViewportHeight = other->m_ViewportHeight;


		auto& srcRegistry = other->m_Registry;
		auto& dstRegistry = newScene->m_Registry;
		std::unordered_map<UUID, entt::entity> entityMap;

		// Create entities in the new scene and map their UUIDs to the new entity IDs
		auto idView = srcRegistry.view<IDComponent>();
		for (auto e : idView)
		{
			UUID uuid = srcRegistry.get<IDComponent>(e).ID;
			const auto& name = srcRegistry.get<TagComponent>(e).Tag;
			Entity newEntity = newScene->CreateEntity_UUID(name, uuid);
			entityMap[uuid] = newEntity;
		}

		// Copy components except IDComponent and TagComponent (already handled during entity creation)
		CopyComponent<TransformComponent>(dstRegistry, srcRegistry, entityMap);
		CopyComponent<SpriteRendererComponent>(dstRegistry, srcRegistry, entityMap);
		CopyComponent<CircleRendererComponent>(dstRegistry, srcRegistry, entityMap);
		CopyComponent<CameraComponent>(dstRegistry, srcRegistry, entityMap);
		CopyComponent<NativeScriptComponent>(dstRegistry, srcRegistry, entityMap);
		CopyComponent<Rigidbody2DComponent>(dstRegistry, srcRegistry, entityMap);
		CopyComponent<BoxCollider2DComponent>(dstRegistry, srcRegistry, entityMap);
		CopyComponent<CircleCollider2DComponent>(dstRegistry, srcRegistry, entityMap);


		return newScene;
	}


	// ======================================================================================== SCENE CAMERA API
	
	Entity Scene::GetPrimaryCameraEntity()
	{
		auto view = m_Registry.view<CameraComponent>();
		for (auto entity : view)
		{
			const auto& camera = view.get<CameraComponent>(entity);
			if (camera.Primary)
				return Entity{ entity, this };
		}

		return {};
	}
	void Scene::SetPrimaryCamera(Entity cameraEntity)
	{
		auto view = m_Registry.view<CameraComponent>();
		for (auto entity : view)
		{
			auto& camera = view.get<CameraComponent>(entity);
			camera.Primary = false;
		}
		auto& camera = cameraEntity.GetComponent<CameraComponent>();
		camera.Primary = true;
	}

	void Scene::ClearPrimaryCamera()
	{
		auto view = m_Registry.view<CameraComponent>();
		for (auto entity : view)
		{
			auto& camera = view.get<CameraComponent>(entity);
			camera.Primary = false;
		}
	}

	// ========================================================================================= PHYSICS API

	void Scene::OnRuntimeStart()
	{
		InitPhysics2D();
	}

	void Scene::OnRuntimeStop()
	{
		ShutdownPhysics2D();		
	}

	void Scene::OnSimulationStart()
	{
		InitPhysics2D();
	}
	
	void Scene::OnSimulationStop()
	{
		ShutdownPhysics2D();
	}

	void Scene::ShutdownPhysics2D()
	{
		if (m_PhysicsWorld)
		{
			delete m_PhysicsWorld;
			m_PhysicsWorld = nullptr;
		}
	}

	void Scene::InitPhysics2D()
	{
		m_PhysicsWorld = new b2World({ 0.0f, -9.81f });

		auto view = m_Registry.view<Rigidbody2DComponent>();
		for (auto e : view)
		{
			Entity entity = { e, this };
			auto& rb2d = entity.GetComponent<Rigidbody2DComponent>();
			auto& transform = entity.GetComponent<TransformComponent>();

			b2BodyDef bodyDef;
			bodyDef.type = GetB2BodyType(rb2d.Type);
			bodyDef.position.Set(transform.Translation.x, transform.Translation.y);
			bodyDef.angle = transform.Rotation.x;

			b2Body* Body = m_PhysicsWorld->CreateBody(&bodyDef);
			Body->SetFixedRotation(rb2d.SetFixedRotation);
			rb2d.RuntimeBody = Body;


			if (entity.HasComponent<BoxCollider2DComponent>())
			{
				auto& bc2d = entity.GetComponent<BoxCollider2DComponent>();

				b2PolygonShape boxShape;
				boxShape.SetAsBox(bc2d.Size.x * transform.Scale.x, bc2d.Size.y * transform.Scale.y);

				b2FixtureDef fixtureDef;
				fixtureDef.shape = &boxShape;
				fixtureDef.density = bc2d.Density;
				fixtureDef.friction = bc2d.Friction;
				fixtureDef.restitution = bc2d.Restitution;
				fixtureDef.restitutionThreshold = bc2d.RestitutionThreshold;

				Body->CreateFixture(&fixtureDef);
			}
			if (entity.HasComponent<CircleCollider2DComponent>())
			{
				auto& cc2d = entity.GetComponent<CircleCollider2DComponent>();

				b2CircleShape circleShape;
				circleShape.m_p.Set(cc2d.Offset.x, cc2d.Offset.y);
				circleShape.m_radius = transform.Scale.x * cc2d.Radius;

				b2FixtureDef fixtureDef;
				fixtureDef.shape = &circleShape;
				fixtureDef.density = cc2d.Density;
				fixtureDef.friction = cc2d.Friction;
				fixtureDef.restitution = cc2d.Restitution;
				fixtureDef.restitutionThreshold = cc2d.RestitutionThreshold;

				Body->CreateFixture(&fixtureDef);
			}
		}
	}
	// ========================================================================================= RENDERING API

	void Scene::RenderScene(EditorCamera& camera)
	{
		Renderer2D::BeginScene(camera);
		// Sprites
		{
			auto group = m_Registry.group<SpriteRendererComponent>(entt::get<TransformComponent>);
			for (auto entity : group)
			{
				auto [sprite, transform] = group.get<SpriteRendererComponent, TransformComponent>(entity);
				Renderer2D::DrawSprite(transform.GetTransform(), sprite, (int)entity);
				//Renderer2D::DrawRect(transform.GetTransform(), glm::vec4(1.0), (int)entity);
			}
		}

		// Draw circles
		{
			auto group = m_Registry.group<CircleRendererComponent>(entt::get<TransformComponent>);
			for (auto entity : group)
			{
				auto [circle, transform] = group.get<CircleRendererComponent, TransformComponent>(entity);

				Renderer2D::DrawCircle(transform.GetTransform(), circle.Color, circle.Thickness, circle.Fade, (int)entity);
			}
			// Circle BUG!!! Circles do not render on startup Scene...
			// A camera need to be created as well as scene needs to init runtime.
		}

		

		Renderer2D::EndScene();
	}
	
	// ========================================================================================= UPDATE API

	void Scene::OnUpdateEditor(Timestep ts, EditorCamera& camera)
	{
		RenderScene(camera);
	}

	void Scene::OnUpdateRuntime(Timestep ts)
	{
		{
			m_Registry.view<Exodus::NativeScriptComponent>().each
			(
				[=](auto entity, auto& nsc)
				{
					{
						if (!nsc.Instance)
							nsc.Instance = nsc.InstantiateScript();
						nsc.Instance->m_Entity = Entity{ entity, this };
						nsc.Instance->OnCreate();
					}
					nsc.Instance->OnUpdate(ts);
				});
		}
		//Physics
		{
			const int32_t velocityIterations = 6;
			const int32_t positionIterations = 2;
			m_PhysicsWorld->Step(ts, velocityIterations, positionIterations);

			auto view = m_Registry.view<Rigidbody2DComponent>();
			for(auto e : view)
			{
				Entity entity = { e, this };
				auto& transform = entity.GetComponent<TransformComponent>();
				auto& rb2d = entity.GetComponent<Rigidbody2DComponent>();
				b2Body* body = (b2Body*)rb2d.RuntimeBody;
				const auto& position = body->GetPosition();
				transform.Translation.x = position.x;
				transform.Translation.y = position.y;
				transform.Rotation.x = body->GetAngle();

			}
		}
		Camera* mainCamera = nullptr;
		glm::mat4* cameraTransform = nullptr;
		{
			auto camGroup = m_Registry.group<CameraComponent>(entt::get<TransformComponent>);
			for (auto entity : camGroup) {
				auto [camera, transform] = camGroup.get<CameraComponent, TransformComponent>(entity);
				if (camera.Primary)
				{
					mainCamera = &camera.Camera;
					static glm::mat4 tempTransform;
					tempTransform = transform.GetTransform();
					cameraTransform = &tempTransform;
					break;
				}
			}
		}
		if (mainCamera)
		{

			Renderer2D::BeginScene(mainCamera->GetProjection(), *cameraTransform);
			{
				auto group = m_Registry.group<SpriteRendererComponent>(entt::get<TransformComponent>);
				for (auto entity : group) {
					auto [sprite, transform] = group.get<SpriteRendererComponent, TransformComponent>(entity);
					
					Renderer2D::DrawSprite(transform.GetTransform(), sprite, (int)entity);
					
				}
			}
			{
				auto group = m_Registry.group<TransformComponent, CircleRendererComponent>();
				for (auto entity : group)
				{
					auto [transform, circle] = group.get<TransformComponent, CircleRendererComponent>(entity);
					Renderer2D::DrawCircle(transform.GetTransform(), circle.Color, circle.Thickness, circle.Fade, (int)entity);
				}
			}

			Renderer2D::EndScene();
		}
	}

	void Scene::OnUpdateSimulation(Timestep ts, EditorCamera& camera)
	{
		//Physics
		{
			const int32_t velocityIterations = 6;
			const int32_t positionIterations = 2;
			m_PhysicsWorld->Step(ts, velocityIterations, positionIterations);

			auto view = m_Registry.view<Rigidbody2DComponent>();
			for (auto e : view)
			{
				Entity entity = { e, this };
				auto& transform = entity.GetComponent<TransformComponent>();
				auto& rb2d = entity.GetComponent<Rigidbody2DComponent>();
				b2Body* body = (b2Body*)rb2d.RuntimeBody;
				const auto& position = body->GetPosition();
				transform.Translation.x = position.x;
				transform.Translation.y = position.y;
				transform.Rotation.x = body->GetAngle();

			}
		}
		//Render
		RenderScene(camera);
	}

	

	// ========================================================================================= COMPONENT CALLBACKS

template<typename T>
	void Scene::OnComponentAdded(Entity entity, T& component)
	{
		static_assert(false, "Unknown component type!");
	}

} // namespace Exodus

// Move all template specializations for OnComponentAdded outside the main Scene class implementation
namespace Exodus {

template<>
void Scene::OnComponentAdded<IDComponent>(Entity entity, IDComponent& component)
{
	// Handle IDComponent initialization if needed
	// Or leave empty if no special handling is required
}

template<>
void Scene::OnComponentAdded<TagComponent>(Entity entity, TagComponent& component)
{
}

template<>
void Scene::OnComponentAdded<TransformComponent>(Entity entity, TransformComponent& component)
{
}

template<>
void Scene::OnComponentAdded<ScriptComponent>(Entity entity, ScriptComponent& component)
{
}

template<>
void Scene::OnComponentAdded<SpriteRendererComponent>(Entity entity, SpriteRendererComponent& component)
{
}

template<>
void Scene::OnComponentAdded<CircleRendererComponent>(Entity entity, CircleRendererComponent& component)
{
}

template<>
void Scene::OnComponentAdded<CameraComponent>(Entity cameraEntity, CameraComponent& component)
{
	component.Camera.SetViewportSize(m_ViewportWidth, m_ViewportHeight);
}

template<>
void Scene::OnComponentAdded<NativeScriptComponent>(Entity entity, NativeScriptComponent& component)
{
}

template<>
void Scene::OnComponentAdded<Rigidbody2DComponent>(Entity entity, Rigidbody2DComponent& component)
{
}

template<>
void Scene::OnComponentAdded<BoxCollider2DComponent>(Entity entity, BoxCollider2DComponent& component)
{
}

template<>
void Scene::OnComponentAdded<CircleCollider2DComponent>(Entity entity, CircleCollider2DComponent& component)
{
}

} // namespace Exodus
