#pragma once

#include "BoltEngine/render/Texture.h"

namespace Bolt
{
	typedef uint16_t BlockID;

	enum BoltBlockFace
	{
		BLOCK_FACE_X_POSITIVE	= 0,
		BLOCK_FACE_X_NEGATIVE	= 1,
		BLOCK_FACE_Y_POSITIVE	= 2,
		BLOCK_FACE_Y_NEGATIVE	= 3,
		BLOCK_FACE_Z_POSITIVE	= 4,
		BLOCK_FACE_Z_NEGATIVE	= 5,
		SIZE					= 6,
		SIDES					= 7,
		ALL						= 8
	};

	struct BlockData
	{
		bool transparency = false;
		bool shouldRender = false;

		int emitLightLevel = 0;

		bool hasGravity = false;
	};

	class Block
	{
	private:
		friend class BlockManager;
		
		BlockID _id;
		std::string _module = "bolt";
		std::string _name = "nullptr";

		BlockData _data;
		Texture::TextureArray::Key _texture[BoltBlockFace::SIZE];
	public:
		Block(BlockID id, std::string module, std::string name) : _id(id), _module(module), _name(name) {} // TODO(Brendan): I don't want module to be a string, but I can't think of any other good way to represent it right now

		inline operator BlockID() { getID(); }

		const BlockID getID() const { return _id; }
		const std::string getName() const { return _name; };
		const std::string getModule() const { return _module; }
		const std::string getFullName() const { return _module + "::" + _name; }

		const Texture::TextureArray::Key getTextureKey(BoltBlockFace face) const { return _texture[face]; }

		const bool hasTransparency() const { return _data.transparency; }
		const bool shouldRender() const { return _data.shouldRender; }
		const bool hasGravity() const { return _data.hasGravity; }
		const int emitLightLevel() const { return _data.emitLightLevel; }
	};
}