#pragma once

#include <cstdint>

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


	struct BlockPos
	{
		int x;
		int y;
		int z;

		BlockPos() : x(0), y(0), z(0) {}
		BlockPos(int _x, int _y, int _z) : x(_x), y(_y), z(_z) {}

		BlockPos(const struct ChunkBlockPos& blockPos);
	};

	struct ChunkPos
	{
		union
		{
			struct
			{
				int32_t x;
				int32_t z;
			};
			uint64_t value;
		};
		int32_t y;

		ChunkPos() : x(0), y(0), z(0) {}
		ChunkPos(uint64_t _value) : value(_value) { }
		ChunkPos(int _x, int _y, int _z) : x(_x), y(_y), z(_z) { }

		ChunkPos(const BlockPos& blockPos)
		{
			x = blockPos.x >> 4; // TODO (Brendan): This won't work if chunks aren't 16 x 16 x 16, btw
			y = blockPos.y >> 4;
			z = blockPos.z >> 4;
		}

        BlockPos toBlockPos(int x, int y, int z);
	};

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

		const int index()
		{
			return x | y << 4 | z << 8;
		}
	};
}