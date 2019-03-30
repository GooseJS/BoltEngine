#include "Chunk.h"

#include "BlockManager.h"

namespace Bolt
{
	Chunk::Chunk(ChunkPos pos) : _pos(pos)
	{

	}

	Block& Chunk::getBlockAt(int index)
	{
		return BlockManager::getInstance().getBlock(_storage[index]);
	}

	Block& Chunk::setBlockAt(int index, Block& block)
	{
		Block& prevBlock = BlockManager::getInstance().getBlock(_storage[index]);
		_storage[index] = block.getID();
		return prevBlock;
	}

	Block& Chunk::getBlockAt(ChunkBlockPos pos)
	{
		return getBlockAt(pos.index());
	}
	
	Block& Chunk::setBlockAt(ChunkBlockPos pos, Block& block)
	{
		return setBlockAt(pos.index(), block);
	}
}