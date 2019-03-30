#pragma once

#include <vector>

#include "BoltEngine/Core.h"
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

	class BOLT_API BlockManager
	{
	private:
		std::vector<Block*> _registeredBlocks;
	public:
		static BlockManager& getInstance();
		
		Block& registerBlock(Block* block);

		Block& getBlock(BlockID id);
		Block& getBlock(const std::string& name);

		template <class BlockType>
		inline BlockType& getBlockAs(BlockID id) { return dynamic_cast<BlockType>(getBlock(id)); }
		template <class BlockType>
		inline BlockType& getBlockAs(const std::string& name) { return dynamic_cast<BlockType>(getBlock(name)); }

		// TODO (Brendan): Is this needed?
		//template <class BlockType>
		//BlockType& getBlock();
	};
}