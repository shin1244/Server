#include "pch.h"
#include "CorePch.h"

#include <thread>
#include <atomic>
#include <mutex>

int32 sum = 0;
std::mutex m;

void Add()
{
	for (int32 i = 0; i < 10'0000; ++i)
	{
		std::lock_guard<std::mutex>guard(m);
		sum++;
	}
}
void Sub()
{
	for (int32 i = 0; i < 10'0000; ++i)
	{
		std::lock_guard<std::mutex>guard(m);
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