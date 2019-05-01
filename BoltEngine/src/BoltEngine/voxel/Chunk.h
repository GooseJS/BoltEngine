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
	/// A simple enum to represent an Axis
	enum Axis
	{
		X = 0,
		Y = 1,
		Z = 2,
		COUNT = 3
	};

	/// Holds the actual data for what block is where in each chunk
	struct ChunkStorage
	{
	private:
		BlockID _storage[BOLT_CHUNK_VOLUME] = { 0 };
	public:
		Block& getBlockAt(int index);
		Block& setBlockAt(int index, const Block& block);
	};

	/// A slice of a chunk on a certain axis (x, y, or z) to be able to skip rendering if it is empty or surrounded by full slices
	struct ChunkSlice
	{
	private:
		int _count; /// The amount of blocks currently in the slice
		bool _full; /// If the slice is full or not
		bool _empty; /// If the slice is empty or not
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

	/// A Chunk consists of the ChunkStorage (which actually stores the blocks) as well as a mesh for rendering
	class BOLT_API Chunk
	{
	private:
		ChunkPos _pos; /// The position of this chunk
		ChunkStorage _storage; /// The chunks block storage
        BoltChunkMesh* _mesh = nullptr; /// The chunks mesh
        class World* _containingWorld; /// The world this chunk is contained in
		class ChunkColumn* _containingColumn; /// The chunk column this chunk is contained in

		ChunkSlice _slices[Axis::COUNT][BOLT_CHUNK_WIDTH]; /// The chunk slices per axis for rendering optimization

	public:
		Chunk(ChunkPos pos);
		~Chunk()
		{
			if (_mesh != nullptr) delete _mesh;
		}

		Block& getBlockAt(ChunkBlockPos pos);
		Block& setBlockAt(ChunkBlockPos pos, const Block& block);

        inline BoltChunkMesh& getMesh() { if (_mesh == nullptr) _mesh = new BoltChunkMesh(); return *_mesh; }

        inline void setContainingWorld(World* world) { _containingWorld = world; }
		inline World* getContainingWorld() const { return _containingWorld; }
		inline void setContainingColumn(ChunkColumn* column) { _containingColumn = column; }
		inline ChunkColumn* getContainingColumn() const { return _containingColumn; }

 		inline ChunkPos getPos() const { return _pos; }

		inline const ChunkSlice& getSliceAt(Axis axis, int value) const { return _slices[axis][value]; }

		void generateMesh();
	};

	typedef Chunk* ChunkPtr;
	typedef std::vector<ChunkPtr> ChunkList;

	/// A wrapper to hold a column of chunks. Makes hashing and chunk lookups easier as the y value can be ignored in the initial lookup and only used for the last step
	class BOLT_API ChunkColumn
	{
	private:
		ChunkPtr* _chunks = nullptr; /// The chunks contained in this column

		volatile bool _initiallyGenerated = false; /// If initial world generation has run on this column or not yet

		volatile bool _generating = false; /// If the column is currently being generated
	public:
		ChunkColumn(class World* containingWorld, int x, int z)
		{
			_chunks = (ChunkPtr*)malloc(sizeof(ChunkPtr) * BOLT_WORLD_HEIGHT);
			for (int y = 0; y < BOLT_WORLD_HEIGHT; y++)
			{
				_chunks[y] = DBG_NEW Chunk(ChunkPos(x, y, z)); // TODO(Brendan): Look into placement new
				_chunks[y]->setContainingWorld(containingWorld);
			}
		}

		~ChunkColumn()
		{
			for (int y = 0; y < BOLT_WORLD_HEIGHT; y++)
			{
				if (_chunks[y] != nullptr) delete _chunks[y];
			}
			free(_chunks);
		}

		inline void setGenerated(bool generated) { _initiallyGenerated = generated; }
		inline bool isGenerated() { return _initiallyGenerated; }

		inline void setGenerating(bool generating) { _generating = generating; }
		inline bool isGenerating() { return _generating; }

		inline bool inColumn(ChunkPos pos) { return pos.x == _chunks[0]->getPos().x && pos.z == _chunks[0]->getPos().z; }

		inline Block& getBlockAt(ChunkBlockPos pos) { return _chunks[pos.chunkPos.y]->getBlockAt(pos); }
		inline Block& setBlockAt(ChunkBlockPos pos, const Block& block) { return _chunks[pos.chunkPos.y]->setBlockAt(pos, block); }

		inline ChunkPtr getChunkAt(int y) { return _chunks[y]; }
	};
}