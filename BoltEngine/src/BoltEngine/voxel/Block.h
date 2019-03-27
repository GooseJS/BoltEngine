#pragma once

#include "BoltEngine/render/Texture.h"

namespace Bolt
{
	typedef uint16_t BlockID;

	class Block
	{
	private:
		friend class BlockManager;
		
		const BlockID _id;
		Texture::TextureArray::Key _texture;
	protected:
		Block(BlockID id) : _id(id) {}

	public:
		const BlockID getID() { return _id; }
		virtual const std::string getName() = 0;

		inline operator BlockID() { getID(); }

		virtual Texture::TextureArray::Key getTextureKey() { return _texture; }

		virtual inline bool hasTransparency() { return false; }
		virtual inline bool shouldRender() { return false; }
		virtual inline bool hasGravity() { return false; }
		virtual inline int getLightLevel() { return 0; }
	};
}