#include "OpenGLVertexArray.h"
#include "glad/glad.h"

namespace Exodus
{

	static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type)
	{
		switch (type)
		{
		case Exodus::ShaderDataType::Float:  return GL_FLOAT;
		case Exodus::ShaderDataType::Float2: return GL_FLOAT;
		case Exodus::ShaderDataType::Float3: return GL_FLOAT;
		case Exodus::ShaderDataType::Float4: return GL_FLOAT;
		case Exodus::ShaderDataType::Mat3:   return GL_FLOAT;
		case Exodus::ShaderDataType::Mat4:   return GL_FLOAT;
		case Exodus::ShaderDataType::Int:    return GL_INT;
		case Exodus::ShaderDataType::Int2:   return GL_INT;
		case Exodus::ShaderDataType::Int3:   return GL_INT;
		case Exodus::ShaderDataType::Int4:   return GL_INT;
		case Exodus::ShaderDataType::Bool:   return GL_BOOL;
		}
		EX_CORE_ASSERT(false, "Unknown ShaderDataType!");
		return 0;
	}
	OpenGLVertexArray::OpenGLVertexArray()
	{
		glCreateVertexArrays(1, &m_RendererID);
	}
	void OpenGLVertexArray::Bind() const
	{
		glBindVertexArray(m_RendererID);
	}
	void OpenGLVertexArray::Unbind() const
	{
		glBindVertexArray(0);
	}
	void OpenGLVertexArray::AddVertexBuffer(const Exodus::Ref<VertexBuffer>& vertexBuffer)
	{
		glBindVertexArray(m_RendererID);
		vertexBuffer->Bind();
		const auto& layout = vertexBuffer->GetLayout();
		for (const auto& element : layout)
		{
			switch (element.Type)
			{
			case Exodus::ShaderDataType::Float: 
			case Exodus::ShaderDataType::Float2:
			case Exodus::ShaderDataType::Float3:
			case Exodus::ShaderDataType::Float4:
			case Exodus::ShaderDataType::Mat3:  
			case Exodus::ShaderDataType::Mat4:  

				glEnableVertexAttribArray(m_VertexBufferIndex);
				glVertexAttribPointer(m_VertexBufferIndex,
					element.GetComponentCount(),
					ShaderDataTypeToOpenGLBaseType(element.Type),                   // , GL_FLOAT  //
					element.Normalized ? GL_TRUE : GL_FALSE,
					layout.GetStride(),
					(const void*)element.Offset);
				m_VertexBufferIndex++;
				break;
			case Exodus::ShaderDataType::Int:   
			case Exodus::ShaderDataType::Int2:  
			case Exodus::ShaderDataType::Int3:  
			case Exodus::ShaderDataType::Int4:  
			case Exodus::ShaderDataType::Bool:
				glEnableVertexAttribArray(m_VertexBufferIndex);
				glVertexAttribIPointer(m_VertexBufferIndex,
					element.GetComponentCount(),
					ShaderDataTypeToOpenGLBaseType(element.Type),                   // , GL_INT  //
					layout.GetStride(),
					(const void*)element.Offset);
				m_VertexBufferIndex++;
				break;
				
			default:
				EX_CORE_ASSERT(false, "Unknown ShaderDataType!");
			}
		}
		m_Vertex_Buffers.push_back(vertexBuffer);
	}
	void OpenGLVertexArray::SetIndexBuffer(const Exodus::Ref<IndexBuffer>& indexBuffer)
	{
		glBindVertexArray(m_RendererID);
		indexBuffer->Bind();
		m_IndexBuffers = indexBuffer;
	}
}