#pragma once

#include <chrono>
#include <vector>

#include "BoltEngine/logger/Logger.h"

namespace Bolt
{
	struct BoltProfilerBlock
	{
		std::chrono::high_resolution_clock::time_point timeStarted;
		long timeTaken = 0;
		
		std::string blockName;

		BoltProfilerBlock* parentBlock = nullptr;
		std::vector<BoltProfilerBlock*> childBlocks;
	};

	class BoltProfilerBlockList
	{
	private:
		friend class BoltProfiler;
		std::vector<BoltProfilerBlock*> _blocks;
	public:
		inline void addBlock(BoltProfilerBlock* block)
		{
			_blocks.push_back(block);
		}

		inline std::vector<BoltProfilerBlock*>& getBlocks()
		{
			return _blocks;
		}

		inline void cleanup()
		{
			for (auto iter = _blocks.begin(); iter != _blocks.end();)
			{
				delete *iter;
				iter = _blocks.erase(iter);
			}
		}
	};

	class BoltProfiler
	{
	private:
		BoltProfilerBlockList _blocks;
		BoltProfilerBlock* _currentBlock = nullptr;
	public:
		static BoltProfiler& getInstance()
		{
			static BoltProfiler instance;
			return instance;
		}

		inline void startSection(const std::string& blockName)
		{
			BoltProfilerBlock* block = new BoltProfilerBlock();
			block->blockName = blockName;
			
			if (_currentBlock != nullptr)
			{
				block->parentBlock = _currentBlock;
				_currentBlock->childBlocks.push_back(block);
			}

			_currentBlock = block;
			_blocks.addBlock(block);
			_currentBlock->timeStarted = std::chrono::high_resolution_clock::now();
		}

		inline void endSection()
		{
			if (_currentBlock == nullptr)
				return;

			_currentBlock->timeTaken = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - _currentBlock->timeStarted).count();
		}

		inline void endStartSection(const std::string& blockName)
		{
			endSection();
			startSection(blockName);
		}

		inline BoltProfilerBlockList newFrame()
		{
			BoltProfilerBlockList copy = _blocks;
			_blocks._blocks.clear();
			return copy;
		}
	};
}