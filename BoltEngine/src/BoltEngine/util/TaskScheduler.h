#pragma once

#include <taskflow/taskflow.hpp>

#include <thread>
#include <functional>
#include <chrono>
#include <queue>

#include "BoltEngine/Core.h"

namespace Bolt
{
	class BOLT_API Task
	{
	private:
		friend class TaskQueue;
		friend class TaskScheduler;
		std::function<void()> _callable;
		std::function<void()> _postTask;
		std::shared_future<void> _future;
	public:
		Task()
		{

		}

		Task(std::function<void()> callable, std::function<void()> postTask = nullptr) : _callable(callable), _postTask(postTask)
		{

		}

		Task(std::function<void()> callable, std::function<void()> postTask, std::shared_future<void> future) : _callable(callable), _postTask(postTask), _future(future)
		{

		}

		Task(const Task& other) : _callable(other._callable), _postTask(other._postTask)
		{

		}
	};

	class BOLT_API TaskQueue
	{
	private:
		std::queue<Task> _queue;
		std::mutex _queueMtx;

		std::condition_variable _queueCV;
	public:
		inline void push(Task task)
		{
			if (!task._callable) return;
			std::lock_guard<std::mutex> lock(_queueMtx);
			_queue.push(task);
			_queueCV.notify_one();
		}

		inline bool next(Task& task)
		{
			std::unique_lock<std::mutex> lock(_queueMtx);
			_queueCV.wait(lock, [this]() { return !_queue.empty(); });

			if (!_queue.empty())
			{
				task = _queue.front();
				_queue.pop();
				return true;
			}

			return false;
		}
	};
	
	class BOLT_API TaskScheduler
	{
	private:
		const int workerThreadCount = 3;
	private:
		TaskQueue _tasksToRun;
		std::vector<Task> _completedTasks;
		std::mutex _completedTaskMutex;

		std::vector<std::thread> _workerThreads;
		volatile bool _workerThreadsRunning = false;

		TaskScheduler() {}
		~TaskScheduler()
		{
			_workerThreadsRunning = false;
			for (auto& thread : _workerThreads)
			{
				if (thread.joinable())
					thread.join();
			}
		}
	public:
		static TaskScheduler& getInstance()
		{
			static TaskScheduler instance;
			return instance;
		}

		inline void createTask(std::function<void()> callable, std::function<void()> postTask = nullptr)
		{
			_tasksToRun.push(Task(callable, postTask));
		}

		void startTaskThread();
		inline void stopTaskThread() { _workerThreadsRunning = false; }

		inline void joinTaskThread()
		{
			stopTaskThread();
			for (auto& workerThread : _workerThreads)
			{
				if (workerThread.joinable())
					workerThread.join();
			}
		}

		void update();

		void workerThreadExec();
	};
}