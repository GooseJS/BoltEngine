#include "Chunk.h"

#include "World.h"
#include "BlockManager.h"

namespace Bolt
{
	Block& ChunkStorage::getBlockAt(int index)
	{
		return BlockManager::getInstance().getBlock(_storage[index]);
	}

	Block& ChunkStorage::setBlockAt(int index, const Block& block)
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
		return _storage.getBlockAt(pos.index());
	}
	
	Block& Chunk::setBlockAt(ChunkBlockPos pos, const Block& block)
	{
		Block& prevBlock = _storage.setBlockAt(pos.index(), block);
		if (block.getID() != prevBlock.getID())
		{
			if (getMesh().initialized && !getMesh().needsRebuild)
			{
				getMesh().needsRebuild = true;
				_containingWorld->getChunksToRebuild().push_back(this);
			}
			_slices[Axis::X][pos.x].addBlock(block);
			_slices[Axis::Y][pos.y].addBlock(block);
			_slices[Axis::Z][pos.z].addBlock(block);
		}
		return prevBlock;
	}
}