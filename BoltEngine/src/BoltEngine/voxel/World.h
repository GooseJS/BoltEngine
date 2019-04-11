#pragma once

#include <mutex>
#include <thread>
#include <vector>
#include <unordered_map>

#include "Types.h"
#include "Chunk.h"

namespace Bolt
{
	typedef std::vector<ChunkPtr> ChunkList;
	typedef std::unordered_map<uint64_t, ChunkColumn*> ChunkMap;

	class World
	{
	private:
		std::mutex _chunkMapMutex;
		ChunkMap _chunkMap;

		std::mutex _rebuildChunkMutex;
		ChunkList _rebuildChunks;
		ChunkList _uploadChunks;
	public:
		ChunkPtr getChunkAt(ChunkPos pos);

		Block& getBlockAt(BlockPos pos);
		Block& setBlockAt(BlockPos pos, Block& block);

		inline ChunkMap& getChunkMap() { return _chunkMap; }
		inline ChunkList& getChunksToRebuild() { return _rebuildChunks; }
		inline ChunkList& getChunksToUpload() { return _uploadChunks; }

		inline ChunkList copyChunksToRebuild() { return std::vector(_rebuildChunks); }
	};
}