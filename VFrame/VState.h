#pragma once
#include "VGroup.h"
#include "VCamera.h"
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <functional>

#if _DEBUG
#include <iostream>
#endif

class VState;
class VSubState;

class VState : public VGroup
{
private:
	bool openSubState = false;
	bool closeSubstate = false;

public:
	typedef VGroup VSUPERCLASS;
	VState() : VGroup() 
	{
		Cameras.emplace_back();
		Cameras[0] = new VCamera();
		DefaultCamera = Cameras[0];
	}
	virtual ~VState() 
	{
		Cleanup();
	}

	//List of cameras to use.
	std::vector<VCamera*> Cameras;
	//Pointer to first camera in array.
	VCamera* DefaultCamera = nullptr;
	//Substate of main state (Useful for stuff where you want activity to be separate from main game).
	VSubState* SubState = nullptr;

	//Open a new substate.
	void OpenSubState(VSubState* subState);
	//Close a current substate.
	void CloseSubState();
	//Handles the opening and closing of substates.
	void ResetSubState();

	//Initialises new state, first function called before changing states.
	virtual void Initialise() {}
	//Cleanup state data, last function called before destruction of state.
	virtual void Cleanup();
	//Function called when pausing state (when pushing a new state onto VStateManager).
	virtual void Pause() {}
	//Function called when resuming state (after higher stake is poped from the VStateManager).
	virtual void Resume() {}
	//Handle event calls.
	virtual void HandleEvents(const sf::Event& event) {}
};

class VSubState : public VGroup
{
protected:
	sf::RectangleShape bgRect;

public:
	std::function<void()> OnClose = nullptr;
	VState* ParentState = nullptr;
	bool UseParentCamera = false;

	typedef VGroup VSUPERCLASS;
	VSubState(sf::Color colour = sf::Color::Transparent) : VGroup()
	{
		bgRect.setFillColor(colour);
	}
	virtual ~VSubState()
	{
		Cleanup();
		ParentState = nullptr;
	}

	//Closes the substate.
	void Close();

	virtual void Initialise() {}
	virtual void Cleanup();
	virtual void HandleEvents(const sf::Event& event) {}
	virtual void Draw(sf::RenderTarget &RenderTarget);
};

class VStateManager
{
public:
	VStateManager() = default;
	~VStateManager()
	{
		Clear();
	}

	//Gets current state.
	VState* CurrentState();
	//Change current top state to a new state.
	void ChangeState(VState* state);
	//Pushes new state to the top of the stack.
	void PushState(VState* state);
	//Pops the top state off the stack.
	void PopState();
	//Clears all states from the stack.
	void Clear();

protected:
	VStateManager(VStateManager const&) = delete;
	void operator=(VStateManager const&) = delete;
	std::vector<VState*> states;
};