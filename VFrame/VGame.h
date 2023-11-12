/**
* @file    VGame.h
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
* Main class that runs a game.
*/

#pragma once

#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/System/String.hpp>
#include <SFML/Window/WindowStyle.hpp>
#include <SFML/Window/ContextSettings.hpp>
#include <memory>
#include "VBase.h"

class VState;
class VCamera;

///The main game class that initialises and cleans up data as well as runs the game loop.
class VGame
{
public:
	VGame() = default;
	virtual ~VGame();

	/**
	* Runs a new game.
	*
	* @param  title Window Title.
	* @param  initialState First VState object to run the game with. Cannot be NULL.
	* @param  windowwidth Width of the Game Window.
	* @param  windowheight Height of the Game Window.
	* @param  fps Framerate in Frames Per Second.
	* @param  flags Window Style Flags using one or more of the sf::Style flags. Default = Titlebar | Resize | Close
	* @param  settings OpenGL Context settings. Default = 0 Depth + 0 Stencil + Version 1.1 + Default Attributes + No sRGB
	* @return An integer returning the result of the game. If 0, there were no errors.
	*/
	int Run(const sf::String& title, VState* initialState, int windowwidth, int windowheight, float fps = 60, int flags = sf::Style::Default, const sf::ContextSettings& settings = sf::ContextSettings());


	/**
	* Runs a new game.
	*
	* @param  title Window Title.
	* @param  initialState First VState object to run the game with. Cannot be NULL.
	* @param  windowwidth Width of the Game Window.
	* @param  windowheight Height of the Game Window.
	* @param  screenwidth Width of the Game Screen.
	* @param  screenheight Height of the Game Screen.
	* @param  fps Framerate in Frames Per Second.
	* @param  flags Window Style Flags using one or more of the sf::Style flags. Default = Titlebar | Resize | Close
	* @param  settings OpenGL Context settings. Default = 0 Depth + 0 Stencil + Version 1.1 + Default Attributes + No sRGB
	* @return An integer returning the result of the game. If 0, there were no errors.
	*/
	int Run(const sf::String& title, VState* initialState, int windowwidth, int windowheight, int screenwidth, int screenheight, float fps = 60, int flags = sf::Style::Default, const sf::ContextSettings& settings = sf::ContextSettings());

protected:

	/**
	* Initialises the game, such as setting up the window and initial state for example.
	*
	* @return An integer returning the result of the intialisation. If 0, there were no errors, else will result in the game automatically closing and output the result.
	*/
	virtual int Init();

	/**
	* Handle any polled events in the form of sf::Event objects during the game loop. Some events such as sf::Event::Close and sf::Event::Resize are handled in VGame.
	*/
	virtual void HandleEvents();

	/**
	* Runs the update stage of the game loop.
	*
	* @param  dt Delta Time between the previous and current update. Value depends on whether VSync is enabled or not and what the fps value is set to.
	*/
	virtual void Update(float dt);

	/**
	* Prepares the game for rendering in the game loop, specifically clearing the current window.
	*/
	virtual void PreRender();

	/**
	* Renders the game state, called for each camera currently active in a state.
	*
	* @param  camera The current camera object being rendered to.
	*/
	virtual void Render(VCamera* camera);

	/**
	* Applies the rendered scene to the window, after applying any post effects currently enabled.
	*/
	virtual void PostRender();

	/**
	* Cleans up any remaining game data, including all global properties. Occurs when the game will shut down.
	*
	* @return An integer returning the result of the cleanup. If 0, there were no errors, else will output the result.
	*/
	virtual int Cleanup();


private:

	/**
	* Checks to see if the screen needs to be resized to fit the current window.
	*/
	void ResizeCheck();

	///Draws debug bounds if visible.
	VBase* debugRenderer = nullptr;
	///The screen the game is rendered to before being applied to the window.
	std::unique_ptr<sf::RenderTexture> renderTarget;
	///The vertices used to render the screen to a window.
	sf::VertexArray vertexArray;
	///Used to check if the game has been properly cleaned up before shutdown.
	bool cleaned = false;
	///Used in event handling to check if the game is currently in focus. Stops update and render if false.
	bool focused = true;
	///Check the orientation
	unsigned char orientation = 0;
};