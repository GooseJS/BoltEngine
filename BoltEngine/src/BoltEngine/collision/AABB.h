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
		float _depth;

	public:
		AABB(float width, float height, float depth) : _width(width), _height(height), _depth(depth) {}

		inline void setPos(float x, float y, float z)
		{
			_x = x;
			_y = y;
			_z = z;
		}

		inline std::vector<BlockPos> getContainingBlockPos()
		{
			bool xNeg = _x < 0;
			bool yNeg = _y < 0;
			bool zNeg = _z < 0;

			if (xNeg) _x = abs(_x);
			if (yNeg) _y = abs(_y);
			if (zNeg) _z = abs(_z);

			int minX = floorf(_x);
			int maxX = ceilf(_x + _width);

			int minY = floorf(_y);
			int maxY = ceilf(_y + _height);

			int minZ = floorf(_z);
			int maxZ = ceilf(_z + _depth);

			std::vector<BlockPos> retVal;

			for (int x = minX; x <= maxX; x++)
			{
				for (int y = minY; y <= maxY; y++)
				{
					for (int z = minZ; z <= maxZ; z++)
					{
						retVal.emplace_back(xNeg ? -x : x, yNeg ? -y : y, zNeg ? -z : z);
					}
				}
			}

			return retVal;
		}
	};
}