#pragma once

#include <glm/glm.hpp>

#include "Chunk.h"
#include "World.h"
#include "VoxelMesh.h"

namespace Bolt
{
    class WorldRenderer
    {
    public:
        void createChunkMesh(ChunkPtr chunk);
    };
}