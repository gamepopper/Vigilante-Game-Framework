//Reference: https://www.reddit.com/r/gamedev/comments/2ivsp6/async_loading_and_syncing_threads_with_your/

#include "VAsync.h"

VAsync::~VAsync()
{
	while (!syncQueue.empty())
	{
		syncQueue.pop();
	}
}

void VAsync::SyncToMainLoop(std::function<void()> callback)
{
	syncMutex.lock();
	syncQueue.push(callback);
	syncMutex.unlock();
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
