#include "pch.h"
#include "CorePch.h"

#include <thread>
#include <atomic>
#include <mutex>

class SpinLock
{
public:
	void lock()
	{
		bool expected = false;
		while (!locked_.compare_exchange_strong(expected, true))
			expected = false;
	}
	void unlock()
	{
		locked_.store(false);
	}
private:
	std::atomic<bool> locked_ = false;
};

int32 sum = 0;
std::mutex m;
SpinLock spinLock;

void Add()
{
	for (int32 i = 0; i < 10'0000; ++i)
	{
		std::lock_guard<SpinLock>guard(spinLock);
		sum++;
	}
}
void Sub()
{
	for (int32 i = 0; i < 10'0000; ++i)
	{
		std::lock_guard<SpinLock>guard(spinLock);
		sum--;
	}
}

int main()
{
	std::thread t1(Add);
	std::thread t2(Sub);

	t1.join();
	t2.join();

	std::cout << sum << "\n";
}