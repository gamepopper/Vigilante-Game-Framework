/**
* @file    VFiniteStateMachine.h
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
* An object for handling a Finite State machine.
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
	void* userData;
	unsigned int state;
	std::vector<VFiniteState> finiteStates;

public:

	/**
	* @param base A base object to set the behavior of. It can be anything that inherits from VBase.
	* @param maxSize The maximum number of states the FSM can have. Needs to be greater than zero to be used.
	* @param data A custom data pointer that could be set to anything usable within the game states. Defaults as nullptr.
	*/
	VFiniteStateMachine(VBase* base, unsigned int maxSize, void* data = nullptr);
	~VFiniteStateMachine();

	///@return The ID number of the currently active state in the FSM.
	unsigned int GetCurrentState();

	///Exits the current state and enters the new state of the FSM from outside. Only works if the new state is not the current one, use ResetState() to exit and re-enter the current state.
	///@param state The ID number of the new state to run in the FSM.
	void SetNewState(unsigned int state);

	///Resets the currently running state in the FSM.
	void ResetState();

	/**
	* Applys a set of functions to a specified State ID in the FSM.
	* @param stateID The ID number of the state you wish to apply the functions to.
	* @param onUpdateFunc The function that gets called for each Update call. Needs to be a function that returns a signed integer (the next state ID) and passes in a VBase class and float (delta time) as a parameter. Required for the FSM to operate.
	* @param onEnterFunc The function that gets called when the FSM first enters a state. Needs to be a function that passes in a VBase object as a parameter. (optional)
	* @param onExitFunc The function that gets called when the FSM exits a state. Needs to be a function that passes in a VBase object as a parameter. (optional)
	*/
	void Add(unsigned int stateID, std::function<int(VBase*, float, void*)> onUpdateFunc, std::function<void(VBase*, void*)> onEnterFunc = nullptr, std::function<void(VBase*, unsigned int, void*)> onExitFunc = nullptr);

	/**
	* Updates the FSM.
	* @param dt Delta Time between the previous and current frame.
	*/
	virtual void Update(float dt);
};

