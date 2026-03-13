#include "Scene/SceneCamera.h"
#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>

namespace Exodus {

	SceneCamera::SceneCamera()
		: Camera(glm::mat4(1.0f))
	{
		RecalculateProjection();
	}
	void SceneCamera::SetOrthographic(float size, float nearClip, float farClip)
	{
		m_ProjectionType = ProjectionType::Orthographic;
		m_OrthographicSize = size;
		m_Orthographic_NearClip = nearClip;
		m_Orthographic_FarClip = farClip;
		RecalculateProjection();
	}
	void SceneCamera::SetPerspective(float fov, float nearClip, float farClip)
	{
		m_ProjectionType = ProjectionType::Perspective;
		m_Perspective_FOV = fov;
		m_Perspective_NearClip = nearClip;
		m_Perspective_FarClip = farClip;
		RecalculateProjection();
	}

	void SceneCamera::SetViewportSize(float width, float height)
	{
		float m_AspectRatio = (float)width / (float)height;
		RecalculateProjection();
	}
	void SceneCamera::RecalculateProjection()
	{
		if (m_ProjectionType == ProjectionType::Perspective)
		{
			m_Projection = glm::perspective(m_Perspective_FOV, m_AspectRatio,
				m_Perspective_NearClip, m_Perspective_FarClip);
		}
		else
		{
			float left = -m_OrthographicSize * m_AspectRatio * 0.5f;
			float right = m_OrthographicSize * m_AspectRatio * 0.5f;
			float bottom = -m_OrthographicSize * 0.5f;
			float top = m_OrthographicSize * 0.5f;
			m_Projection = glm::ortho(left, right, bottom, top,
				m_Orthographic_NearClip, m_Orthographic_FarClip);
		}

	}
}