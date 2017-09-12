#pragma once
#include <vector>

#ifdef __linux__
#include "stddef.h"
#endif

class VTimer
{
private:
	bool running = true;
	float time = 0.0f;

public:
	VTimer(bool addToManager = true);
	~VTimer() = default;

	float Update(float dt);

	//Returns true if the timer is currently paused.
	bool IsPaused();

	//Restarts timer at 0.
	float Restart();
	//Pauses the timer.
	void Pause();
	//Resumes timer at current point.
	void Resume();

	//Gets time in seconds.
	float Seconds();
	//Gets time in milliseconds.
	int Milliseconds();
	//Gets time in microseconds.
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
