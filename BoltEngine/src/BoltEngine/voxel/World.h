#pragma once

#include <unordered_map>

#include "Types.h"
#include "Chunk.h"

namespace Bolt
{
	typedef Chunk* ChunkPtr;
	typedef std::unordered_map<uint64_t, ChunkPtr> ChunkMap;

	class World
	{
	private:
		ChunkMap _chunkMap;
	public:
		ChunkPtr getChunkAt(ChunkPos pos);

		Block& getBlockAt(BlockPos pos);
		Block& setBlockAt(BlockPos pos, Block& block);

		inline ChunkMap& getChunkMap() { return _chunkMap; }
	};
}