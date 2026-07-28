#include "pch.h"
#include "CorePch.h"

#include <thread>
#include <atomic>
#include <mutex>
#include <queue>
#include <windows.h>
#include <chrono>
#include <valarray>
#include "ThreadManager.h"
#include "CoreMacro.h"
using namespace std::chrono_literals;

CoreGlobal Core;

void ThreadMain()
{
	while (true)
	{
		std::cout << "Hello! I am Thread... " << LThreadId << std::endl;
		std::this_thread::sleep_for(1s);
	}
}

int main()
{
	for (int32 i = 0; i < 5; ++i)
	{
		g_thread_manager->Launch(ThreadMain);
	}
	g_thread_manager->Join();
}