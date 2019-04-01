#include "World.h"

namespace Bolt
{
	ChunkPtr ChunkProvider::getChunkAt(ChunkPos pos)
	{
		pos.y = 0;
		ChunkPtr foundChunk;
		if (_chunkMap.find(pos.value) != _chunkMap.end())
		{
			foundChunk = _chunkMap.at(pos.value);
		}
		else
		{
			foundChunk = new Chunk(pos);
			_chunkMap.insert(std::make_pair(pos.value, foundChunk));
		}
		return foundChunk;
	}

	Block& World::getBlockAt(BlockPos pos)
	{
		return _chunkProvider.getChunkAt(pos)->getBlockAt(pos);
	}

	Block& World::setBlockAt(BlockPos pos, Block& block)
	{
		return _chunkProvider.getChunkAt(pos)->setBlockAt(pos, block);
	}
}