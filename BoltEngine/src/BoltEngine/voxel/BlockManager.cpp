#define STB_IMAGE_IMPLEMENTATION

#include <fstream>
#include <nlohmann/json.hpp>

#include "BlockManager.h"

#include "BoltEngine/logger/Logger.h"

namespace Bolt
{
	BlockManager& BlockManager::getInstance()
	{
		static BlockManager instance;
		return instance;
	}

	void BlockManager::loadBlockData(Block* block)
	{
		using nlohmann::json;

		std::ifstream blockIFStream("blocks/" + block->getModule() + "/" + block->getName() + ".json");
		json file;
		if (!blockIFStream.is_open())
			return;

		blockIFStream >> file;

		if (file.find("blockData") == file.end())
			return;

		json blockData = file["blockData"];

		for (auto&[blockDataKey, blockDataValue] : blockData.items())
		{
			if (blockDataKey == "textureLayers")
			{
				json textureLayers = blockDataValue;
				std::string nx = "nullptr";
				std::string px = "nullptr";
				std::string ny = "nullptr";
				std::string py = "nullptr";
				std::string nz = "nullptr";
				std::string pz = "nullptr";

				for (auto&[textureKey, textureValue] : textureLayers.items())
				{
					if (textureKey == "nx")
						nx = textureValue;
					else if (textureKey == "px")
						px = textureValue;
					else if (textureKey == "ny" || textureKey == "bottom")
						ny = textureValue;
					else if (textureKey == "py" || textureKey == "top")
						py = textureValue;
					else if (textureKey == "nz")
						nz = textureValue;
					else if (textureKey == "pz")
						pz = textureValue;
					else if (textureKey == "sides")
					{
						nx = textureValue;
						px = textureValue;
						nz = textureValue;
						pz = textureValue;
					}
					else if (textureKey == "all")
					{
						nx = textureValue;
						px = textureValue;
						ny = textureValue;
						py = textureValue;
						nz = textureValue;
						pz = textureValue;
					}
				}

				block->_texture[BLOCK_FACE_X_NEGATIVE] = _textureArray.getEntry(nx);
				block->_texture[BLOCK_FACE_X_POSITIVE] = _textureArray.getEntry(px);
				block->_texture[BLOCK_FACE_Y_NEGATIVE] = _textureArray.getEntry(ny);
				block->_texture[BLOCK_FACE_Y_POSITIVE] = _textureArray.getEntry(py);
				block->_texture[BLOCK_FACE_Z_NEGATIVE] = _textureArray.getEntry(nz);
				block->_texture[BLOCK_FACE_Z_POSITIVE] = _textureArray.getEntry(pz);
			}
			else if (blockDataKey == "shouldRender")
				block->_data.shouldRender = blockDataValue;
			else if (blockDataKey == "transparency")
				block->_data.transparency = blockDataValue;
			else if (blockDataKey == "emitLightLevel")
				block->_data.emitLightLevel = blockDataValue;
			else if (blockDataKey == "hasGravity")
				block->_data.hasGravity = blockDataValue;
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

	Block& BlockManager::registerBlock(const std::string& module, const std::string& name)
	{
		BlockID blockID = _registeredBlocks.size();
		_registeredBlocks.emplace_back(new Block(blockID, module, name));
		Block& block = *_registeredBlocks.at(blockID);
		loadBlockData(&block);
		return block;
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