/**
* @file    VFiniteStateMachine.h
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
* Text rendering object that displays the text to follow a specified path.
*/

#pragma once
#include "VBase.h"

#include <vector>
#include <functional>

///The State Object for the FSM.
class VFiniteState;

///The Finite State Machine (FSM) Management class, which handles running each of the states.
class VFiniteStateMachine
{
private:
	VBase* base;
	unsigned int state;
	std::vector<VFiniteState> finiteStates;

public:

	/**
	* @param base A base object to set the behavior of. It can be anything that inherits from VBase.
	* @param maxSize The maximum number of states the FSM can have. Needs to be greater than zero to be used.
	*/
	VFiniteStateMachine(VBase* base, unsigned int maxSize);

	///@return The ID number of the currently active state in the FSM.
	unsigned int GetCurrentState();

	/**
	* Applys a set of functions to a specified State ID in the FSM.
	* @param stateID The ID number of the state you wish to apply the functions to.
	* @param onUpdateFunc The function that gets called for each Update call. Needs to be a function that returns a signed integer (the next state ID) and passes in a VBase class and float (delta time) as a parameter. Required for the FSM to operate.
	* @param onEnterFunc The function that gets called when the FSM first enters a state. Needs to be a function that passes in a VBase object as a parameter. (optional)
	* @param onExitFunc The function that gets called when the FSM exits a state. Needs to be a function that passes in a VBase object as a parameter. (optional)
	*/
	void Add(unsigned int stateID, std::function<int(VBase*,float)> onUpdateFunc, std::function<void(VBase*)> onEnterFunc = nullptr, std::function<void(VBase*)> onExitFunc = nullptr);

	/**
	* Updates the FSM.
	* @param dt Delta Time between the previous and current frame.
	*/
	virtual void Update(float dt);
};

