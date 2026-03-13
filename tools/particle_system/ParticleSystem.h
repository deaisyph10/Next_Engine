#pragma once
#include "core/TimeStep.h"
#include "renderer/Camera.h"
#include <glm/fwd.hpp>
#include <vector>
#include <cstdint>

struct ParticleProps
{
	glm::vec2 Position = glm::vec2(0.0f);
	glm::vec2 Velocity = glm::vec2(0.0f);
	glm::vec2 VelocityVariation = glm::vec2(0.0f);
	glm::vec4 ColorBegin = glm::vec4(1.0f);
	glm::vec4 ColorEnd = glm::vec4(1.0f);
	float SizeBegin = 1.0f;
	float SizeEnd = 1.0f;
	float SizeVariation = 0.0f;
	float LifeTime = 1.0f;
};

class ParticleSystem
{
public:
	ParticleSystem();

	void OnUpdate(Exodus::Timestep ts);
	void OnRender(Exodus::OrthographicCamera& camera);

	void Emit(const ParticleProps& particleProps);
private:
	struct Particle
	{
		glm::vec2 Position;
		glm::vec2 Velocity;
		glm::vec4 ColorBegin, ColorEnd;
		float Rotation = 0.0f;
		float SizeBegin, SizeEnd;

		float LifeTime = 1.0f;
		float LifeRemaining = 0.0f;

		bool Active = false;
	};
	std::vector<Particle> m_ParticlePool;
	uint32_t m_PoolIndex = 999;
};