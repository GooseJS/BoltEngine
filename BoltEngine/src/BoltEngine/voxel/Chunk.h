#pragma once

#include "BoltEngine/Core.h"

#include "Types.h"
#include "Block.h"

#include "VoxelMesh.h"

#ifndef BOLT_BLOCK_RENDER_SIZE
#define BOLT_BLOCK_RENDER_SIZE 1
#endif

namespace Bolt
{
	struct ChunkStorage
	{
		BlockID _storage[BOLT_CHUNK_VOLUME] = { 0 };

		Block& getBlockAt(int index);
		Block& setBlockAt(int index, Block& block);
	};

	class BOLT_API Chunk
	{
	private:
		ChunkPos _pos;
		ChunkStorage _storage[BOLT_WORLD_HEIGHT];
        BoltChunkMesh* _mesh = nullptr;
        class World* _containingWorld;

	public:
		Chunk(ChunkPos pos);

		Block& getBlockAt(ChunkBlockPos pos);
		Block& setBlockAt(ChunkBlockPos pos, Block& block);

        inline BoltChunkMesh& getMesh(int y) { if (_mesh == nullptr) _mesh = new BoltChunkMesh[BOLT_WORLD_HEIGHT]; return _mesh[y]; }

        inline void setContainingWorld(World* world) { _containingWorld = world; }
		inline World* getContainingWorld() const { return _containingWorld; }

		inline ChunkPos getPos() const { return _pos; }
	};
}