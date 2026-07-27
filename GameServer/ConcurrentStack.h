#pragma once

#include <mutex>
#include <stack>

template<typename T>
class LockStack
{
public:
	LockStack() {}
	LockStack(const LockStack&) = delete;
	LockStack& operator = (const LockStack&) = delete;

	void Push(T val)
	{
		std::lock_guard<std::mutex> lock(mutex_);
		stack_.push(std::move(val));
		condVar_.notify_one();
	}

	bool Pop(T& val)
	{
		std::lock_guard<std::mutex> lock(mutex_);
		if (stack_.empty())
			return false;

		val = std::move(stack_.top());
		stack_.pop();
		return true;
	}

	bool Empty()
	{
		std::lock_guard<std::mutex> lock(mutex_);
		return stack_.empty();
	}

	void WaitPop(T& val)
	{
		std::unique_lock<std::mutex> lock(mutex_);
		condVar_.wait(lock, [this] {return !stack_.empty(); });
		val = std::move(stack_.top());
		stack_.pop();
	}

private:
	std::stack<T> stack_;
	std::mutex mutex_;
	std::condition_variable condVar_;
};

