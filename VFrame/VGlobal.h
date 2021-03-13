/**
* @file    VGlobal.h
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
* A Singleton that contains the global properties for the game.
*/

#pragma once
#include <SFML/System.hpp>
#include <SFML/Config.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Window.hpp>
#include "VState.h"
#include "VContent.h"
#include "VMusic.h"
#include "VSoundManager.h"
#include "VRandom.h"
#include "VInputHandler.h"
#include "VAsync.h"
#include <vector>
#include <functional>
#include <memory>

#if _DEBUG
#include <iostream>
#include <sstream>
#endif

///Currently defined build version of the Vigilante Game Framework
#define VFRAME_VERSION "1.0.0.8"

class VBase;
class VObject;
class VPostEffectBase;
class VCollision;

///A singleton class that holds a lot of global properties other object require.
class VGlobal
{
private:
	static VGlobal* Instance; ///Singleton Instance of VGlobal
	VGlobal();
	~VGlobal();

public: 
	///Access and creation of VGlobal
	static VGlobal* p();
 
	///Window the game is rendered on.
	std::unique_ptr<sf::RenderWindow> App;
	 ///sf::RenderStates object used for rendering the target to the scene with additional features like transform and blend mode.
	sf::RenderStates RenderState;
	///Offset for the main game's viewport, important for the game window being resized and the screen needing to be repositioned in the window.
	sf::Vector2f ViewportOffset; 
	///The OpenGL Context Settings
	sf::ContextSettings ContextSettings; 
	///Should pixels be smooth or pixellated when the screen is resized?
	bool Antialiasing = false;
	///Should the game use Verticle Syncing (where the framerate is based on the monitor refresh rate) instead of the variable framerate?
	bool VSync = false;
#ifdef _DEBUG
	///Show the rectangle collision area of each collideable object.
	bool DrawDebug = false;
#endif
	///Colour of the Window's background prior to rendering.
	sf::Color BackgroundColor = sf::Color::Black;
	///Speed of GameTime by manipulating the Delta Time. (e.g. 1 is default speed, 2 is 2x speed)
	float TimeScale = 1.0f;
	///Set the game to pause when going out of focus.
	bool FocusPause = true;
	///VGame Area Width (Read Only)	
	unsigned int Width = 0;
	///VGame Area Height (Read Only)
	unsigned int Height = 0;
	///Width of the game's Window
	unsigned int WindowWidth = 0;
	///Height of the game's Window
	unsigned int WindowHeight = 0;
	///Bounderies of the game world regardless of state. Especially important for collision handling.
	sf::FloatRect WorldBounds; 
	///Allows streamed audio files to be played between states (typically for music).
	std::unique_ptr<VMusic> Music; 
	///Allows loaded audio files to be played between states (typically for sound effects).
	std::unique_ptr<VSoundManager> Sound;
	///Content Management System for handling asset files (i.e. Textures, Fonts ect)
	std::unique_ptr<VContent> Content;
	///PostEffect used for the entire game, applied after the state is drawn.
	std::unique_ptr<VPostEffectBase> PostProcess;
	///Frames Per Second - used to calculate average framerate
	float FPS = 0;
	///Value is true if the current VState is being changed in the state manager.
	bool IfChangedState = false;
	///Value is true if the a new VState is being pushed onto the stack in the state manager.
	bool IfPushedState = false;
	///Value is true if user wants to force the game to render a single frame, regardless of state handling or frame delay.
	bool ForceFrame = false;
	///Random Number Generator
	std::unique_ptr<VRandom> Random;
	///Input Handling for Keyboard, Mouse and Controllers.
	std::unique_ptr<VInputHandler> Input;
	///Asynchronous Function Handler for Multithreaded Functionality.
	std::unique_ptr<VAsync> Async;
	///Current Window Style being used (set with sf::Style).
	int WindowStyle;
	///@param newTitle The new text the window should display in the title bar.
	void SetTitle(const char* newTitle);
	///@param newTitle The new text the window should display in the title bar.
	void SetTitle(std::string newTitle);
	///@return The current text on the window's title bar.
	const char* GetTitle();
	///@param set Set the game to fullscreen mode or not.
	void SetFullscreen(bool set);
	///Toggles from fullscreen to window mode.
	void ToggleFullscreen();
	///@return If the game is in fullscreen mode, function returns true.
	bool IsFullscreen();
	///@return True if the game is running.
	bool IsRunning();
	///@return Position of the mouse relative to the game screen (even when resized).
	sf::Vector2f GetMousePosition();
	/// @param set Sets mouse cursor to be visible or not, depending of if you want an in-game cursor.
	void SetMouseCursorVisible(bool set);
	/// @return The Current Gameplay State.
	VState* CurrentState();
	/// @param state The new VState to change to.
	void ChangeState(VState* state);
	/// @param state A new VState to push onto the stack to run without changing the current one.
	void PushState(VState* state);
	/// Pop the current state from the stack.
	void PopState();
	/// Clear all states from the stack.
	void ClearState();
	/// Close window and close out game.
	void Exit();

	/**
	* Test if an object is overlapping at a specified point.
	* @param point The position in the game world to test with.
	* @param a The base object to test overlap with.
	* @return True if there is an overlap.
	*/
	bool OverlapAtPoint(const sf::Vector2f& point, VBase* a);

	/**
	* Test if two objects are overlapping.
	* @param a The base object to test overlap with.
	* @param b The other base object to test overlap with. If you only want to test overlap with a itself (like in a VGroup), set this to null.	
	* @param responseCall An optional response callback if an overlap is found. Only gets called if an overlap is true.
	* @param processCall An optional process callback if an overlap is found and there is a need to handle further tests and changes to the objects. Only gets called if an overlap is true, and should return true further collision tests are true.
	* @return Function returns true if an overlap has been found to be true.
	*/
	bool Overlaps(VBase* a, VBase* b = nullptr, std::function<void(VObject*, VObject*)>const& responseCall = nullptr, std::function<bool(VObject*, VObject*)>const& processCall = nullptr);

	/**
	* Test if two objects are colliding by testing an overlap and applying the VObject::separate function as a process call to separate the objects.
	* @param a The base object to test overlap with.
	* @param b The other base object to test overlap with. If you only want to test overlap with a itself (like in a VGroup), set this to null.
	* @param responseCall An optional response callback if an overlap is found. Only gets called if an overlap is true.
	* @return Function returns true if an overlap has been found to be true.
	*/
	bool Collides(VBase* a, VBase* b = nullptr, std::function<void(VObject*, VObject*)>const& responseCall = nullptr);

	/**
	* Test if an object is overlapping at a specified point using circle overlap testing.
	* @param point The position in the game world to test with.
	* @param a The base object to test overlap with.
	* @return True if there is an overlap.
	*/
	bool OverlapCircleAtPoint(const sf::Vector2f& point, VBase* a);

	/**
	* Test if two objects are overlapping using circle overlap testing.
	* @param a The base object to test overlap with.
	* @param b The other base object to test overlap with. If you only want to test overlap with a itself (like in a VGroup), set this to null.
	* @param responseCall An optional response callback if an overlap is found. Only gets called if an overlap is true.
	* @param processCall An optional process callback if an overlap is found and there is a need to handle further tests and changes to the objects. Only gets called if an overlap is true, and should return true further collision tests are true.
	* @return Function returns true if an overlap has been found to be true.
	*/
	bool OverlapsCircle(VBase* a, VBase* b = nullptr, std::function<void(VObject*, VObject*)>const& responseCall = nullptr, std::function<bool(VObject*, VObject*)>const& processCall = nullptr);

	/**
	* Test if two objects are colliding by testing an overlap with circle overlap testing and applying the VObject::separateCircle function as a process call to separate the objects.
	* @param a The base object to test overlap with.
	* @param b The other base object to test overlap with. If you only want to test overlap with a itself (like in a VGroup), set this to null.
	* @param responseCall An optional response callback if an overlap is found. Only gets called if an overlap is true.
	* @return Function returns true if an overlap has been found to be true.
	*/
	bool CollidesCircle(VBase* a, VBase* b = nullptr, std::function<void(VObject*, VObject*)>const& responseCall = nullptr);

	///Cleans up all data in VGlobal to NULL.
	static void Cleanup();

private:
	std::unique_ptr<VStateManager> gsm; ///The game's state manager.
	VState* nextState = nullptr; ///Store the next state to apply until the current state has finished processing.
	bool fullscreen = false; ///Sets for when the game is in fullscreen mode or not.
	bool mouseCursorVisible = true; ///Sets for when the system mouse cursor is visible.
	bool running = true;
	const char* title = "";

	std::unique_ptr<VCollision> collision; ///The game's collision handling system.
	std::function<bool(VObject*, VObject*)> rectCollision; ///Function for handling rectangle collisions.
	std::function<bool(VObject*, VObject*)> circleCollision; ///Function for handling circle collisions.
};

