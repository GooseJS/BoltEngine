#include "TaskScheduler.h"

namespace Bolt
{
	void TaskScheduler::startTaskThread()
	{
		if (!_workerThreadsRunning)
		{
			_workerThreadsRunning = true;
			for (int i = 0; i < workerThreadCount; i++)
			{
				_workerThreads.emplace_back(std::bind(&TaskScheduler::workerThreadExec, this));
			}

			for (auto& thread : _workerThreads)
			{
				thread.detach();
			}
		}
	}

	void TaskScheduler::update()
	{
		std::lock_guard<std::mutex> lock(_completedTaskMutex);
		if (_completedTasks.empty()) return;

		for (auto& task : _completedTasks)
		{
			if (task._postTask)
				task._postTask();
		}

		_completedTasks.clear();
	}

	void TaskScheduler::workerThreadExec()
	{
		while (_workerThreadsRunning)
		{
			Task task;
			_tasksToRun.next(task);
			task._callable();
			
			if (task._postTask)
			{
				std::lock_guard<std::mutex> lock(_completedTaskMutex);
				_completedTasks.push_back(task);
			}
		}
	}
}