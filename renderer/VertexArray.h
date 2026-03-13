#pragma once
#include <memory>
#include "Renderer/Buffer.h"

namespace Exodus
{
	class VertexArray
	{
	public:
		virtual ~VertexArray() {}
		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;
		virtual void AddVertexBuffer(const Exodus::Ref<VertexBuffer>& vertexBuffer) = 0;
		virtual void SetIndexBuffer(const Exodus::Ref<IndexBuffer>& indexBuffer) = 0;
		virtual const std::vector<Exodus::Ref<VertexBuffer>>& GetVertexBuffers() const = 0;
		virtual const Exodus::Ref<IndexBuffer>& GetIndexBuffer() const = 0;
		static Ref<VertexArray> Create();
	};
}