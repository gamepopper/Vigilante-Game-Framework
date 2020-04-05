#include "VTimer.h"

VTimeManager* VTimeManager::Instance = nullptr;

VTimeManager::VTimeManager()
{

}

VTimeManager::~VTimeManager()
{
	Clear(true);
}

VTimeManager* VTimeManager::p()
{
	if (!Instance)
	{
		Instance = new VTimeManager();
	}

	return Instance;
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
		for (int i = 0; i < (int)timers.size(); i++)
		{
			delete timers[i];
			i--;
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

VTimer::~VTimer()
{
	VTimeManager::p()->RemoveTimer(this);
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

unsigned int VTimer::Milliseconds()
{
	return static_cast<unsigned int>(time * 1000);
}

uint64_t VTimer::Microseconds()
{
	return static_cast<uint64_t>(time * 1000 * 1000);
}

VTimeline::VTimeline(bool looping, bool addToManager) : VTimer(addToManager)
{
}

void VTimeline::AddEvent(unsigned int time, std::function<void()> function)
{
	events.emplace_back(std::make_unique<VTimelineEvent>(time, function));

	if (lastEvent < time)
		lastEvent = time;
}

void VTimeline::AddEvent(float time, std::function<void()> function)
{
	AddEvent(static_cast<unsigned int>(time * 1000), function);
}

void VTimeline::Start(bool clearWhenFinished)
{
	Restart();
	destroyWhenDone = clearWhenFinished;
}

void VTimeline::Stop()
{
	ready = false;
}

bool VTimeline::IsFinished()
{
	return ready && Milliseconds() >= lastEvent;
}

void VTimeline::SetLooping(bool value)
{
	loop = value;
}

bool VTimeline::GetLooping()
{
	return loop;
}

float VTimeline::Update(float dt)
{
	float t = VSUPERCLASS::Update(dt);

	if (!ready)
		return t;

	for (unsigned int i = 0; i < events.size(); i++)
	{
		if (!events[i]->done && events[i]->timePoint <= Milliseconds())
		{
			events[i]->function();
			events[i]->done = true;
		}
	}

	if (IsFinished())
	{
		ready = false;
		
		if (loop)
		{
			Start(false);
		}
		else if (destroyWhenDone)
		{
			events.clear();
		}
	}

	return t;
}
