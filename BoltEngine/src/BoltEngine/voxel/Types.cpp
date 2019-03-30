#include "Types.h"

namespace Bolt
{
	BlockPos::BlockPos(const ChunkBlockPos& blockPos)
	{
		this->x = (BOLT_CHUNK_WIDTH * blockPos.chunkPos.x) + blockPos.x;
		this->y = (BOLT_CHUNK_WIDTH * blockPos.chunkPos.y) + blockPos.y;
		this->z = (BOLT_CHUNK_WIDTH * blockPos.chunkPos.z) + blockPos.z;
	}
}