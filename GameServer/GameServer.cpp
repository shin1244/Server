#include "pch.h"
#include "CorePch.h"

#include <thread>
#include <atomic>
#include <mutex>
#include <queue>
#include <windows.h>
#include <chrono>

int main()
{
	std::atomic<bool> flag = false;

	flag.store(true);

	bool val = flag.load();
}