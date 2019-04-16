#pragma once

#include <vector>
#include <glm/glm.hpp>
#include <glm/gtx/rotate_vector.hpp>

#include "BoltEngine/render/DebugRenderer.h"
#include "BoltEngine/voxel/Types.h"

namespace Bolt
{
	class BOLT_API AABB
	{
		float _x = 0.0f;
		float _y = 0.0f;
		float _z = 0.0f;

		float _width;
		float _height;
		float _depth;

		bool _moved;

		BlockPosList _containingPos;
	public:
		AABB(float width, float height, float depth) : _width(width), _height(height), _depth(depth), _containingPos(0) {}

		inline void setPos(float x, float y, float z)
		{
			_x = x;
			_y = y;
			_z = z;

			_moved = true;
		}

		inline void setCenter(float x, float y, float z)
		{
			_x = x - (_width / 2.f);
			_y = y - (_height / 2.f);
			_z = z - (_depth / 2.f);

			_moved = true;
		}

		inline BlockPosList& getContainingBlockPos()
		{
			if (_moved)
			{
				bool xNeg = _x < 0;
				bool yNeg = _y < 0;
				bool zNeg = _z < 0;

				if (xNeg) _x = abs(_x);
				if (yNeg) _y = abs(_y);
				if (zNeg) _z = abs(_z);

				int minX = floorf(_x);
				int maxX = floorf(_x + _width);

				int minY = floorf(_y);
				int maxY = floor(_y + _height);

				int minZ = floorf(_z);
				int maxZ = floor(_z + _depth);

				int numIter = 3;
				if (maxX - minX == 0) numIter--;
				if (maxY - minY == 0) numIter--;
				if (maxZ - minZ == 0) numIter--;

				int size = ((maxX - minX) * numIter) + ((maxY - minY) * numIter) + ((maxZ - minZ) * numIter);

				_containingPos.clear();
				_containingPos.reserve(size);

				auto iter = _containingPos.begin();
				for (int x = minX; x <= maxX; x++)
				{
					for (int y = minY; y <= maxY; y++)
					{
						for (int z = minZ; z <= maxZ; z++)
						{
							_containingPos.emplace_back(xNeg ? -x : x, yNeg ? -y : y, zNeg ? -z : z);
						}
					}
				}

				_moved = false;
			}

			return _containingPos;
		}

		inline void debugDraw()
		{
			ddVec3 origin	= { _x, _y, _z };
			ddVec3 max		= { _x + _width, _y + _height, _z + _depth };

			dd::aabb(origin, max, dd::colors::Blue);
		}
	};
}