#pragma once

#include <vector>

#include "BoltEngine/Core.h"
#include "BoltEngine/voxel/Block.h"

#define BOLT_MAX_BLOCK_COUNT 256

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
		Block** _registeredBlocks;
		BlockID _lastRegisteredBlock = -1;

		std::vector<std::string> _neededTextures;
		Texture::TextureArray _textureArray;

		void loadBlockData(Block* block);

		BlockManager()
		{
			_registeredBlocks = DBG_NEW Block*[BOLT_MAX_BLOCK_COUNT];
		}
	public:
		~BlockManager()
		{
			for (int i = 0; i <= _lastRegisteredBlock; i++)
			{
				delete _registeredBlocks[i];
			}
			delete[] _registeredBlocks;
		}

		static BlockManager& getInstance();
		
		Block& registerBlock(const std::string& module, const std::string& name);
		void requireTexture(std::string texture);

		void createTexture();

		Block& getBlock(BlockID id);
		Block& getBlock(const std::string& name);

		//template <class BlockType>
		//inline BlockType& getBlockAs(BlockID id) { return dynamic_cast<BlockType>(getBlock(id)); }
		//template <class BlockType>
		//inline BlockType& getBlockAs(const std::string& name) { return dynamic_cast<BlockType>(getBlock(name)); }

		inline GLuint getTexture() { return _textureArray.textureID; }

		// TODO (Brendan): Is this needed?
		//template <class BlockType>
		//BlockType& getBlock();
	};
}