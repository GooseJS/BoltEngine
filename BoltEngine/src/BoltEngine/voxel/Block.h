#pragma once

#include "BoltEngine/render/Texture.h"

namespace Bolt
{
	typedef uint16_t BlockID;

	enum BoltBlockFace
	{
		BLOCK_FACE_X_POSITIVE = 0,
		BLOCK_FACE_X_POSITIVE = 1,
		BLOCK_FACE_X_POSITIVE = 2,
		BLOCK_FACE_X_POSITIVE = 3,
		BLOCK_FACE_X_POSITIVE = 4,
		BLOCK_FACE_X_POSITIVE = 5,
		SIZE				  = 6
	};

	class Block
	{
	private:
		friend class BlockManager;
		
		BlockID _id;
		Texture::TextureArray::Key _texture[BoltBlockFace::SIZE];
	public:
		Block() {}

		const BlockID getID() { return _id; }

		virtual const std::string getName() { return "bolt::nullptr"; };

		inline operator BlockID() { getID(); }

		virtual Texture::TextureArray::Key getTextureKey(BoltBlockFace face) { return _texture[face]; }

		virtual inline bool hasTransparency() { return false; }
		virtual inline bool shouldRender() { return false; }
		virtual inline bool hasGravity() { return false; }
		virtual inline int getLightLevel() { return 0; }
	};
}