#pragma once

#include <vector>
#include <unordered_map>

#include "Types.h"
#include "Chunk.h"

namespace Bolt
{
	typedef Chunk* ChunkPtr;
	typedef std::vector<ChunkPtr> ChunkList;
	typedef std::unordered_map<uint64_t, ChunkPtr> ChunkMap;

	class World
	{
	private:
		ChunkMap _chunkMap;

		ChunkList _rebuildChunks;
	public:
		ChunkPtr getChunkAt(ChunkPos pos);

		Block& getBlockAt(BlockPos pos);
		Block& setBlockAt(BlockPos pos, Block& block);

		inline ChunkMap& getChunkMap() { return _chunkMap; }
		inline ChunkList& getChunksToRebuild() { return _rebuildChunks; }
	};
}