#include "Chunk.h"

#include "World.h"
#include "BlockManager.h"

namespace Bolt
{
	Block& ChunkStorage::getBlockAt(int index)
	{
		return BlockManager::getInstance().getBlock(_storage[index]);
	}

	Block& ChunkStorage::setBlockAt(int index, Block& block)
	{
		Block& prevBlock = BlockManager::getInstance().getBlock(_storage[index]);
		_storage[index] = block.getID();
		return prevBlock;
	}

	Chunk::Chunk(ChunkPos pos) : _pos(pos)
	{

	}

	Block& Chunk::getBlockAt(ChunkBlockPos pos)
	{
		return _storage[pos.chunkPos.y].getBlockAt(pos.index());
	}
	
	Block& Chunk::setBlockAt(ChunkBlockPos pos, Block& block)
	{
		Block& prevBlock = _storage[pos.chunkPos.y].setBlockAt(pos.index(), block);
		if (block.getID() != prevBlock.getID())
		{
			if (getMesh(pos.chunkPos.y).initialized)
			{
				getMesh(pos.chunkPos.y).needsRebuild = true;
				_containingWorld->getChunksToRebuild().push_back(this);
			}
		}
		return prevBlock;
	}
}