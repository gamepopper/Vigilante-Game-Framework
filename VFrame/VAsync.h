#pragma once
#include <queue>
#include <mutex>
#include <future>

class VAsync
{
private:
	std::queue<std::function<void()>> syncQueue;
	std::mutex syncMutex;
	std::condition_variable condition;
	unsigned int activeAsyncFunctions = 0;

public:
	VAsync() = default;
	virtual ~VAsync();

	//Check if any active async functions are in scene.
	bool ActiveAsyncFunctions();

	//Pass in an std::future to process the asynchronous function call.
	void LaunchAsyncFunction(std::future<void> &f);

	//Must call this in order to tell the game that the function has ended
	void SyncToMainLoop(std::function<void()> callback);

	//VGame must call this to process the async function queue.
	void ProcessSyncRequests();
};

