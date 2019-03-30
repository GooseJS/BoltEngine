#pragma once

#include "BoltEngine/Core.h"

#include "Types.h"
#include "Block.h"

namespace Bolt
{
	class BOLT_API Chunk
	{
	private:
		ChunkPos _pos;
		BlockID _storage[BOLT_CHUNK_VOLUME]{};
	public:
		Chunk(ChunkPos pos);
		Block& getBlockAt(int index);
		Block& setBlockAt(int index, Block& block);

		Block& getBlockAt(ChunkBlockPos pos);
		Block& setBlockAt(ChunkBlockPos pos, Block& block);
	};
}