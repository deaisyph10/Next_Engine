#include "Entity.h"


namespace Exodus
{
	Entity::Entity(entt::entity handle, Scene* scene)
		: m_EntityHandle(handle), m_Scene(scene)
	{
	}
	
} // namespace Exodus