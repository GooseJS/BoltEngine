#pragma once

#include <glm/glm.hpp>

#include "Chunk.h"
#include "World.h"
#include "VoxelMesh.h"
#include "BoltEngine/render/Camera.h"
#include "BoltEngine/render/Shader.h"

namespace Bolt
{
    class BOLT_API WorldRenderer
    {
	private:
		Camera* _worldRenderCam;

		Shader _worldRenderShader;
    public:
		void initRenderer();

        void createChunkMesh(ChunkPtr chunk, int chunkY = -1);

		void buildChunks(World* world);

		void renderWorld(World* world);

		inline void setWorldRenderCam(Camera* cam) { _worldRenderCam = cam; }
    };
}