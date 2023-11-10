#include "VTimer.h"

VTimeManager::VTimeManager()
{

}

VTimeManager::~VTimeManager()
{
	Clear(true);
}

VTimer* VTimeManager::AddTimer()
{
	VTimer* t = new VTimer();
	AddTimer(t);
	return t;
}

void VTimeManager::Update(float dt)
{
	for (VTimer* time : timers)
	{
		if (!time->IsPaused())
		{
			time->Update(dt);
		}
	}
}

VTimerEvent* VTimeManager::AddTimerEvent(unsigned int time, std::function<void()> eventFunction, bool looping)
{
	VTimerEvent* t = new VTimerEvent(time, eventFunction, looping);
	AddTimer(t);
	return t;
}

void VTimeManager::AddTimer(VTimer* timer)
{
	timers.push_back(timer);
}

#include <algorithm>
void VTimeManager::RemoveTimer(VTimer* timer)
{
	std::vector<VTimer*>::iterator i = std::find(timers.begin(), timers.end(), timer);

	if (i != timers.end())
	{
		timers.erase(i);
	}
}

void VTimeManager::Reserve(size_t size)
{
	timers.reserve(size);
}

void VTimeManager::Clear(bool destroy)
{
	if (destroy)
	{
		for (VTimer* time : timers)
		{
			delete time;
		}
	}

	timers.clear();
	timers.shrink_to_fit();
}

int VTimeManager::Count()
{
	return (int)timers.size();
}

VTimer::VTimer()
{
	running = true;
	time = 0.0f;
}

VTimer::~VTimer()
{
	
}

float VTimer::Update(float dt)
{
	if (running)
		time += dt;

	return time;
}

bool VTimer::IsPaused()
{
	return !running;
}

float VTimer::Restart()
{
	float oldTime = time;
	time = 0;
	running = true;

	return oldTime;
}

void VTimer::Pause()
{
	running = false;
}

void VTimer::Resume()
{
	running = true;
}

float VTimer::Seconds()
{
	return time;
}

unsigned int VTimer::Milliseconds()
{
	return static_cast<unsigned int>(time * 1000);
}

uint64_t VTimer::Microseconds()
{
	return static_cast<uint64_t>(time * 1000 * 1000);
}

VTimerEvent::VTimerEvent(unsigned int time, std::function<void()> eventFunction, bool looping) : VTimer()
, loop(looping)
, timePoint(time)
, function(eventFunction)
{
}

VTimerEvent::~VTimerEvent()
{
	function = nullptr;
}

void VTimerEvent::Stop()
{
	Pause();
	function = nullptr;
}

void VTimerEvent::SetLooping(bool value)
{
	loop = value;
}

bool VTimerEvent::GetLooping()
{
	return loop;
}

float VTimerEvent::Update(float dt)
{
	float t = VSUPERCLASS::Update(dt);

	if (function != nullptr && timePoint <= Milliseconds())
	{
		function();

		if (loop)
			Restart();
		else
			function = nullptr;
	}

	return t;
}
