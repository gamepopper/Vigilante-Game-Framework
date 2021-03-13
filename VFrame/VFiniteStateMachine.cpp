#include "VFiniteStateMachine.h"

class VFiniteState
{
	std::function<void(VBase*, void*)> onEnter = nullptr;
	std::function<int(VBase*, float, void*)> onUpdate = nullptr;
	std::function<void(VBase*, unsigned int, void*)> onExit = nullptr;
	bool hasEntered = false;

public:
	VFiniteState() {}
	virtual ~VFiniteState() {}

	/**
	* Sets the functions to the state.
	* @param onUpdateFunc The function that gets called for each Update call. Needs to be a function that returns an integer (the next state ID) and passes in a VBase class, custom data and float (delta time) as a parameter. Required for the FSM to operate.
	* @param onEnterFunc The function that gets called when the FSM first enters a state. Needs to be a function that passes in a VBase object and custom data as parameters. (optional)
	* @param onExitFunc The function that gets called when the FSM exits a state. Needs to be a function that passes in a VBase object and custom data as parameters. (optional)
	*/
	void SetupFunctions(std::function<int(VBase*, float, void*)> onUpdateFunc, std::function<void(VBase*, void*)> onEnterFunc = nullptr, std::function<void(VBase*, unsigned int, void*)> onExitFunc = nullptr);
	
	/**
	* Calls the Enter function of the state (if one is defined).
	* @param base The base object to pass into the Enter function.
	*/
	void CallEnter(VBase* base, void* data);

	/**
	* Calls the Update function of the state (if one is defined).
	* @param base The base object to pass into the Enter function.
	* @param dt The delta time to pass into the Update function.
	* @return The state ID number of the next state to enter. If the number is less than 0 (or the state ID does not exist) than the state will continue as normal.
	*/
	int CallUpdate(VBase* base, float dt, void* data);

	/**
	* Calls the Exit function of the state (if one is defined) and reset the state.
	* @param base The base object to pass into the Exit function.
	* @param state The next state the FSM is entering.
	*/
	void CallExit(VBase* base, unsigned int state, void* data);
};

VFiniteStateMachine::VFiniteStateMachine(VBase* baseObj, unsigned int MaxSize, void* data)
{
	finiteStates.resize(MaxSize);
	base = baseObj;
	userData = data;
	state = 0;
}

VFiniteStateMachine::~VFiniteStateMachine()
{
	finiteStates.clear();
}

unsigned int VFiniteStateMachine::GetCurrentState()
{
	return state;
}

void VFiniteStateMachine::SetNewState(unsigned int state)
{
	if (this->state != state && state < finiteStates.size())
	{
		finiteStates[this->state].CallExit(base, state, userData);
		this->state = state;
		finiteStates[this->state].CallEnter(base, userData);
	}
}

void VFiniteStateMachine::ResetState()
{
	finiteStates[state].CallExit(base, state, userData);
	finiteStates[state].CallEnter(base, userData);
}

void VFiniteStateMachine::Update(float dt)
{
	int newState = finiteStates[state].CallUpdate(base, dt, userData);

	if (newState >= 0 && newState < (int)finiteStates.size())
	{
		finiteStates[state].CallExit(base, newState, userData);
		state = (unsigned int)newState;
	}
}

void VFiniteStateMachine::Add(unsigned int stateID, std::function<int(VBase*, float, void*)> onUpdateFunc, std::function<void(VBase*, void*)> onEnterFunc, std::function<void(VBase*, unsigned int, void*)> onExitFunc)
{
	finiteStates[stateID].SetupFunctions(onUpdateFunc, onEnterFunc, onExitFunc);
}

void VFiniteState::SetupFunctions(std::function<int(VBase*, float, void*)> onUpdateFunc, std::function<void(VBase*, void*)> onEnterFunc, std::function<void(VBase*, unsigned int, void*)> onExitFunc)
{
	onEnter = onEnterFunc;
	onUpdate = onUpdateFunc;
	onExit = onExitFunc;
}

void VFiniteState::CallEnter(VBase* base, void* data)
{
	if (onEnter != nullptr)
		onEnter(base, data);
}

int VFiniteState::CallUpdate(VBase* base, float dt, void* data)
{
	if (!hasEntered)
	{
		CallEnter(base, data);
		hasEntered = true;
	}

	if (onUpdate)
		return onUpdate(base, dt, data);

	return -1;
}

void VFiniteState::CallExit(VBase* base, unsigned int state, void* data)
{
	if (onExit != nullptr)
		onExit(base, state, data);

	hasEntered = false;
}
