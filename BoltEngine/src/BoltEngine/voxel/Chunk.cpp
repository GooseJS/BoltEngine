#include "Chunk.h"

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
		return _storage[pos.y].getBlockAt(pos.index());
	}
	
	Block& Chunk::setBlockAt(ChunkBlockPos pos, Block& block)
	{
		return _storage[pos.y].setBlockAt(pos.index(), block);
	}
}