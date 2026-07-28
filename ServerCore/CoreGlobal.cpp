#include "pch.h"
#include "CoreGlobal.h"
#include "ThreadManager.h"

ThreadManager* g_thread_manager = nullptr;

CoreGlobal::CoreGlobal()
{
	g_thread_manager = new ThreadManager();
}
CoreGlobal::~CoreGlobal()
{
	delete g_thread_manager;
}
