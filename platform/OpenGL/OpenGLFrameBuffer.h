#pragma once
#include "Core.h"
#include "Renderer/FrameBuffer.h"
#include <vector>
#include <glad/glad.h>


namespace Exodus {
	class OpenGLFrameBuffer : public FrameBuffer
	{
	public:
		OpenGLFrameBuffer(const FrameBufferSpecification& spec);
		virtual ~OpenGLFrameBuffer();

		virtual void Bind() override;
		virtual void Unbind() override;

		virtual void Resize(uint32_t width, uint32_t height) override;

		virtual void Invalidate();

		virtual int ReadPixel(uint32_t attachmentIndex, int x, int y) override;

		virtual void ClearAttachment(uint32_t attachmentIndex, int value) override;

		virtual uint32_t GetColorAttachmentRendererID(uint32_t index = 0)const override
		{
			EX_CORE_ASSERT(index < m_ColorAttachments.size());
			return m_ColorAttachments[index]; 
		}
		
		virtual const FrameBufferSpecification& GetSpecification() const override { return m_Specification; }

	private:
		FrameBufferSpecification m_Specification;
		uint32_t m_RendererID = 0;
		std::vector<FrameBufferTextureSpecification> m_ColorAttachmentSpecifications;
		FrameBufferTextureFormat m_DepthAttachmentSpecifications = FrameBufferTextureFormat::None;

		std::vector<GLenum> m_ColorAttachments;
		uint32_t m_DepthAttachmentFormat = 0;
	};
}