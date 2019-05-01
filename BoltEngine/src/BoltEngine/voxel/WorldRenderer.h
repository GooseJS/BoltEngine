#pragma once

#include <thread>
#include <future>
#include <glm/glm.hpp>

#include "Chunk.h"
#include "VoxelMesh.h"
#include "Types.h"
#include "BoltEngine/render/Camera.h"
#include "BoltEngine/render/Shader.h"

namespace Bolt
{
    class BOLT_API WorldRenderer
    {
	private:
		ChunkList _uploadChunks; /// The list of chunks that need to have their meshes upload
		std::mutex _uploadChunksMtx; /// The mutex for the chunks that need to be uploaded since they may be getting accessed from different threads

		Camera* _worldRenderCam;

		Shader _worldRenderShader;
    public:
		void initRenderer();

		void initialBuild(class World* world);

		void checkForRebuildChunks(World* world);

		void renderWorld(World* world);

		inline void setWorldRenderCam(Camera* cam) { _worldRenderCam = cam; }
		inline void queueChunkMeshForUpload(ChunkPtr chunk)
		{
			_uploadChunksMtx.lock();
			_uploadChunks.push_back(chunk);
			_uploadChunksMtx.unlock();
		}
		inline ChunkList getUploadChunks()
		{
			if (_uploadChunksMtx.try_lock())
			{
				ChunkList retVal = _uploadChunks; // TODO(Brendan): Look into efficiency of double copy?
				_uploadChunksMtx.unlock();
				return retVal;
			}
			else
			{
				return ChunkList();
			}
		}
    };
}