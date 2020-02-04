#include "VFiniteStateMachine.h"

class VFiniteState
{
	std::function<void(VBase*)> onEnter = nullptr;
	std::function<int(VBase*, float)> onUpdate = nullptr;
	std::function<void(VBase*, unsigned int)> onExit = nullptr;
	bool hasEntered = false;

public:
	VFiniteState() {}
	virtual ~VFiniteState() {}

	/**
	* Sets the functions to the state.
	* @param onUpdateFunc The function that gets called for each Update call. Needs to be a function that returns an integer (the next state ID) and passes in a VBase class and float (delta time) as a parameter. Required for the FSM to operate.
	* @param onEnterFunc The function that gets called when the FSM first enters a state. Needs to be a function that passes in a VBase object as a parameter. (optional)
	* @param onExitFunc The function that gets called when the FSM exits a state. Needs to be a function that passes in a VBase object as a parameter. (optional)
	*/
	void SetupFunctions(std::function<int(VBase*, float)> onUpdateFunc, std::function<void(VBase*)> onEnterFunc = nullptr, std::function<void(VBase*, unsigned int)> onExitFunc = nullptr);
	
	/**
	* Calls the Enter function of the state (if one is defined).
	* @param base The base object to pass into the Enter function.
	*/
	void CallEnter(VBase* base);

	/**
	* Calls the Update function of the state (if one is defined).
	* @param base The base object to pass into the Enter function.
	* @param dt The delta time to pass into the Update function.
	* @return The state ID number of the next state to enter. If the number is less than 0 (or the state ID does not exist) than the state will continue as normal.
	*/
	int CallUpdate(VBase* base, float dt);

	/**
	* Calls the Exit function of the state (if one is defined) and reset the state.
	* @param base The base object to pass into the Exit function.
	* @param state The next state the FSM is entering.
	*/
	void CallExit(VBase* base, unsigned int state);
};

VFiniteStateMachine::VFiniteStateMachine(VBase* baseObj, unsigned int MaxSize)
{
	finiteStates.resize(MaxSize);
	base = baseObj;
	state = 0;
}

unsigned int VFiniteStateMachine::GetCurrentState()
{
	return state;
}

void VFiniteStateMachine::SetNewState(unsigned int state)
{
	if (this->state != state && state < finiteStates.size())
	{
		finiteStates[this->state].CallExit(base, state);
		this->state = state;
		finiteStates[this->state].CallEnter(base);
	}
}

void VFiniteStateMachine::ResetState()
{
	finiteStates[state].CallExit(base, state);
	finiteStates[state].CallEnter(base);
}

void VFiniteStateMachine::Update(float dt)
{
	int newState = finiteStates[state].CallUpdate(base, dt);

	if (newState >= 0 && newState < finiteStates.size())
	{
		finiteStates[state].CallExit(base, newState);
		state = (unsigned int)newState;
	}
}

void VFiniteStateMachine::Add(unsigned int stateID, std::function<int(VBase*, float)> onUpdateFunc, std::function<void(VBase*)> onEnterFunc, std::function<void(VBase*, unsigned int)> onExitFunc)
{
	finiteStates[stateID].SetupFunctions(onUpdateFunc, onEnterFunc, onExitFunc);
}

void VFiniteState::SetupFunctions(std::function<int(VBase*, float)> onUpdateFunc, std::function<void(VBase*)> onEnterFunc, std::function<void(VBase*, unsigned int)> onExitFunc)
{
	onEnter = onEnterFunc;
	onUpdate = onUpdateFunc;
	onExit = onExitFunc;
}

void VFiniteState::CallEnter(VBase* base)
{
	if (onEnter != nullptr)
		onEnter(base);
}

int VFiniteState::CallUpdate(VBase* base, float dt)
{
	if (!hasEntered)
	{
		CallEnter(base);
		hasEntered = true;
	}

	return onUpdate(base, dt);
}

void VFiniteState::CallExit(VBase* base, unsigned int state)
{
	if (onExit != nullptr)
		onExit(base, state);

	hasEntered = false;
}
