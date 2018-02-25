/**
* @file    VAsync.h
* @author  Tim Stoddard <tim@gamepopper.co.uk>
*
* @section LICENSE
*
* MIT License
*
* Copyright (c) 2018 Tim Stoddard
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
* This is an asynchronous process handler that allows a game to run functions on separate threads using std::async and std::future.
*/

#pragma once
#include <queue>
#include <mutex>
#include <future>

///Handler for asynchronous functions.
class VAsync
{
private:
	///A queue of callback functions to call if a asynchronous function has finished.
	std::queue<std::function<void()>> syncQueue;
	///The mutex to lock and unlock the current thread.
	std::mutex syncMutex;
	///A total count of asynchronous functions currently being handled in VAsync.
	unsigned int activeAsyncFunctions = 0;

public:
	VAsync() = default;
	virtual ~VAsync();

	/**
	* @return If there are any asynchronous functions currently being handled, then the function returns true. Good for preventing certain actions from occuring while other threads are active (like state changes).
	*/
	bool ActiveAsyncFunctions();

	/**
	* @param f The std::future object that represents the asynchronous function (i.e. from calling std::async).
	*/
	void LaunchAsyncFunction(std::future<void> &f);

	/**
	* Function to tell the game's main thread to sync the current function back to the main one. This function MUST be called at the end of an asynchronous function.
	* @param callback A callback function used to respond to the notification that the asynchronous function has finished.
	*/
	void SyncToMainLoop(std::function<void()> callback);

	/**
	* Called in VGame to process all the sync requests from the SyncQueue.
	*/
	void ProcessSyncRequests();
};

