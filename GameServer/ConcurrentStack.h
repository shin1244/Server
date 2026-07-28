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

template<typename T>
class LockFreeStack
{
	struct Node
	{
		Node(const T& value) : data(value), next(nullptr) {}
		T data;
		Node* next;
	};

public:
	void Push(const T& value)
	{
		Node* n = new Node(value);
		n->next = head_;
		
		head_.compare_exchange_strong(n->next, n);
	}

	bool Pop(T& value)
	{
		Node* oldHead = head_;
		if (oldHead == nullptr) return false;
		
		head_.compare_exchange_strong(oldHead, oldHead->next);
		value = oldHead->data;
		delete oldHead;
		return true;
	} 
private:
	std::atomic<Node*> head_;
	std::atomic<int32> popCount_ = 0;
};