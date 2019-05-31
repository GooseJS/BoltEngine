#pragma once

#include <mutex>
#include <thread>
#include <vector>
#include <unordered_map>
#include <future>

#include "WorldRenderer.h"
#include "Types.h"
#include "Chunk.h"
#include "BoltEngine/voxel/WorldGenerator.h"

namespace Bolt
{
	typedef std::unordered_map<uint64_t, ChunkColumn*> ChunkMap;
	typedef std::vector<ChunkColumn*> ChunkColumnList;

	class World
	{
	private:
		ChunkPool _chunkPool; /// Managing getting new chunks and freeing old chunks

		ChunkMap _chunkMap; /// All the chunks currently stored in the world
		ChunkColumn* _lastAccessedChunk = nullptr; /// The last accessed chunk column (cached to speed up lookups)

		ChunkList _rebuildChunks; /// The list of chunks that need to have their meshes rebuilt
		ChunkColumnList _unloadChunks; /// The list of chunks that need to be unloaded from memory
		//ChunkColumnList _freeChunks; /// A list of chunks that have been loaded and can be re-used so new data doesn't have to be allocated
		std::vector<std::future<ChunkPtr>> _generatingChunks; /// The list of chunks that need to have world generation run on them

		std::mutex _unloadMtx;

		WorldGenerator* _generator; /// The current world generator

		int _renderDistance = 2; // TODO(Brendan): Render distance should be pulled out and put into a different system. It feels weird in here
	public:
		World() {}
		~World()
		{
			for (auto chunkColumnPair : _chunkMap)
			{
				delete chunkColumnPair.second;
			}
			_chunkMap.clear();
		}

		ChunkPtr getChunkAt(ChunkPos pos);
		ChunkColumn* getChunkColumnAt(ChunkPos pos);

		/// Gets the block at the specified position
		Block& getBlockAt(BlockPos pos);
		/// Sets the block at the specified position, returns the previous block
		Block& setBlockAt(BlockPos pos, Block& block);

		void updateRenderChunksFromPos(BlockPos pos, WorldRenderer* worldRenderer); // TODO(Brendan): This is temporary, just for procedual gen now

		inline ChunkMap& getChunkMap() { return _chunkMap; }
		inline ChunkList& getChunksToRebuild() { return _rebuildChunks; }

		inline void setWorldGenerator(WorldGenerator* worldGenerator) { _generator = worldGenerator; }

		inline ChunkList copyChunksToRebuild() { return std::vector(_rebuildChunks); } // TODO(Brendan): Currently I have this copy to be safer for multithreading, look into efficiency though
	};
}