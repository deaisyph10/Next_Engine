#include "Renderer/FrameBuffer.h"
#include "Platform/OpenGL/OpenGLFrameBuffer.h"
#include "Renderer.h"

namespace Exodus {

	Ref<FrameBuffer> FrameBuffer::Create(const FrameBufferSpecification& spec)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:  EX_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL: return CreateRef<OpenGLFrameBuffer>(spec);
		}

		EX_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}