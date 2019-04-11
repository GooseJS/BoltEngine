#include "World.h"

#include "BlockManager.h"

namespace Bolt
{
	ChunkPtr World::getChunkAt(ChunkPos pos)
	{
		ChunkPtr foundChunk;
		if (_chunkMap.find(pos.value) != _chunkMap.end())
		{
			foundChunk = _chunkMap.at(pos.value)->getChunkAt(pos.y);
		}
		else
		{
			ChunkColumn* chunkColumn = new ChunkColumn(this, pos.x, pos.z);
			foundChunk = chunkColumn->getChunkAt(pos.y);
			foundChunk->setContainingWorld(this);
			_chunkMap.insert(std::make_pair(pos.value, chunkColumn));
		}
		return foundChunk;
	}

	Block& World::getBlockAt(BlockPos pos)
	{
		if (pos.y < 0) return BlockManager::getInstance().getBlock(0);
		return getChunkAt(pos)->getBlockAt(pos);
	}

	Block& World::setBlockAt(BlockPos pos, Block& block)
	{
		if (pos.y < 0) return BlockManager::getInstance().getBlock(0);
		return getChunkAt(pos)->setBlockAt(pos, block);
	}
}