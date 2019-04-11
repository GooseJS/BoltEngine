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
	enum Axis
	{
		X = 0,
		Y = 1,
		Z = 2,
		COUNT = 3
	};

	struct ChunkStorage
	{
	private:
		BlockID _storage[BOLT_CHUNK_VOLUME] = { 0 };
	public:
		Block& getBlockAt(int index);
		Block& setBlockAt(int index, const Block& block);
	};

	struct ChunkSlice
	{
	private:
		int _count;
		bool _full;
		bool _empty;
	public:
		void addBlock(const Block& block)
		{
			if (block.shouldRender())
				_count++;
			else
				_count--;

			_full = false;
			_empty = false;

			if (_count == BOLT_CHUNK_AREA && !_full)
				_full = true;
			else if (_full)
				_full = false;
			else if (_count == 0 && !_empty)
				_empty = true;
			else if (_empty)
				_empty = false;
		}

		inline const bool isFull() const { return _full; }
		inline const bool isEmpty() const { return _empty; }
	};

	class BOLT_API Chunk
	{
	private:
		ChunkPos _pos;
		ChunkStorage _storage;
        BoltChunkMesh* _mesh = nullptr;
        class World* _containingWorld;

		ChunkSlice _slices[Axis::COUNT][BOLT_CHUNK_WIDTH];

	public:
		Chunk(ChunkPos pos);

		Block& getBlockAt(ChunkBlockPos pos);
		Block& setBlockAt(ChunkBlockPos pos, const Block& block);

        inline BoltChunkMesh& getMesh() { if (_mesh == nullptr) _mesh = new BoltChunkMesh(); return *_mesh; }

        inline void setContainingWorld(World* world) { _containingWorld = world; }
		inline World* getContainingWorld() const { return _containingWorld; }

		inline ChunkPos getPos() const { return _pos; }

		inline const ChunkSlice& getSliceAt(Axis axis, int value) const { return _slices[axis][value]; }
	};

	typedef Chunk* ChunkPtr;

	class BOLT_API ChunkColumn
	{
	private:
		ChunkPtr* _chunks = nullptr;
	public:
		ChunkColumn(World* containingWorld, int x, int z)
		{
			_chunks = (ChunkPtr*)malloc(sizeof(ChunkPtr) * BOLT_WORLD_HEIGHT);
			for (int y = 0; y < BOLT_WORLD_HEIGHT; y++)
			{
				_chunks[y] = new Chunk(ChunkPos(x, y, z)); // TODO(Brendan): Look into placement new
				_chunks[y]->setContainingWorld(containingWorld);
			}
		}

		inline Block& getBlockAt(ChunkBlockPos pos) { return _chunks[pos.chunkPos.y]->getBlockAt(pos); }
		inline Block& setBlockAt(ChunkBlockPos pos, const Block& block) { return _chunks[pos.chunkPos.y]->setBlockAt(pos, block); }

		inline ChunkPtr getChunkAt(int y) { return _chunks[y]; }
	};
}