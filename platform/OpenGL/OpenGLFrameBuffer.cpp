#include "OpenGLFrameBuffer.h"
#include "Log.h"
#include <glad/glad.h>
#include <vector>
#include "core.h"
#include "Renderer/FrameBuffer.h"
#include <cstdint>


namespace Exodus
{
	static const uint32_t s_MaxFrameBufferSize = 8192;
	
	namespace Utils
	{
		static GLenum TextureTarget(bool multiSampled)
		{
			return multiSampled ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
		}

		static void CreateTextures(bool multiSampled, uint32_t* outID, uint32_t count)
		{
			glCreateTextures(multiSampled ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D, count, outID);
		}

		static void BindTexture(bool multiSampled, uint32_t id)
		{
			glBindTexture(multiSampled ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D, id);
		}

		static bool IsDepthFormat(FrameBufferTextureFormat format)
		{
			switch (format)
			{
				case Exodus::FrameBufferTextureFormat::DEPTH24STENCIL8: return true;
			}
			return false;
		}
		static void AttachColorTexture(uint32_t id, int samples, GLenum interalformat, GLenum format, uint32_t width, uint32_t hieght, int index)
		{
			bool multiSampled = samples > 1;
			if (multiSampled)
			{
				glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, interalformat, width, hieght, GL_FALSE);
			}
			else
			{
				glTexImage2D(GL_TEXTURE_2D, 0, interalformat, width, hieght, 0, format, GL_UNSIGNED_BYTE, nullptr);
				
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
				glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
			}
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, TextureTarget(multiSampled), id, 0);
		}

		static void AttachDepthTexture(uint32_t id, int samples, FrameBufferTextureFormat format, uint32_t width, uint32_t height)
		{
			bool multiSampled = samples > 1;
			if (multiSampled)
			{
				glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, GL_DEPTH24_STENCIL8, width, height, GL_FALSE);
			}
			else
			{
				glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, width, height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, nullptr);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
				glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
			}
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, TextureTarget(multiSampled), id, 0);
		}

		static GLenum FBTextureFormatToGL(FrameBufferTextureFormat format)
		{
			switch (format)
			{
				case FrameBufferTextureFormat::RGBA8: return GL_RGBA8;
				case FrameBufferTextureFormat::RED_INTEGER: return GL_RED_INTEGER;
			}
			EX_CORE_ASSERT(false, "Unknown FrameBufferTextureFormat!");
			return 0;
		}

		static GLenum GLDataType(FrameBufferTextureFormat format)  //internal format for glTexImage2D
		{
			switch (format)
			{
				case FrameBufferTextureFormat::RGBA8: return GL_RGBA;
				case FrameBufferTextureFormat::RED_INTEGER: return GL_RED_INTEGER;
			}
			EX_CORE_ASSERT(false, "Unknown FrameBufferTextureFormat!");
			return 0;
		}

	} //end namespace Utils
	
	OpenGLFrameBuffer::OpenGLFrameBuffer(const FrameBufferSpecification& spec)
		: m_Specification(spec)
	{
		for (auto& spec : m_Specification.Attachments.Attachments)
		{
			if (!Utils::IsDepthFormat(spec.TextureFormat))
				m_ColorAttachmentSpecifications.emplace_back(spec);
			else
				m_DepthAttachmentSpecifications = spec.TextureFormat;
		}
		Invalidate();
	}

	OpenGLFrameBuffer::~OpenGLFrameBuffer()
	{
		glDeleteFramebuffers(1, &m_RendererID);
		if (!m_ColorAttachments.empty())
			glDeleteTextures((GLsizei)m_ColorAttachments.size(), m_ColorAttachments.data());
		glDeleteTextures(1, &m_DepthAttachmentFormat);
	}

	//-----------------------------------------------------------INVALIDATE--------------------------------------------------------------

	void OpenGLFrameBuffer::Invalidate()
	{
		bool multiSampled = m_Specification.Samples > 1;
		
		if (m_RendererID)
		{
			glDeleteFramebuffers(1, &m_RendererID);
			if (!m_ColorAttachments.empty())
				glDeleteTextures((GLsizei)m_ColorAttachments.size(), m_ColorAttachments.data());
			glDeleteTextures(1, &m_DepthAttachmentFormat);
			m_ColorAttachments.clear();
			m_DepthAttachmentFormat = 0;
		}
		glGenFramebuffers(1, &m_RendererID);
		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);


	//--------------------------------------------Color Attachments-----------------------------
		if (m_ColorAttachmentSpecifications.size()) 
		{
			m_ColorAttachments.resize(m_ColorAttachmentSpecifications.size());
			Utils::CreateTextures(multiSampled, m_ColorAttachments.data(), (uint32_t)m_ColorAttachments.size());

			for (size_t i = 0; i < m_ColorAttachmentSpecifications.size(); i++) {
				
				Utils::BindTexture(multiSampled, m_ColorAttachments[i]);
				switch (m_ColorAttachmentSpecifications[i].TextureFormat)
				{
						case FrameBufferTextureFormat::RGBA8:
						Utils::AttachColorTexture(m_ColorAttachments[i], m_Specification.Samples, GL_RGBA8,
												  GL_RGBA, m_Specification.Width, m_Specification.Height, (int)i);
						break;
						case FrameBufferTextureFormat::RED_INTEGER:
							Utils::AttachColorTexture(m_ColorAttachments[i], m_Specification.Samples, GL_R32I,
								GL_RED_INTEGER, m_Specification.Width, m_Specification.Height, (int)i);
						break;
				}
			}
		}

	//-------------------------------------------Depth Attachment----------------------------------
		if (m_DepthAttachmentSpecifications != FrameBufferTextureFormat::None)
		{
			Utils::CreateTextures(multiSampled, &m_DepthAttachmentFormat, 1);
			Utils::BindTexture(multiSampled, m_DepthAttachmentFormat);
			switch (m_DepthAttachmentSpecifications)
			{
				case FrameBufferTextureFormat::DEPTH24STENCIL8:
				{
					Utils::AttachDepthTexture(m_DepthAttachmentFormat, m_Specification.Samples, FrameBufferTextureFormat::DEPTH24STENCIL8, m_Specification.Width, m_Specification.Height);
					break;
				}
			}//switch statement
		}//depth attachment

		if (m_ColorAttachments.size() > 1)
			{
				EX_CORE_ASSERT(m_ColorAttachments.size() <= 4, "Exodus only supports up to 4 color attachments");
				std::vector<GLenum> buffers(m_ColorAttachments.size());
				for (size_t i = 0; i < m_ColorAttachments.size(); i++)
					buffers[i] = GL_COLOR_ATTACHMENT0 + (GLenum)i;
				glDrawBuffers((GLsizei)m_ColorAttachments.size(), buffers.data());
			}
		else if (m_ColorAttachments.empty())
			{
				glDrawBuffer(GL_NONE);
			}

		EX_CORE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is incomplete!");
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
	//------------------------------------------------END INVALIDATE-----------------------------------------------------------------


	int OpenGLFrameBuffer::ReadPixel(uint32_t attachmentIndex, int x, int y)
	{
		EX_CORE_ASSERT(attachmentIndex < m_ColorAttachments.size(), "Attachment index out of bounds");
		glReadBuffer(GL_COLOR_ATTACHMENT0 + attachmentIndex);
		int pixelData;
		glReadPixels(x, y, 1, 1, GL_RED_INTEGER, GL_INT, &pixelData);
		return pixelData;
	}

	void OpenGLFrameBuffer::ClearAttachment(uint32_t attachmentIndex, int value)
	{
		EX_CORE_ASSERT(attachmentIndex < m_ColorAttachments.size(), "Attachment index out of bounds");
		auto& spec = m_ColorAttachmentSpecifications[attachmentIndex];
		GLenum format = Utils::FBTextureFormatToGL(spec.TextureFormat);
		glClearTexImage(m_ColorAttachments[attachmentIndex], 0, format, GL_INT, &value);
	}

	void OpenGLFrameBuffer::Bind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
		glViewport(0, 0, m_Specification.Width, m_Specification.Height);
	}

	void OpenGLFrameBuffer::Unbind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void OpenGLFrameBuffer::Resize(uint32_t width, uint32_t height)
	{
		if (width == 0 || height == 0 || width > 8192 || height > 8192)
		{
			EX_CORE_WARN("Attempted to resize framebuffer to {0}, {1}", width, height);
			return;
		}
		m_Specification.Width = width;
		m_Specification.Height = height;
		Invalidate();
	}


}
