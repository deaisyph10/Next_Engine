#include "Renderer.h"
#include "Platform/OpenGL//OpenGLShader.h"
#include "Renderer/Renderer2D.h"
#include "RenderCommands.h"
#include "Camera.h"
#include <memory>
#include "Shader.h"
#include "VertexArray.h"
#include <cstdint>
#include <glm/fwd.hpp>


namespace Exodus
{
	Renderer::ScreenData* Renderer::m_ScreenData = new Renderer::ScreenData;


	void Renderer::Init()
	{
		RenderCommand::Init();
		Renderer2D::Init();
	}

	void Renderer::Shutdown()
	{
		Renderer2D::Shutdown();
		delete m_ScreenData;
	}

	void Renderer::OnWindowResize(uint32_t width, uint32_t height)
	{
		RenderCommand::SetViewport(0, 0, width, height);
	}
	void Renderer::BeginScene(OrthographicCamera& camera)
	{
		m_ScreenData->ViewProjectionMatix = camera.GetViewProjectionMatrix();
	}
	void Renderer::EndScene()
	{
	}
	void Renderer::Submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vertexArray, const glm::mat4& transform)
	{

		shader->Bind();
		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("u_ViewProjection", m_ScreenData->ViewProjectionMatix);
		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("u_Transform", transform);

		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray, vertexArray->GetIndexBuffer()->GetCount());
	}
}
