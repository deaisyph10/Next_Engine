#pragma once
#include <glm/glm.hpp>
#include "Camera.h"

namespace Exodus {
	class SceneCamera : public Camera
	{
	public:
		enum class ProjectionType
		{
			Perspective = 0,
			Orthographic = 1
		};
	public:
		SceneCamera();
		virtual ~SceneCamera() = default;

		// Getters and Setters Orthographic
		float GetOrthographicSize() const { return m_OrthographicSize; }
		float GetOrthographicNearClip() const { return m_Orthographic_NearClip; }
		float GetOrthographicFarClip() const { return m_Orthographic_FarClip; }

		void SetOrthographic(float size, float nearClip, float farClip);
		void SetViewportSize(float width, float height);
		void SetOrthographicSize(float size) { m_OrthographicSize = size; RecalculateProjection(); }
		void SetOrthographicNearClip(float nearClip) { m_Orthographic_NearClip = nearClip; RecalculateProjection(); }
		void SetOrthographicFarClip(float farClip) { m_Orthographic_FarClip = farClip; RecalculateProjection(); }

		// Getters and Setters Perspective
		float GetPerspectiveFOV() const { return m_Perspective_FOV; }
		float GetPerspectiveNearClip() const { return m_Perspective_NearClip; }
		float GetPerspectiveFarClip() const { return m_Perspective_FarClip; }

		void SetPerspective(float fov, float nearClip, float farClip);
		void SetPerspectiveFOV(float fov) { m_Perspective_FOV = fov; RecalculateProjection(); }
		void SetPerspectiveNearClip(float nearClip) { m_Perspective_NearClip = nearClip; RecalculateProjection(); }
		void SetPerspectiveFarClip(float farClip) { m_Perspective_FarClip = farClip; RecalculateProjection(); }

		// Getters and Setters Projection Type
		ProjectionType GetProjectionType() const { return m_ProjectionType; }
		void SetProjectionType(ProjectionType type) { m_ProjectionType = type; RecalculateProjection(); }

	private:
		void RecalculateProjection();
	private:
		ProjectionType m_ProjectionType = ProjectionType::Orthographic;

		// Viewport
		float m_AspectRatio = 1.778f;
		// Orthographic
		float m_OrthographicSize = 10.0f;
		float m_Orthographic_NearClip = -1.0f;
		float m_Orthographic_FarClip = 1.0f;
		// Perspective
		float m_Perspective_FOV = glm::radians(45.0f);
		float m_Perspective_NearClip = 0.01f;
		float m_Perspective_FarClip = 1000.0f;

	};
}