#include "SceneHierarchyPanel.h"
#include <Scene/Scene.h>
#include <Scene/Entity.h>
#include "vendor/ImGUI/imgui.h"
#include "imgui_internal.h"
#include <Scene/Component.h>
#include "log.h"
#include <glm/gtc/type_ptr.hpp>
#include <filesystem>
#include <renderer/Texture.h>  
#include "scripting/ScriptEngine.h"



namespace Exodus
{
	extern const std::filesystem::path g_AssetPath;

	SceneHierarchyPanel::SceneHierarchyPanel(const Ref<Scene>& scene)
	{
		SetContext(scene);
	}
	void SceneHierarchyPanel::SetContext(const Ref<Scene>& scene)
	{
		m_Context = scene;
		m_SelectionContext = {};
	}
	void SceneHierarchyPanel::OnImGuiRender()
	{
		if (!m_Context)
			return;

		// ------------------------Scene Hierarchy Panel------------------------

		ImGui::Begin("Scene Hierarchy");
		auto& registry = m_Context->GetRegistry();

		for (auto entityID : registry.view<TagComponent>())
		{
			Entity entity{ entityID, m_Context.get() };
			DrawEntityNode(entity);
		}
		if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
		{
			m_SelectionContext = {};
		}
		if (ImGui::BeginPopupContextWindow())
		{
			if (ImGui::MenuItem("Create Empty Entity"))
			{
				
				Entity newEntity = m_Context->CreateEntity("Empty Entity");
				m_SelectionContext = newEntity;
				
			}
			ImGui::EndPopup();
		}
		ImGui::End();
		// ----------------------End Scene Hierarchy Panel------------------------
		
		// --------------------------Properties Panel---------------------------
		ImGui::Begin("Properties");
		if (m_SelectionContext)
		{
			DrawComponents(m_SelectionContext);
		}
		ImGui::End();
		// -----------------------End Properties Panel--------------------------
	}

	void SceneHierarchyPanel::SetSelectedEntity(Entity entity)
	{
		m_SelectionContext = entity;
	}

	void SceneHierarchyPanel::DrawVec3Control(const std::string& name, glm::vec3& values, float resetValue, float column)
	{
		ImGuiIO& io = ImGui::GetIO();
		auto boldfont = io.Fonts->Fonts[1];

		// USE THIS PUSHID to control binding all three X values for Controller
			ImGui::PushID(name.c_str());
		//------------------------------------------------------------------
			ImGui::Columns(2);
			ImGui::SetColumnWidth(0, column);
			ImGui::Text(name.c_str());
			ImGui::NextColumn();

			ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
			ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });
			float lineHeight = ImGui::GetFontSize() + ImGui::GetStyle().FramePadding.y * 2.0f;
			ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

		//X------------------------------------
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
			ImGui::PushFont(boldfont);
			if (ImGui::Button("X", buttonSize))
			{
				values.x = resetValue;
			}
			ImGui::PopFont();
			ImGui::PopStyleColor(3);
			ImGui::SameLine();
			ImGui::DragFloat("##X", &values.x, 0.1f);
			ImGui::PopItemWidth();
		//Y------------------------------------
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.3f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.35f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.15f, 1.0f });
			ImGui::SameLine();
			ImGui::PushFont(boldfont);
			if (ImGui::Button("Y", buttonSize))
			{
				values.y = resetValue;
			}
			ImGui::PopFont();
			ImGui::PopStyleColor(3);
			ImGui::SameLine();
			ImGui::DragFloat("##Y", &values.y, 0.1f);
			ImGui::PopItemWidth();
			
		//Z------------------------------------
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.0f, 0.35f, 0.85f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
			ImGui::SameLine();
			ImGui::PushFont(boldfont);
			if (ImGui::Button("Z", buttonSize))
			{
				values.z = resetValue;
			}
			ImGui::PopFont();
			ImGui::PopStyleColor(3);
			ImGui::SameLine();
			ImGui::DragFloat("##Z", &values.z, 0.1f);
			ImGui::PopItemWidth();
			ImGui::PopStyleVar();
			ImGui::SameLine();
		//------------------End of X Y Z controls------------------	
			ImGui::Columns(1);
			ImGui::PopID();
	}

	void SceneHierarchyPanel::DrawEntityNode(Entity entity)
	{
		const auto& tag = entity.GetComponent<TagComponent>().Tag;
	//	ImGui::Text("%s", tag.c_str());

		ImGuiTreeNodeFlags flags = ((m_SelectionContext == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
		flags |= ImGuiTreeNodeFlags_SpanAvailWidth;
		bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, tag.c_str());
		if (ImGui::IsItemClicked())
		{
			m_SelectionContext = entity;
		}
		bool entityDeleted = false;
		if (ImGui::BeginPopupContextWindow())
		{
			if (ImGui::MenuItem("Delete Entity"))
				entityDeleted = true;
												
			ImGui::EndPopup();
		}
		if (opened)
		{
			ImGuiTreeNodeFlags Flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_Leaf;
			bool opened = ImGui::TreeNodeEx((void*)9817239, Flags, "Components");
			if (opened)
			{
				ImGui::TreePop();
			}
			ImGui::TreePop();
		}
		if (entityDeleted)
		{
			m_Context->DestroyEntity(entity);
			if (m_SelectionContext == entity)
				m_SelectionContext = {};
		}
	}


	template<typename T, typename UIfunction>
	static void DrawComponent(const std::string& name, Entity entity, UIfunction uifunction)
	{
		const ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_AllowItemOverlap;
		if (entity.HasComponent<T>())
		{
			auto& component = entity.GetComponent<T>();
			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4, -2 });
			bool ifopen = (ImGui::TreeNodeEx((void*)typeid(T).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, name.c_str()));
			ImGui::SameLine(ImGui::GetWindowWidth()-25.0f);
			if (ImGui::Button("x", ImVec2{ 15, 15 }))
			{
				ImGui::OpenPopup("ComponentSettings");
			}
			ImGui::PopStyleVar();

			bool removeComponent = false;
			if (ImGui::BeginPopup("ComponentSettings"))
			{
				if (ImGui::MenuItem("Remove Component"))
				{
					removeComponent = true;
				}
				ImGui::EndPopup();
			}
			if (ifopen)
			{
				uifunction(component);
				ImGui::TreePop();
			}
			if (removeComponent)
			{
				entity.RemoveComponent<T>();
			}
		}

	}

	void SceneHierarchyPanel::DrawComponents(Entity entity)
	{
		if (entity.HasComponent<TagComponent>())
		{
			auto& tag = entity.GetComponent<TagComponent>().Tag;

			char buffer[256];
			memset(buffer, 0, sizeof(buffer));
			strcpy_s(buffer, tag.c_str());
			if (ImGui::InputText("Tag", buffer, sizeof(buffer)))
			{
				tag = std::string(buffer);
			}
			ImGui::Text("Tag: %s", tag.c_str());
		}


		ImGui::SameLine();
		ImGui::PushItemWidth(-1);

		if (ImGui::Button("Add Component"))
			ImGui::OpenPopup("AddComponent");

		if (ImGui::BeginPopup("AddComponent"))
		{
			DisplayAddComponentEntry<CameraComponent>("Camera");
			DisplayAddComponentEntry<ScriptComponent>("Script");
			DisplayAddComponentEntry<SpriteRendererComponent>("Sprite Renderer");
			DisplayAddComponentEntry<CircleRendererComponent>("Circle Renderer");
			DisplayAddComponentEntry<Rigidbody2DComponent>("Rigidbody 2D");
			DisplayAddComponentEntry<BoxCollider2DComponent>("Box Collider 2D");
			DisplayAddComponentEntry<CircleCollider2DComponent>("Circle Collider 2D");

			ImGui::EndPopup();
		}

		ImGui::PopItemWidth();


		DrawComponent<TransformComponent>("Transform Component", entity, [this](auto& component)
			{
				DrawVec3Control("Translation", component.Translation);
				DrawVec3Control("Rotation", component.Rotation);
				DrawVec3Control("Scale", component.Scale, 1.0f);
				ImGui::Separator();
			});

		DrawComponent<CameraComponent>("Camera Component", entity, [](auto& component)
			{
				const char* projectionTypeStrings[] = { "Perspective", "Orthographic" };
				const char* currentProjectionTypeString = projectionTypeStrings[(int)component.Camera.GetProjectionType()];
				if (ImGui::BeginCombo("Projection Type", currentProjectionTypeString))
				{
					for (int i = 0; i < 2; i++)
					{
						bool isSelected = currentProjectionTypeString == projectionTypeStrings[i];
						if (ImGui::Selectable(projectionTypeStrings[i], isSelected))
						{
							currentProjectionTypeString = projectionTypeStrings[i];
							component.Camera.SetProjectionType((SceneCamera::ProjectionType)i);
						}
						if (isSelected)
							ImGui::SetItemDefaultFocus();
					}
					ImGui::EndCombo();
				}
				if (component.Camera.GetProjectionType() == SceneCamera::ProjectionType::Orthographic)
				{
					float orthoSize = component.Camera.GetOrthographicSize();
					if (ImGui::DragFloat("Distance", &orthoSize))
					{
						component.Camera.SetOrthographicSize(orthoSize);
					}
					float orthoNear = component.Camera.GetOrthographicNearClip();
					if (ImGui::DragFloat("Near Clip", &orthoNear))
					{
						component.Camera.SetOrthographicNearClip(orthoNear);
					}
					float orthoFar = component.Camera.GetOrthographicFarClip();
					if (ImGui::DragFloat("Far Clip", &orthoFar))
					{
						component.Camera.SetOrthographicFarClip(orthoFar);
					}
				}
				if (component.Camera.GetProjectionType() == SceneCamera::ProjectionType::Perspective)
				{
					float perspectiveFOV = glm::degrees(component.Camera.GetPerspectiveFOV());
					if (ImGui::DragFloat("FOV", &perspectiveFOV))
					{
						component.Camera.SetPerspectiveFOV(glm::radians(perspectiveFOV));
					}
					float perspectiveNear = component.Camera.GetPerspectiveNearClip();
					if (ImGui::DragFloat("Near Clip", &perspectiveNear))
					{
						component.Camera.SetPerspectiveNearClip(perspectiveNear);
					}
					float perspectiveFar = component.Camera.GetPerspectiveFarClip();
					if (ImGui::DragFloat("Far Clip", &perspectiveFar))
					{
						component.Camera.SetPerspectiveFarClip(perspectiveFar);
					}
				}
			});

			DrawComponent<ScriptComponent>("Script", entity, [](auto& component)
			{
				bool scriptClassExists = ScriptEngine::EntityClassExists(component.ClassName);

				static char buffer[64];
				strcpy_s(buffer, component.ClassName.c_str());

				if (!scriptClassExists)
					ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.9f, 0.2f, 0.3f, 1.0f));

				if (ImGui::InputText("Class", buffer, sizeof(buffer)))
					component.ClassName = buffer;

				if (!scriptClassExists)
					ImGui::PopStyleColor();
			});

		   DrawComponent<SpriteRendererComponent>("Sprite Renderer Component", entity, [](auto& component)
				{
					ImGui::ColorEdit4("Color", glm::value_ptr(component.Color));

					//texture
					ImGui::Button("Texture", ImVec2(100.0f, 0.0f));
					if (ImGui::BeginDragDropTarget())
					{
						if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
						{
							const wchar_t* path = (const wchar_t*)payload->Data;
							std::filesystem::path texturepath = std::filesystem::path(g_AssetPath) / path;
							component.Texture = Texture2D::Create(texturepath.string());

						}
						ImGui::EndDragDropTarget();
					}
					ImGui::DragFloat("Tiling Factor", &component.TilingFactor, 0.1f, 0.0f, 100.0f);

				});

		   DrawComponent<CircleRendererComponent>("Circle Renderer Component", entity, [](auto& component)
			   {
				   ImGui::ColorEdit4("Color", glm::value_ptr(component.Color));
				   ImGui::DragFloat("Thckness", &component.Thickness, 0.025f, 0.0f, 1.0f);
				   ImGui::DragFloat("Fade", &component.Fade, 0.00025f, 0.0f, 1.0f);
				   
			   });

		   DrawComponent<Rigidbody2DComponent>("Rigidbody 2D", entity, [](auto& component)
			   {
				   const char* bodyTypeStrings[] = { "static", "dynamic", "kinematic" };
				   const char* currentbodyTypeString = bodyTypeStrings[(int)component.Type];
				   if (ImGui::BeginCombo("Body Type", currentbodyTypeString))
				   {
					   for (int i = 0; i < 3; i++)
					   {
						   bool isSelected = currentbodyTypeString == bodyTypeStrings[i];
						   if (ImGui::Selectable(bodyTypeStrings[i], isSelected))
						   {
							   currentbodyTypeString = bodyTypeStrings[i];
							   component.Type = (Exodus::Rigidbody2DComponent::BodyType)i;
						   }
						   if (isSelected)
							   ImGui::SetItemDefaultFocus();
					   }
					   ImGui::EndCombo();
				   }
				   ImGui::Checkbox("Fixed Rotation", &component.SetFixedRotation);
			   });

		   DrawComponent<BoxCollider2DComponent>("BoxCollider 2D", entity, [](auto& component)
				{
					ImGui::DragFloat2("OffSet", glm::value_ptr(component.Offset));
					ImGui::DragFloat2("Size", glm::value_ptr(component.Size));
					ImGui::DragFloat("Density", &component.Density, 0.01f, 0.0f, 1.0f);
					ImGui::DragFloat("Friction", &component.Friction, 0.01f, 0.0f, 1.0f);
					ImGui::DragFloat("Restitution", &component.Restitution, 0.01f, 0.0f, 1.0f);
				});
		   DrawComponent<CircleCollider2DComponent>("CircleCollider 2D", entity, [](auto& component)
			   {
				   ImGui::DragFloat2("OffSet", glm::value_ptr(component.Offset));
				   ImGui::DragFloat("Radius", &component.Radius);
				   ImGui::DragFloat("Density", &component.Density, 0.01f, 0.0f, 1.0f);
				   ImGui::DragFloat("Friction", &component.Friction, 0.01f, 0.0f, 1.0f);
				   ImGui::DragFloat("Restitution", &component.Restitution, 0.01f, 0.0f, 1.0f);
			   });

	}

	template<typename T>
	void SceneHierarchyPanel::DisplayAddComponentEntry(const std::string& entryName) {
		if (!m_SelectionContext.HasComponent<T>())
		{
			if (ImGui::MenuItem(entryName.c_str()))
			{
				m_SelectionContext.AddComponent<T>();
				ImGui::CloseCurrentPopup();
			}
		}
	}

} // namespace Exodus