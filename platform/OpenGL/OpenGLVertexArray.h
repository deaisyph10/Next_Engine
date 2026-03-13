#pragma once
#include "Renderer/VertexArray.h"
#include <vector>
#include "glad/glad.h"



namespace Exodus
{
	class OpenGLVertexArray :public VertexArray
	{
	public:
		OpenGLVertexArray();
		virtual ~OpenGLVertexArray() {}
		virtual void Bind() const override;
		virtual void Unbind() const override;
		virtual void AddVertexBuffer(const Exodus::Ref<VertexBuffer>& vertexBuffer)override;
		virtual void SetIndexBuffer(const Exodus::Ref<IndexBuffer>& indexBuffer)override;
		virtual const std::vector<Exodus::Ref<VertexBuffer>>& GetVertexBuffers() const
		{
			return m_Vertex_Buffers;
		}
		virtual const Exodus::Ref<IndexBuffer>& GetIndexBuffer() const
		{
			return m_IndexBuffers;
		}
	private:
		uint32_t m_RendererID;
		uint32_t m_VertexBufferIndex = 0;
		std::vector<Exodus::Ref<VertexBuffer>> m_Vertex_Buffers;
		Exodus::Ref<IndexBuffer> m_IndexBuffers;
		
	};
}