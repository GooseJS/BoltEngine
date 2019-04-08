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

	class Block
	{
	private:
		friend class BlockManager;
		
		BlockID _id;
		Texture::TextureArray::Key _texture[BoltBlockFace::SIZE];

		std::unordered_map<BoltBlockFace, std::string> _faceTextures;
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