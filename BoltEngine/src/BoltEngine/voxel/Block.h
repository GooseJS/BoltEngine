#pragma once

#include "BoltEngine/render/Texture.h"

namespace Bolt
{
	typedef uint16_t BlockID;

	class Block
	{
	private:
		friend class BlockManager;
		
		BlockID _id;
		Texture::TextureArray::Key _texture;
	public:
		Block() {}

		const BlockID getID() { return _id; }

		virtual const std::string getName() { return "bolt::nullptr"; };

		inline operator BlockID() { getID(); }

		virtual Texture::TextureArray::Key getTextureKey() { return _texture; }

		virtual inline bool hasTransparency() { return false; }
		virtual inline bool shouldRender() { return false; }
		virtual inline bool hasGravity() { return false; }
		virtual inline int getLightLevel() { return 0; }
	};
}