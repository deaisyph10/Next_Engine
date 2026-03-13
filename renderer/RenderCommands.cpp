#include "RenderCommands.h"
#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace Exodus
{
	RendererAPI* RenderCommand::s_RendererAPI = new OpenGLRendererAPI;

}