#pragma once

#include <xhash>
#include <functional>
#include <cstdint>

namespace Exodus
{
	class UUID
	{
	public:

		UUID();
		UUID(uint64_t uuid);

		operator uint64_t() const { return m_UUID; }

	private:
		uint64_t m_UUID;
	};
}

namespace std
{
	template<>
	struct hash<Exodus::UUID>
	{
		size_t operator()(const Exodus::UUID& uuid) const
		{
			return hash<uint64_t>()((uint64_t)uuid);
		}
	};
}
