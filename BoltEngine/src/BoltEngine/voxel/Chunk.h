#pragma once

#include "BoltEngine/Core.h"

#include "Types.h"
#include "Block.h"

#include "VoxelMesh.h"

#ifndef BOLT_BLOCK_RENDER_SIZE
#define BOLT_BLOCK_RENDER_SIZE 16
#endif

namespace Bolt
{
	struct ChunkStorage
	{
		BlockID _storage[BOLT_CHUNK_VOLUME]{};

		Block& getBlockAt(int index);
		Block& setBlockAt(int index, Block& block);
	};

	class BOLT_API Chunk
	{
	private:
		ChunkPos _pos;
		ChunkStorage _storage[BOLT_WORLD_HEIGHT];
        BoltChunkMesh* _mesh;
        class World* _containingWorld;

	public:
		Chunk(ChunkPos pos);

		Block& getBlockAt(ChunkBlockPos pos);
		Block& setBlockAt(ChunkBlockPos pos, Block& block);

        inline BoltChunkMesh& getMesh() { if (_mesh == nullptr) _mesh = new BoltChunkMesh();  return *_mesh; }

        inline void setContainingWorld(World* world) { _containingWorld = world; }
		inline World* getContainingWorld() const { return _containingWorld; }

		inline ChunkPos getPos() const { return _pos; }
	};
}