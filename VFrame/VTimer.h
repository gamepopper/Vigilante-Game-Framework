#pragma once
#include <vector>

class VTimer
{
private:
	bool running = true;
	float time = 0.0f;

public:
	VTimer(bool addToManager = true);
	~VTimer() = default;

	float Update(float dt);
	bool IsPaused();

	float Restart();
	void Pause();
	void Resume();

	float Seconds();
	int Milliseconds();
	int Microseconds();
};

class VTimeManager
{
private:
	static VTimeManager* Instance;
	VTimeManager();
	~VTimeManager();

	std::vector<VTimer*> timers;

public:
	static bool AnyActiveTimers();

	static VTimeManager* p()
	{
		if (!Instance)
		{
			Instance = new VTimeManager();
		}

		return Instance;
	}

	void Update(float dt);
	void AddTimer(VTimer* timer);
	void Reserve(size_t size);
	void Clear(bool destroy = true);
	int Count();
};
