#pragma once

#include <vector>

#include "BoltEngine/voxel/Block.h"

namespace Bolt
{
	// TODO (Brendan): Block manager should completely handle the lifecycle of blocks in memory.
	// They should be created in here, and destroyed in here
	// They will be passed in as templates, before they are essentially translated into their ID's
	// Blocks will be stored as a single reference, with ID's to tell what reference a block is referring to 
	// Blocks should ONLY take in a BlockID parameter, as it is assigned on startup of the game.
	// This could potentially cause issues with saves, but is only temporary
	// Blocks should override the functions defined in the Block class to add functionality (see Bolt::Voxel::Block)

	class BlockManager
	{
	private:
		std::vector<Block*> _registeredBlocks;
	public:
		static BlockManager& getInstance()
		{
			static BlockManager instance;
			return instance;
		}
		
		template <class BlockType>
		BlockType& registerBlock();

		Block& getBlock(BlockID id);
		Block& getBlock(const std::string& name);

		// TODO (Brendan): Is this needed?
		//template <class BlockType>
		//BlockType& getBlock();
	};
}