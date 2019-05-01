#pragma once

#include <noise/noise.h>
#include <noiseutils.h>

#include "WorldGenerator.h"

namespace Bolt
{
	class EarthWorldGenerator : public WorldGenerator
	{
	private:
		noise::module::Perlin _perlin;

		int _lowestAboveGround = 50;

	public:
		virtual void setup() override;

		virtual void generateChunk(ChunkColumn* chunk) override;
		
		virtual BlockPos getTopHeight(const BlockPos& pos, noise::utils::NoiseMap* heightMap) override;
	};
}