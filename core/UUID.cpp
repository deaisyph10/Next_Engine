#include "core/UUID.h"
#include <string>
#include <cstdlib>
#include <functional>
#include <cstdint>

#include <random>

namespace Exodus
{
	static std::hash<std::string> stringHasher;
	static std::hash<uint64_t> uint64Hasher;
	
	static std::random_device rd;
	static std::mt19937_64 s_engine(std::random_device{}());
	static std::uniform_int_distribution<uint64_t> s_distribution;


	UUID::UUID()
		:m_UUID(s_distribution(s_engine) )
	{
	}
	UUID::UUID(uint64_t uuid)
		: m_UUID(uuid)
	{
	}
}