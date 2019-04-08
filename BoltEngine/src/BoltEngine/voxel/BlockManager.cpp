#define STB_IMAGE_IMPLEMENTATION

#include "BlockManager.h"

#include "BoltEngine/logger/Logger.h"

namespace Bolt
{
	BlockManager& BlockManager::getInstance()
	{
		static BlockManager instance;
		return instance;
	}

	void BlockManager::loadBlockTextures(Block* block)
	{
		for (auto iter = block->_faceTextures.begin(); iter != block->_faceTextures.end(); iter++)
		{
			Texture::TextureArray::Key textureKey = _textureArray.getEntry(iter->second);

			if (iter->first == BLOCK_FACE_X_NEGATIVE)
				block->_texture[BLOCK_FACE_X_NEGATIVE] = textureKey;
			else if (iter->first == BLOCK_FACE_X_POSITIVE)
				block->_texture[BLOCK_FACE_X_POSITIVE] = textureKey;
			else if (iter->first == BLOCK_FACE_Y_NEGATIVE)
				block->_texture[BLOCK_FACE_Y_NEGATIVE] = textureKey;
			else if (iter->first == BLOCK_FACE_Y_POSITIVE)
				block->_texture[BLOCK_FACE_Y_POSITIVE] = textureKey;
			else if (iter->first == BLOCK_FACE_Z_NEGATIVE)
				block->_texture[BLOCK_FACE_Z_NEGATIVE] = textureKey;
			else if (iter->first == BLOCK_FACE_Z_POSITIVE)
				block->_texture[BLOCK_FACE_Z_POSITIVE] = textureKey;
			else if (iter->first == SIDES)
			{
				block->_texture[BLOCK_FACE_X_NEGATIVE] = textureKey;
				block->_texture[BLOCK_FACE_X_POSITIVE] = textureKey;
				block->_texture[BLOCK_FACE_Z_NEGATIVE] = textureKey;
				block->_texture[BLOCK_FACE_Z_POSITIVE] = textureKey;
			}
			else if (iter->first == ALL)
			{
				block->_texture[BLOCK_FACE_X_NEGATIVE] = textureKey;
				block->_texture[BLOCK_FACE_X_POSITIVE] = textureKey;
				block->_texture[BLOCK_FACE_Y_NEGATIVE] = textureKey;
				block->_texture[BLOCK_FACE_Y_POSITIVE] = textureKey;
				block->_texture[BLOCK_FACE_Z_NEGATIVE] = textureKey;
				block->_texture[BLOCK_FACE_Z_POSITIVE] = textureKey;
			}
		}
	}

	void BlockManager::requireTexture(std::string texture)
	{
		_neededTextures.push_back(texture);
	}

	void BlockManager::createTexture()
	{
		_textureArray = Texture::create2DTextureArray(16, 16, _neededTextures);
	}

	Block& BlockManager::registerBlock(Block* block)
	{
		BlockID blockID = _registeredBlocks.size();
		block->_id = blockID;
		loadBlockTextures(block);
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