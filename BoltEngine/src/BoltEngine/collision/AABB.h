#pragma once

#include <vector>
#include <glm/glm.hpp>
#include <glm/gtx/rotate_vector.hpp>

#include "BoltEngine/voxel/Types.h"

namespace Bolt
{
	class BOLT_API BlockPosList
	{
	private:
		int _size;
		BlockPos* _first;
	public:
		class BlockPosIter
		{
		public:
			typedef BlockPosIter SelfType;
			typedef BlockPos ValueType;
			typedef BlockPos& RefType;
			typedef BlockPos* PointerType;
			typedef std::forward_iterator_tag IteratorCategory;
			typedef int DifferenceType;

			BlockPosIter(PointerType ptr) : _ptr(ptr) {}
			SelfType operator++() { _ptr++; return *this; }
			SelfType operator++(int i) { _ptr++; return *this; }
			RefType operator*() { return *_ptr; }
			PointerType operator->() { return _ptr; }
			bool operator==(const SelfType& other) { return _ptr == other._ptr; }
			bool operator!=(const SelfType& other) { return _ptr != other._ptr; }

		private:
			PointerType _ptr;
		};

		BlockPosList(int size) : _size(size)
		{
			_first = (BlockPos*)malloc(sizeof(BlockPos) * _size);
		}

		~BlockPosList()
		{
			free(_first);
		}

		int size() const
		{
			return _size;
		}

		BlockPos& operator[](int index)
		{
			assert(index < _size); // TODO(Brendan): Better error checking!
			return _first[index];
		}

		BlockPosIter begin()
		{
			return BlockPosIter(_first);
		}

		BlockPosIter end()
		{
			return BlockPosIter(_first + _size);
		}

	};

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

				_containingPos = BlockPosList(((maxX - minX) * numIter) + ((maxY - minY) * numIter) + ((maxZ - minZ) * numIter));

				auto iter = _containingPos.begin();
				for (int x = minX; x <= maxX; x++)
				{
					for (int y = minY; y <= maxY; y++)
					{
						for (int z = minZ; z <= maxZ; z++)
						{
							BlockPos& pos = *iter;
							pos.x = x;
							pos.y = y;
							pos.z = z;
							if (iter != _containingPos.end()) iter++;
						}
					}
				}
			}

			return _containingPos;
		}
	};
}