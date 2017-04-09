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

public:
	VAsync() = default;
	virtual ~VAsync();

	void SyncToMainLoop(std::function<void()> callback);
	void ProcessSyncRequests();
};

