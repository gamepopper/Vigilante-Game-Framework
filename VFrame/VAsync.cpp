//Reference: https://www.reddit.com/r/gamedev/comments/2ivsp6/async_loading_and_syncing_threads_with_your/

#include "VAsync.h"

VAsync::~VAsync()
{
	while (!syncQueue.empty())
	{
		syncQueue.pop();
	}
}

bool VAsync::ActiveAsyncFunctions()
{
	return activeAsyncFunctions > 0;
}

void VAsync::LaunchAsyncFunction(std::future<void> &f)
{
	if (f.valid())
		activeAsyncFunctions++;
}

void VAsync::SyncToMainLoop(std::function<void()> callback)
{
	if (callback != nullptr)
	{
		syncMutex.lock();
		syncQueue.push(callback);
		syncMutex.unlock();
	}
	activeAsyncFunctions--;
}

void VAsync::ProcessSyncRequests()
{
	if (syncQueue.empty())
		return;

	syncMutex.lock();
	while (!syncQueue.empty())
	{
		auto callback = syncQueue.front();
		callback();
		syncQueue.pop();
	}
	syncMutex.unlock();
}
