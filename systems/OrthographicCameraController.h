#pragma once
#include "Core.h"
#include "Core/Timestep.h"
#include "Event/Event.h"
#include "Event/ApplicationEvent.h"
#include "Renderer/Camera.h"
#include "Input.h"
#include "KeyCodes.h"
#include "Event/MouseEvent.h"
#include "MouseButtonCodes.h"
#include "glm/glm.hpp"
#include <glm/fwd.hpp>

namespace Exodus {

	struct OrthographicCameraBounds
	{
		float Left, Right;
		float Bottom, Top;

		float GetWidth() const { return Right - Left; }
		float GetHeight() const { return Top - Bottom; }
	};

	class EXODUS_API OrthographicCameraController
	{
	public:
		OrthographicCameraController(float aspectRatio, bool rotation = false);

		void OnUpdate(Timestep ts);
		void OnEvent(Event& e);

		OrthographicCamera& GetCamera() { return m_Camera; }
		const OrthographicCamera& GetCamera() const { return m_Camera; }

		void SetCameraPosition(const glm::vec3& position) { m_CameraPosition = position; CalculateView(); }
		void SetCameraRotation(float rotation) { m_CameraRotation = rotation; CalculateView(); }
		void SetCameraTranslationSpeed(float speed) { m_CameraTranslationSpeed = speed; }
		void SetCameraRotationSpeed(float speed) { m_CameraRotationSpeed = speed; }
		void SetRotation(bool rotation) { m_Rotation = rotation; }
		void SetAspectRatio(float aspectRatio) { m_AspectRatio = aspectRatio; Resize(m_Bounds.GetWidth(), m_Bounds.GetHeight()); }
		void SetBounds(const OrthographicCameraBounds& bounds) { m_Bounds = bounds; Resize(bounds.GetWidth(), bounds.GetHeight()); }
		void SetBounds(float left, float right, float bottom, float top) { m_Bounds = { left, right, bottom, top }; Resize(right - left, top - bottom); }
		void SetBounds(float width, float height) { m_Bounds = { -width * 0.5f, width * 0.5f, -height * 0.5f, height * 0.5f }; Resize(width, height); }
		void SetBounds(float aspectRatio) { m_Bounds = { -aspectRatio * m_ZoomLevel, aspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel }; Resize(m_Bounds.GetWidth(), m_Bounds.GetHeight()); }
		void ResetCamera() { m_CameraPosition = { 0.0f, 0.0f, 0.0f }; m_CameraRotation = 0.0f; CalculateView(); }
		void ResetZoom() { m_ZoomLevel = 2.0f; CalculateView(); }
		void ResetBounds() { m_Bounds = { -m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel }; CalculateView(); }
		void ResetAspectRatio() { m_AspectRatio = 16.0f / 9.0f; Resize(m_Bounds.GetWidth(), m_Bounds.GetHeight()); }
		void ResetRotation() { m_Rotation = false; m_CameraRotation = 0.0f; CalculateView(); }
		void ResetAll() { ResetCamera(); ResetZoom(); ResetBounds(); ResetAspectRatio(); ResetRotation(); }
		void MoveCamera(const glm::vec3& translation) { m_CameraPosition += translation; CalculateView(); }
		void RotateCamera(float rotation) { m_CameraRotation += rotation; CalculateView(); }
		void ZoomCamera(float zoom) { m_ZoomLevel += zoom; CalculateView(); }
		void SetCamera(const OrthographicCamera& camera) { m_Camera = camera; CalculateView(); }
		void Resize(float width, float height);
		float GetZoomLevel() const { return m_ZoomLevel; }
		void SetZoomLevel(float level) { m_ZoomLevel = level; CalculateView(); }
		const OrthographicCameraBounds& GetBounds() const { return m_Bounds; }
	private:
		void CalculateView();
		bool OnMouseScrolled(MouseScrolledEvent& e);
		bool OnWindowResized(WindowResizeEvent& e);
	private:
		float m_AspectRatio;
		float m_ZoomLevel = 2.0f;
		OrthographicCameraBounds m_Bounds;
		OrthographicCamera m_Camera;
		bool m_Rotation;
		glm::vec3 m_CameraPosition = { 0.0f, 0.0f, 0.0f };
		float m_CameraRotation = 0.0f;
		float m_CameraTranslationSpeed = 5.0f;
		float m_CameraRotationSpeed = 180.0f;
		float m_CameraMoveSpeed = 5.0f;
		float m_CameraZoomSpeed = 0.25f;
		float m_CameraRotationMoveSpeed = 180.0f;
		float m_CameraZoomMoveSpeed = 0.25f;
		float m_CameraRotationZoomSpeed = 180.0f;
		float m_CameraTranslationZoomSpeed = 5.0f;
		float m_CameraRotationTranslationSpeed = 180.0f;
		float m_CameraZoomTranslationSpeed = 5.0f;
		float m_CameraRotationZoomTranslationSpeed = 180.0f;
		float m_CameraTranslationRotationSpeed = 5.0f;
		float m_CameraZoomRotationSpeed = 5.0f;
	};
}
