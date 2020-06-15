/**
* @file    VTimer.h
* @author  Tim Stoddard <tim@gamepopper.co.uk>
*
* @section LICENSE
*
* MIT License
*
* Copyright (c) 2020 Tim Stoddard
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*
* @section DESCRIPTION
*
* VTimer updates and gets the amount of time since the object's creation or since Restart() is called.
* VTimeline is a special timer that also calls events at points in time.
* VTimeManager updates VTimer objects, as a singleton it can be updated even after 
*/

#pragma once
#include <vector>
#include <functional>
#include <memory>

#ifdef __linux__
#include "stddef.h"
#endif

///Single timer class for measuring the length of time between function calls.
class VTimer
{
private:
	bool running = true;
	float time = 0.0f;

public:
	VTimer();
	virtual ~VTimer();

	/**
	* Updates the time.
	* @param dt Delta Time to increment the time by.
	* @return The resulting time once updated.
	*/
	virtual float Update(float dt);

	///@return True if the timer is currently paused.
	bool IsPaused();

	/**
	* Restarts timer at 0.
	* @return The time in seconds before the function was called.
	*/
	float Restart();
	///Pauses the timer.
	void Pause();
	///Resumes timer at current point.
	void Resume();

	///@return Time in seconds.
	float Seconds();
	///@return Time in milliseconds. (Millisecond = 1/1000th of a second)
	unsigned int Milliseconds();
	///@return Time in microseconds. (Microsecond = 1/1000th of a millisecond)
	uint64_t Microseconds();
};

///Special VTimer that can perform an event at a set interval. Like VTimer, it can be paused or restarted at any point. It can also be looped.
class VTimerEvent : public VTimer
{
private:
	bool loop = false;
	unsigned int timePoint = 0;
	std::function<void()> function = nullptr;

public:
	///Used to call parent class functions when they are overrided in class.
	typedef VTimer VSUPERCLASS;

	/**
	* @param time Time in milliseconds for the event to occur.
	* @param eventFunction The event to call.
	* @param looping If true, the timeline will start at the beginning once finished, otherwise it will destroy itself.
	*/
	VTimerEvent(unsigned int time, std::function<void()> eventFunction, bool looping = false);
	virtual ~VTimerEvent();

	///Stops the timer and removes its event function.
	void Stop();

	///@param value If true, the timeline will start from the beginning instead of being destroyed.
	void SetLooping(bool value);
	///@return If the timeline is looping or not.
	bool GetLooping();

	/**
	* Updates the time.
	* @param dt Delta Time to increment the time by.
	* @return The resulting time once updated.
	*/
	virtual float Update(float dt);
};

/**
* Manages multiple timers at once in a singleton data structure.
* An independent VTimeManager can also be created, however any VTimer or VTimeEvent objects must be constructed with the AddToManager parameter set to false.
*/
class VTimeManager
{
private:
	std::vector<VTimer*> timers;

public:
	///Constructor
	VTimeManager();
	///Destructor
	~VTimeManager();

	///@return The VTimer object that was added directly to the time manager.
	VTimer* AddTimer();

	/**
	* @param time Time in milliseconds for the event to occur.
	* @param eventFunction The event to call.
	* @param looping If true, the timeline will start at the beginning once finished, otherwise it will destroy itself.
	* @return The VTimerEvent object that was added directly to the time manager.
	*/
	VTimerEvent* AddTimerEvent(unsigned int time, std::function<void()> eventFunction, bool looping = false);

	/**
	* Adds a timer from the list.
	* @param timer The VTimer object to add to the list.
	*/
	void AddTimer(VTimer* timer);

	/**
	* Removes a timer from the list.
	* @param timer The VTimer object to remove from the list.
	*/
	void RemoveTimer(VTimer* timer);

	/**
	* Updates all timers.
	* @param dt Delta Time to increment the time by.
	*/
	void Update(float dt);

	/**
	* Reserves a set amount of timers in the list, good for optimization purposes.
	* @param size The amount of timers to reserve in the list.
	*/
	void Reserve(size_t size);

	/**
	* Clears timer list.
	* @param destroy Destroys all timers in the list if true, else it'd need to be done manually.
	*/
	void Clear(bool destroy = true);

	///@return Amount of timers in the manager.
	int Count();
};
