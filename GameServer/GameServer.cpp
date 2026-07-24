#include "pch.h"
#include "CorePch.h"

#include <thread>
#include <atomic>
#include <mutex>
#include <queue>
#include <windows.h>
#include <chrono>

std::mutex m;
std::queue<int32> q;
HANDLE handle;

void Producer()
{
	while (true)
	{
		std::unique_lock<std::mutex> lock(m);
		q.push(100);
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		SetEvent(handle);
	}
}

void Consumer()
{
	while (true)
	{
		WaitForSingleObject(handle, -1);
		std::unique_lock<std::mutex> lock(m);
		if (!q.empty())
		{
			int32 data = q.front();
			q.pop();
			std::cout << data << "\n";
		}
	}
}

int main()
{
	handle = CreateEvent(NULL, FALSE, FALSE, NULL);
	std::thread t1(Producer);
	std::thread t2(Consumer);

	t1.join();
	t2.join();
}