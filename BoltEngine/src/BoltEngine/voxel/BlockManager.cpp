#include "BlockManager.h"

#include "BoltEngine/logger/Logger.h"

namespace Bolt
{
	template <class BlockType>
	BlockType& BlockManager::registerBlock()
	{
		BlockID blockID = _registeredBlocks.size();
		BlockType* blockInstance = new BlockType(blockID);
		_registeredBlocks.push_back(blockInstance);
		return *blockInstance;
	}

	Block& BlockManager::getBlock(BlockID id)
	{
		if (_registeredBlocks.size() > id)
			return (*_registeredBlocks.at(id));
		else
		{
			return (*_registeredBlocks.at(0));
			BOLT_ENGINE_ERROR("Cannot find block at id {}, max id is {}", id, _registeredBlocks.size());
		}
	}

	Block& BlockManager::getBlock(const std::string& name)
	{
		// TODO (Brendan): This needs to be implemented
		return (*_registeredBlocks.at(0));
	}
}