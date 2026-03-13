#pragma once
#include "RenderCommands.h"

#include "Camera.h"
#include "Shader.h"
#include <cstdint>
#include <memory>
#include "RendererAPI.h"
#include "VertexArray.h"
#include <glm/fwd.hpp>

namespace Exodus
{
	class Renderer
	{
	public:
		static void Init();
		static void Shutdown();
		static void OnWindowResize(uint32_t width, uint32_t height);
		static void BeginScene(OrthographicCamera& camera);
		static void EndScene();
		static void Submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vertexArray, const glm::mat4& transform = glm::mat4(1.0f));
		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
	private:
		struct ScreenData
		{
			glm::mat4 ViewProjectionMatix;
		};
		static ScreenData* m_ScreenData;
	};
}