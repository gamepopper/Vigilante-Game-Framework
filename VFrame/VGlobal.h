#pragma once
#include <SFML/System.hpp>
#include <SFML/Config.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Window.hpp>
#include "VState.h"
#include "VContent.h"
#include "VObject.h"
#include "VMusic.h"
#include "VSoundManager.h"
#include "VRandom.h"
#include "VInputHandler.h"
#include "VPostEffect.h"
#include "VAsync.h"
#include <vector>
#include <functional>

#if _DEBUG
#include <iostream>
#include <sstream>
#endif

#define VFRAME_VERSION "0.9.9.5"

class VGlobal
{
private:
	static VGlobal* instance;
	VGlobal()
	{
		gsm = new VStateManager();
		Content = new VContent();
		Music = new VMusic();
		Sound = new VSoundManager();
	}

	~VGlobal()
	{
		if (Content)
		{
			delete Content;
			Content = NULL;
		}
		if (Music)
		{
			delete Music;
			Music = NULL;
		}
		if (Sound)
		{
			delete Sound;
			Sound = NULL;
		}
		if (PostProcess)
		{
			delete PostProcess;
			PostProcess = NULL;
		}
		if (gsm)
		{
			delete gsm;
			gsm = NULL;
		}
	}

public:
	static VGlobal* p()
	{
		if (!instance)
		{
			instance = new VGlobal();
		}

		return instance;
	}

	//App that handles all rendering.
	sf::RenderWindow App;
	//RenderState for entire rendered scene.
	sf::RenderStates RenderState;
	//Sprite object of the last rendered frame.
	sf::Sprite RenderSprite;
	sf::ContextSettings ContextSettings;

	//Whether the rendering will appear smooth or pixelated when scaling.
	bool Antialiasing = false;
	//VSync
	bool VSync = false;
	//Whether to render collision boxes for debugging perposes
	bool DrawDebug = false;

	//Colour of the background prior to rendering.
	sf::Color BackgroundColor = sf::Color::Black;

	//Speed of GameTime (1 is default speed, 2 is 2x speed)
	float TimeScale = 1.0f;

	//VGame Title (Use Window->SetTitle to show it on window)
	sf::String Title = "";

	//Pause Game When it Looses Focused
	bool FocusPause = true;

	//VGame Area Width (Read Only)
	unsigned int Width = 0;
	//VGame Area Height (Read Only)
	unsigned int Height = 0;
	//Window Width
	unsigned int WindowWidth = 0;
	//Window Height
	unsigned int WindowHeight = 0;
	//World Bounds - used to bound camera to world area.
	sf::FloatRect WorldBounds;
	//Music - play streamed music (particularly between states).
	VMusic* Music = NULL;
	//Sound - play loaded sounds.
	VSoundManager* Sound = NULL;

	VContent* Content = NULL;
	VPostEffectBase* PostProcess = NULL;

	//Frames Per Second - used to calculate average framerate
	float FPS = 0;

	bool IfChangedState = false;
	bool IfPushedState = false;

	//Random Number Generator
	VRandom Random;

	//Access Keyboard, Mouse and Gamepad Input here
	VInputHandler Input;

	//Asynchronus Function Handling
	VAsync Async;

	//Window Style
	int WindowStyle;

	void SetFullscreen(bool set);
	void ToggleFullscreen();

	sf::Vector2f GetMousePosition();
	void SetMouseCursorVisible(bool set);

	//The Current Gameplay State
	VState* CurrentState();
	//Change the current state
	void ChangeState(VState* state);
	//Push a new state onto the stack to run without changing the current one.
	void PushState(VState* state);
	//Pop the current state from the stack.
	void PopState();
	//Clear all states from the stack.
	void ClearState();

	void Exit();

	bool Overlaps(VBase* a, VBase* b = NULL, std::function<void(VObject*, VObject*)>const& responseCall = nullptr, std::function<bool(VObject*, VObject*)>const& processCall = nullptr);
	bool Collides(VBase* a, VBase* b = NULL, std::function<void(VObject*, VObject*)>const& responseCall = nullptr);

	bool OverlapsCircle(VBase* a, VBase* b = NULL, std::function<void(VObject*, VObject*)>const& responseCall = nullptr, std::function<bool(VObject*, VObject*)>const& processCall = nullptr);
	bool CollidesCircle(VBase* a, VBase* b = NULL, std::function<void(VObject*, VObject*)>const& responseCall = nullptr);

	static void Cleanup();

private:
	VStateManager* gsm = NULL;
	VState* nextState = NULL;
	bool fullscreen = false;
	bool mouseCursorVisible = true;
};

