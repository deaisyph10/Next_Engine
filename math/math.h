#pragma once
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "core.h"

namespace Exodus {
	
	inline bool IsZero(float value, float epsilon = 1e-6f) {
		return std::fabs(value) < epsilon;
	}

	inline float Length(const glm::vec3& v) {
		return std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
	}

	inline bool DecomposeTransform(const glm::mat4& transform, glm::vec3& outTranslation, glm::vec3& outRotation, glm::vec3& outScale) {
		// Extract translation
		outTranslation = glm::vec3(transform[3]);
		// Extract scale
		outScale.x = Length(glm::vec3(transform[0]));
		outScale.y = Length(glm::vec3(transform[1]));
		outScale.z = Length(glm::vec3(transform[2]));
		if (IsZero(outScale.x) || IsZero(outScale.y) || IsZero(outScale.z)) {
			return false; // Cannot decompose if any scale component is zero
		}
		// Normalize the rotation part of the matrix
		glm::mat4 rotationMatrix = transform;
		rotationMatrix[0] /= outScale.x;
		rotationMatrix[1] /= outScale.y;
		rotationMatrix[2] /= outScale.z;
		// Extract rotation (assuming the rotation is in Euler angles)
		outRotation.y = std::atan2(rotationMatrix[0][2], rotationMatrix[0][0]); // Yaw
		float cosYaw = std::cos(outRotation.y);
		outRotation.x = std::atan2(-rotationMatrix[0][1], rotationMatrix[0][0] / cosYaw); // Pitch
		outRotation.z = std::atan2(-rotationMatrix[1][0], rotationMatrix[1][1]); // Roll
		return true;
	}
} // namespace Exodus