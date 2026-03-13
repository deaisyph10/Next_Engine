#pragma once
#include <glm/glm.hpp>
#include <glm/fwd.hpp>

namespace Exodus
{
	class Camera
	{
	public:
		Camera(const glm::mat4& projection)
			: m_Projection(projection) {
		}
		const glm::mat4& GetProjection() const { return m_Projection; }
	protected:
		glm::mat4 m_Projection;
	};
}