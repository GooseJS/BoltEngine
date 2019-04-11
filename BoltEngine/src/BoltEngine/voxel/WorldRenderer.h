#pragma once

#include <thread>
#include <future>
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
		const int maxChunkGenerationThreads = 8;

	private:
		std::vector<std::future<ChunkPtr>> _chunkGenerationThreads;

		Camera* _worldRenderCam;

		Shader _worldRenderShader;
    public:
		void initRenderer();

        void createChunkMesh(ChunkPtr chunk);

		void checkForRebuildChunks(World* world);

		void renderWorld(World* world);

		inline void setWorldRenderCam(Camera* cam) { _worldRenderCam = cam; }
    };
}