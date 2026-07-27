#include "pch.h"
#include "CorePch.h"

#include <thread>
#include <atomic>
#include <mutex>
#include <queue>
#include <windows.h>
#include <chrono>

thread_local  int32 LThreadId = 0;

void ThreadMain(int32 id)
{
	LThreadId = id;

}

int main()
{
	std::thread t;
	t.get_id()
}