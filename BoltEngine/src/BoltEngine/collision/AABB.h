#pragma once

#include <vector>
#include <glm/glm.hpp>
#include <glm/gtx/rotate_vector.hpp>

#include "BoltEngine/voxel/Types.h"

namespace Bolt
{
	class AABB
	{
		float _x = 0.0f;
		float _y = 0.0f;
		float _z = 0.0f;

		float _width;
		float _height;

		float _xRotation;

	public:
		AABB(float width, float height) : _width(width), _height(height) {}

		inline void setPos(float x, float y, float z)
		{
			_x = x;
			_y = y;
			_z = z;
		}

		//inline std::vector<BlockPos> getBlockPosBelow()
		//{
		//	glm::vec3 pos(_x, _y, _z);
		//
		//	glm::rotate()
		//}
	};
}