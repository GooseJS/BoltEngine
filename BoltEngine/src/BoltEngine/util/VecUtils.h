#pragma once

#include <algorithm>
#include <glm/glm.hpp>

#include "BoltEngine/Core.h"
#include "BoltEngine/logger/Logger.h"

namespace Bolt
{
	glm::vec3 clampVec3(const glm::vec3& value, const glm::vec3& min, const glm::vec3& max)
	{
		return glm::vec3(std::clamp(value.x, min.x, max.x), std::clamp(value.y, min.y, max.y), std::clamp(value.z, min.z, max.z));
	}
}