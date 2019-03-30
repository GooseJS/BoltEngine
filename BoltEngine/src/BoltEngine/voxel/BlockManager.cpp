#include "BlockManager.h"

#include "BoltEngine/logger/Logger.h"

namespace Bolt
{
	BlockManager& BlockManager::getInstance()
	{
		static BlockManager instance;
		return instance;
	}

	Block& BlockManager::registerBlock(Block* block)
	{
		BlockID blockID = _registeredBlocks.size();
		block->_id = blockID;
		_registeredBlocks.push_back(block);
		return *block;
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