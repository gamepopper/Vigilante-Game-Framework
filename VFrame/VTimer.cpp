#include "VTimer.h"

VTimeManager* VTimeManager::Instance = nullptr;

VTimeManager::VTimeManager()
{

}

VTimeManager::~VTimeManager()
{
	Clear(true);
}

bool VTimeManager::AnyActiveTimers()
{
	if (Instance != nullptr)
	{
		return Instance->Count() > 0;
	}

	return false;
}

void VTimeManager::Update(float dt)
{
	for (unsigned int i = 0; i < timers.size(); i++)
	{
		if (!timers[i]->IsPaused())
		{
			timers[i]->Update(dt);
		}
	}
}

void VTimeManager::AddTimer(VTimer* timer)
{
	timers.push_back(timer);
}

void VTimeManager::RemoveTimer(VTimer* timer)
{
	std::vector<VTimer*>::iterator i = std::find(timers.begin(), timers.end(), timer);

	if (i != timers.end())
	{
		timers.erase(i);
		delete *i;
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
		for (unsigned int i = 0; i < timers.size(); i++)
		{
			delete timers[i];
			timers[i] = nullptr;
		}
	}

	timers.clear();
}

int VTimeManager::Count()
{
	return (int)timers.size();
}

VTimer::VTimer(bool addToManager)
{
	running = true;
	time = 0.0f;

	if (addToManager)
		VTimeManager::p()->AddTimer(this);
}

float VTimer::Update(float dt)
{
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

int VTimer::Milliseconds()
{
	return static_cast<int>(time * 1000);
}

int VTimer::Microseconds()
{
	return Milliseconds() / 1000;
}