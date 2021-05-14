/**
* @file    VBase.h
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
* Game state and state manager.
*/

#pragma once
#include "VGroup.h"
#include "VCamera.h"
#include "VTimer.h"
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <functional>

#if _DEBUG
#include <iostream>
#endif

class VSubState;
///Processes the game's current scene as well as 2D cameras and substates.
class VState : public VGroup
{
private:
	bool openSubState = false;
	bool closeSubstate = false;
	std::unique_ptr<VSubState> subState = nullptr;

public:
	///Used to call parent class functions when they are overrided in class.
	typedef VGroup VSUPERCLASS;
	VState();
	virtual ~VState();

	///List of cameras to use.
	std::vector<VCamera*> Cameras;
	///Pointer to first camera in array.
	VCamera* DefaultCamera = nullptr;
	///Local TimeManager of the main state (Useful if you want timers that are exclusively controlled by a specific state).
	std::unique_ptr<VTimeManager> TimeManager = nullptr;

	///Substate of main state.
	VSubState* SubState() const;
	///@param subState The VSubState to open.
	void OpenSubState(VSubState* subState);
	///Close a current substate.
	void CloseSubState();
	///Handles the opening and closing of substates.
	void ResetSubState();

	///Initialises new state and sets up the default camera, first function called before changing states.
	virtual void Initialise();
	///Cleanup state data, last function called before destruction of state.
	virtual void Cleanup();
	///Function called when pausing state (when pushing a new state onto VStateManager).
	virtual void Pause() {}
	///Function called when resuming state (after higher stake is poped from the VStateManager).
	virtual void Resume() {}
	///@param event The current event that can be processed.
	virtual void HandleEvents(const sf::Event& event);

	/**
	* Update the state.
	* @param dt The delta time.
	*/
	virtual void Update(float dt);

	/**
	* If overridden, allows the opportunity to render objects directly to the entire window before the scene is rendered.
	* @param RenderTarget The game's sf::RenderWindow, after the scene had been rendered and post-processed effects had been applied.
	*/
	virtual void PreDraw(sf::RenderTarget& RenderTarget);

	/**
	* If overridden, allows the opportunity to render objects directly to the entire window after the scene is rendered.
	* @param RenderTarget The game's sf::RenderWindow, after the scene had been rendered and post-processed effects had been applied.
	*/
	virtual void PostDraw(sf::RenderTarget& RenderTarget);
};

///A different kind of state class that would update and render over the top of the main state. Good for pause screens, menus, battle screens and even GUI.
class VSubState : public VGroup
{
protected:
	///The vertex data to render the colour fade.
	sf::VertexArray vertices;

public:
	///Used to call parent class functions when they are overrided in class.
	typedef VGroup VSUPERCLASS;

	///Callback function that can be called when VState::CloseSubState is called.
	std::function<void()> OnClose = nullptr;
	///The VState object that the substate has been opened from.
	VState* ParentState = nullptr;
	///If true, the substate's transform is based on the parent state's camera.
	bool UseParentCamera = false;
	///Local TimeManager of the substate.
	std::unique_ptr<VTimeManager> TimeManager = nullptr;

	/**
	* @param colour The background colour of the substate.
	*/
	VSubState(sf::Color colour = sf::Color::Transparent);
	virtual ~VSubState();

	///Closes the substate.
	void Close();
	///@param color The tint colour of the substate's background.
	void SetFillColour(const sf::Color& color);
	///Initialise the substate and its contents.
	virtual void Initialise() {}
	///Destroys all content in the substate before cleaning it up.
	virtual void Cleanup();
	///@param event The current event that can be processed.
	virtual void HandleEvents(const sf::Event& event) {}

	/**
	* Update the substate.
	* @param dt The delta time.
	*/
	virtual void Update(float dt);

	/**
	* If overridden, allows the opportunity to render objects directly to the entire window before the scene is rendered.
	* @param RenderTarget The game's sf::RenderWindow, after the scene had been rendered and post-processed effects had been applied.
	*/
	virtual void PreDraw(sf::RenderTarget& RenderTarget) {}

	/**
	* Draws the substate's background and the content of the substate.
	* @param RenderTarget The game's sf::RenderTarget object to render data onto.
	*/
	virtual void Draw(sf::RenderTarget &RenderTarget);

	/**
	* If overridden, allows the opportunity to render objects directly to the entire window after the scene is rendered.
	* @param RenderTarget The game's sf::RenderWindow, after the scene had been rendered and post-processed effects had been applied.
	*/
	virtual void PostDraw(sf::RenderTarget& RenderTarget) {}
};

///Manager class for VStates, allows switching current states or adding states to a stack.
class VStateManager
{
public:
	VStateManager() = default;
	~VStateManager()
	{
		Clear();
	}

	///Gets current state.
	VState* CurrentState();

	/**
	* Change current top state to a new state.
	* @param state the VState to change to.
	*/
	void ChangeState(VState* state);

	/**
	* Pushes new state to the top of the stack.
	* @param state the VState to add to the stack.
	*/
	void PushState(VState* state);

	///Pops the top state off the stack.
	void PopState();
	///Clears all states from the stack.
	void Clear();

protected:
	VStateManager(VStateManager const&) = delete;
	void operator=(VStateManager const&) = delete;
	///List of VStates in the manager.
	std::vector<VState*> states;
};