#pragma once
#include <mutex>
#include <queue>
#include <stack>

template<typename T>
class LockQueue
{
public:
	LockQueue() {}
	LockQueue(const LockQueue&) = delete;
	LockQueue& operator = (const LockQueue&) = delete;

	void Push(T val)
	{
		std::lock_guard<std::mutex> lock(mutex_);
		queue_.push(std::move(val));
		condVar_.notify_one();
	}

	bool Pop(T& val)
	{
		std::lock_guard<std::mutex> lock(mutex_);
		if (queue_.empty())
			return false;

		val = std::move(queue_.front());
		queue_.pop();
		return true;
	}

	bool Empty()
	{
		std::lock_guard<std::mutex> lock(mutex_);
		return queue_.empty();
	}

	void WaitPop(T& val)
	{
		std::unique_lock<std::mutex> lock(mutex_);
		condVar_.wait(lock, [this] {return !queue_.empty(); });
		val = std::move(queue_.front());
		queue_.pop();
	}

private:
	std::queue<T> queue_;
	std::mutex mutex_;
	std::condition_variable condVar_;
};

