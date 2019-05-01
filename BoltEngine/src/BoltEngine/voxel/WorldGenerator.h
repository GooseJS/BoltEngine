#pragma once

#include "BoltEngine/Core.h"
#include "BoltEngine/logger/Logger.h"
#include "BoltEngine/voxel/Types.h"
#include "BoltEngine/voxel/Chunk.h"

#include <noiseutils.h>

namespace Bolt
{
	class WorldGenerator
	{
	public:
		virtual void setup() = 0;
		virtual void generateChunk(ChunkColumn* chunk) = 0;
		virtual BlockPos getTopHeight(const BlockPos& pos, noise::utils::NoiseMap* map) = 0;
	};
}