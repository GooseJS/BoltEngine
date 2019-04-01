#include "Types.h"

namespace Bolt
{
	BlockPos::BlockPos(const ChunkBlockPos& blockPos)
	{
		this->x = (BOLT_CHUNK_WIDTH * blockPos.chunkPos.x) + blockPos.x;
		this->y = (BOLT_CHUNK_WIDTH * blockPos.chunkPos.y) + blockPos.y;
		this->z = (BOLT_CHUNK_WIDTH * blockPos.chunkPos.z) + blockPos.z;
	}

    BlockPos ChunkPos::toBlockPos(int _x, int _y, int _z)
    {
        BlockPos retVal;
        retVal.x = (x * BOLT_CHUNK_WIDTH) + _x;
        retVal.y = (y * BOLT_CHUNK_WIDTH) + _y;
        retVal.z = (z * BOLT_CHUNK_WIDTH) + _z;
    }
}