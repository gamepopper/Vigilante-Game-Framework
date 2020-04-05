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
	///If it's currently running.
	bool running = true;
	///Current time in seconds.
	float time = 0.0f;

public:
	///@param addToManager If true, this will automatically add the timer to VTimeManager. If it's not added to the VTimeManager it has to be updated manually.
	VTimer(bool addToManager = true);
	~VTimer();

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

///Special VTimer that can perform events at set intervals. VTimer functions are usable, but the new functions need to be called in order for events to take effect.
class VTimeline : public VTimer
{
private:
	bool loop = false;
	bool ready = false;
	bool destroyWhenDone = false;
	unsigned int lastEvent = 0;

public:
	///Used to call parent class functions when they are overrided in class.
	typedef VTimer VSUPERCLASS;

	/**
	* @param looping If true, the timeline will start at the beginning once finished.
	* @param addToManager If true, this will automatically add the timer to VTimeManager. If it's not added to the VTimeManager it has to be updated manually.
	*/
	VTimeline(bool looping, bool addToManager = true);

	///An event struct that holds the function to call as well as the point in time for the event to occur.
	struct VTimelineEvent
	{
		///The point in time to run the event in milliseconds.
		unsigned int timePoint;
		///The function to call to perform the event.
		std::function<void()> function;
		///Flag to indicate that the event has occured in the timeline.
		bool done;
		
		/**
		* @param time The point in time to run the event in milliseconds.
		* @param function The function to call to perform the event.
		*/
		VTimelineEvent(unsigned int time, std::function<void()> function)
		{
			this->timePoint = time;
			this->function = function;
			done = false;
		}
	};

	/**
	* Adds an event to the timeline.
	* @param time The point in time to run the event in milliseconds.
	* @param function The function to call to perform the event.
	*/
	void AddEvent(unsigned int time, std::function<void()> function);

	/**
	* Adds an event to the timeline.
	* @param time The point in time to run the event in seconds.
	* @param function The function to call to perform the event.
	*/
	void AddEvent(float time, std::function<void()> function);

	/**
	* Starts the timeline.
	* @param clearWhenFinished If true, all the events will be removed when the timeline is finished.
	*/
	void Start(bool clearWhenFinished);

	///Stop the timeline (resets the time.
	void Stop();

	///Returns true if the timeline has played through all events.
	bool IsFinished();

	///@param value If true, the timeline will start from the beginning instead of stopping and/or clearing.
	void SetLooping(bool value);
	///@return If the timeline is looping or not.
	bool GetLooping();

	/**
	* Updates the time.
	* @param dt Delta Time to increment the time by.
	* @return The resulting time once updated.
	*/
	virtual float Update(float dt);

private:
	std::vector<std::unique_ptr<VTimelineEvent>> events;
};

///Manages multiple timers at once in a singleton data structure.
class VTimeManager
{
private:
	///Singleton Instance
	static VTimeManager* Instance;
	VTimeManager();
	~VTimeManager();

	///List of VTimer objects.
	std::vector<VTimer*> timers;

public:
	///@return True if any timers are currently active.
	static bool AnyActiveTimers();

	///@returns The singleton instance as a pointer.
	static VTimeManager* p();

	/**
	* Updates all timers.
	* @param dt Delta Time to increment the time by.
	*/
	void Update(float dt);

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
