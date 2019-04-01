#pragma once

#include <vector>
#include <glm/glm.hpp>
#include "BoltEngine/GL.h"

namespace Bolt
{
	typedef struct BoltChunkMeshVertex
	{
		float vertexPosition[3];
		float vertexNormal[3];
		float texCoord[3];
	};

	typedef struct BoltChunkMeshTriangle
	{
		unsigned int vertexIndices[3];
	};

	class BoltChunkMesh
	{
	private:
		std::vector<BoltChunkMeshTriangle*> _triangles;
		std::vector<BoltChunkMeshVertex*> _vertices;
	public:		
		bool building = false;
		bool initialized = false;

		unsigned int _vaoID;
		unsigned int _vertexBufferID;
		unsigned int _indexBufferID;

		unsigned int numVertices = 0;
		unsigned int numIndices = 0;

        unsigned int addVertexToMesh(glm::vec3 positions, glm::vec3 normals, glm::vec3 texCoords);
        unsigned int addTriangleToMesh(unsigned int v1, unsigned int v2, unsigned int v3);

		void uploadMesh();
	};
}