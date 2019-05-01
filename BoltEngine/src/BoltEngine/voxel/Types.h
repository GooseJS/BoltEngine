#pragma once

#include "BoltEngine/Core.h"
#include <cstdint>
#include <vector>

#ifndef BOLT_CHUNK_WIDTH
#define BOLT_CHUNK_WIDTH 16
#endif

#define BOLT_CHUNK_AREA		BOLT_CHUNK_WIDTH * BOLT_CHUNK_WIDTH
#define BOLT_CHUNK_VOLUME	BOLT_CHUNK_AREA  * BOLT_CHUNK_WIDTH

#ifndef BOLT_BLOCK_WIDTH
#define BOLT_BLOCK_WIDTH 16
#endif

#ifndef BOLT_WORLD_HEIGHT
#define BOLT_WORLD_HEIGHT 16
#endif

namespace Bolt
{
	/// A struct to hold an XYZ int value, essentially a voxel position
	struct BlockPos
	{
		int x;
		int y;
		int z;

		BlockPos() : x(0), y(0), z(0) {}
		BlockPos(int _x, int _y, int _z) : x(_x), y(_y), z(_z) {}

		BlockPos(const struct ChunkBlockPos& blockPos);
	};

	/// A struct to hold a chunk X and Z, as well as a Y. They are seperated as the X and Z are used for hashing and lookup, while the Y is
	/// used to get the actual chunk from the ChunkColumn
	struct ChunkPos
	{
		union
		{
			struct
			{
				int32_t x; // X takes up 32-bits
				int32_t z; // Z takes up 32-bits
			};
			uint64_t value; // 64-bit representation of X and Z for hashing
		};
		int32_t y;

		ChunkPos() : x(0), y(0), z(0) {}
		ChunkPos(uint64_t _value) : value(_value) { }
		ChunkPos(int _x, int _y, int _z) : x(_x), y(_y), z(_z) { }

		/// Turns a block pos into a chunk pos. This is done by bitshifting the X Y and Z to only get the last 4 bits (16 values for a 16x16x16 chunk)
		ChunkPos(const BlockPos& blockPos)
		{
			x = blockPos.x >> 4; // TODO (Brendan): This won't work if chunks aren't 16 x 16 x 16, btw
			y = blockPos.y >> 4;
			z = blockPos.z >> 4;
		}

		bool operator==(ChunkPos& pos)
		{
			return x == pos.x && y == pos.y && z == pos.z;
		}

		/// Turns a ChunkPos into a block pos, but can also apply an offset to get to a specific blockpos in a chunk
        BlockPos toBlockPos(int x, int y, int z);
	};

	/// A ChunkBlockPos is a ChunkPos with an offset to hold specific blocks in specific chunks (i.e, the block at 4x4x4 in each chunk)
	struct ChunkBlockPos
	{
		int x;
		int y;
		int z;

		ChunkPos chunkPos;

		ChunkBlockPos() : x(0), y(0), z(0) {}
		ChunkBlockPos(int _x, int _y, int _z, ChunkPos _chunkPos) : x(_x), y(_y), z(_z), chunkPos(_chunkPos) {}

		ChunkBlockPos(const BlockPos& blockPos)
		{
			x = blockPos.x & BOLT_CHUNK_WIDTH - 1;
			y = blockPos.y & BOLT_CHUNK_WIDTH - 1;
			z = blockPos.z & BOLT_CHUNK_WIDTH - 1;
			chunkPos = ChunkPos(blockPos);
		}

		// Hashing the 3 x,y, and z values into a single into for easy lookup (using 16x16x16 chunks this will never be higher than 4096)
		const int index()
		{
			return x | y << 4 | z << 8;
		}
	};

	typedef std::vector<BlockPos> BlockPosList;
}