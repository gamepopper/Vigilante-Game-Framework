#pragma once
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/System/String.hpp>
#include <SFML/Window/WindowStyle.hpp>
#include <SFML/Window/ContextSettings.hpp>
#include "VBase.h"

class VState;

class VGame : public VBase
{
public:
	VGame() = default;
	virtual ~VGame();
	/*
	Run a new game.
	title: Window Title
	initialState: First VState object to run.
	windowwidth: Width of window.
	windowheight: Height of window.
	fps: Framerate of window.
	flags: Window style flags
	context: OpenGL Context settings.
	*/
	int Run(const sf::String& title, VState* initialState, int windowwidth, int windowheight, float fps = 60, int flags = sf::Style::Default, const sf::ContextSettings& settings = sf::ContextSettings(32U));

	/*
	Run a new game.
	title: Window Title
	initialState: First VState object to run.
	windowwidth: Width of window.
	windowheight: Height of window.
	screenwidth: Width of game screen.
	screenheight: Height of game screen.
	fps: Framerate of window.
	flags: Window style flags
	context: OpenGL Context settings.
	*/
	int Run(const sf::String& title, VState* initialState, int windowwidth, int windowheight, int screenwidth, int screenheight, float fps = 60, int flags = sf::Style::Default, const sf::ContextSettings& = sf::ContextSettings(32U));

protected:
	virtual int Init();
	virtual void HandleEvents();
	virtual void Update(float dt);
	virtual void PreRender();
	virtual void Render();
	virtual void PostRender();
	virtual int Cleanup();


private:
	void ResizeCheck();

	sf::RenderTexture RenderTarget;

	bool cleaned = false;
	bool focused = true;
	int currentView = 0;
};

