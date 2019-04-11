#include "World.h"

#include "BlockManager.h"

namespace Bolt
{
	ChunkPtr World::getChunkAt(ChunkPos pos)
	{
		if (_lastAccessedChunk != nullptr && _lastAccessedChunk->getPos() == pos)
			return _lastAccessedChunk;

		ChunkPtr foundChunk;
		auto findChunk = _chunkMap.find(pos.value);
		if (findChunk != _chunkMap.end())
		{
			foundChunk = findChunk->second->getChunkAt(pos.y);
		}
		else
		{
			ChunkColumn* chunkColumn = DBG_NEW ChunkColumn(this, pos.x, pos.z);
			foundChunk = chunkColumn->getChunkAt(pos.y);
			foundChunk->setContainingWorld(this);
			_lastAccessedChunk = foundChunk;
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