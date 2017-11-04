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

#define VFRAME_VERSION "0.9.9.13"

class VBase;
class VObject;
class VPostEffectBase;
class VCollision;

class VGlobal
{
private:
	static VGlobal* Instance;
	VGlobal();
	~VGlobal();

public:
	static VGlobal* p()
	{
		if (!Instance)
		{
			Instance = new VGlobal();
		}

		return Instance;
	}

	//App that handles all rendering.
	std::unique_ptr<sf::RenderWindow> App;
	//RenderState for entire rendered scene.
	sf::RenderStates RenderState;
	//Sprite object of the last rendered frame.
	std::unique_ptr<sf::Sprite> RenderSprite;
	sf::ContextSettings ContextSettings;

	//Whether the rendering will appear smooth or pixelated when scaling.
	bool Antialiasing = false;
	//VSync
	bool VSync = false;

#ifdef _DEBUG
	//Whether to render collision boxes for debugging perposes
	bool DrawDebug = false;
#endif

	//Colour of the background prior to rendering.
	sf::Color BackgroundColor = sf::Color::Black;

	//Speed of GameTime (1 is default speed, 2 is 2x speed)
	float TimeScale = 1.0f;

	//VGame Title (Use Window->SetTitle to show it on window)
	sf::String Title;

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
	//World Bounds - used to bound camera to world area and for quad tree collisions.
	sf::FloatRect WorldBounds;
	//Music - play streamed music (particularly between states).
	std::unique_ptr<VMusic> Music;
	//Sound - play loaded sounds.
	std::unique_ptr<VSoundManager> Sound;

	std::unique_ptr<VContent> Content;
	std::unique_ptr<VPostEffectBase> PostProcess;

	//Frames Per Second - used to calculate average framerate
	float FPS = 0;

	bool IfChangedState = false;
	bool IfPushedState = false;

	//Random Number Generator
	std::unique_ptr<VRandom> Random;

	//Access Keyboard, Mouse and Gamepad Input here
	std::unique_ptr<VInputHandler> Input;

	//Asynchronus Function Handling
	std::unique_ptr<VAsync> Async;

	//Window Style
	int WindowStyle;

	//Set game to fullscreen or not.
	void SetFullscreen(bool set);

	//Toggles from fullscreen to window mode.
	void ToggleFullscreen();

	//Returns true if set to fullscreen.
	bool IsFullscreen();

	//Get mouse position relative to screen.
	sf::Vector2f GetMousePosition();

	//Sets mouse cursor to be visible or not, depending of if you want an in-game cursor.
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
	//Close window and close out game.
	void Exit();

	//Test if two objects are overlapping, use responseCall to handle overlap response, and processCall to process objects in event of overlap.
	bool OverlapAtPoint(const sf::Vector2f& point, VBase* a);
	//Test if two objects are overlapping, use responseCall to handle overlap response, and processCall to process objects in event of overlap.
	bool Overlaps(VBase* a, VBase* b = nullptr, std::function<void(VObject*, VObject*)>const& responseCall = nullptr, std::function<bool(VObject*, VObject*)>const& processCall = nullptr);
	//Test if two objects are overlapping, and then separates objects when true, use responseCall to handle collision response.
	bool Collides(VBase* a, VBase* b = nullptr, std::function<void(VObject*, VObject*)>const& responseCall = nullptr);

	//Test if two objects are overlapping, use responseCall to handle overlap response, and processCall to process objects in event of overlap.
	bool OverlapCircleAtPoint(const sf::Vector2f& point, VBase* a);
	//Test if two objects are overlapping as a circle, use responseCall to handle overlap response, and processCall to process objects in event of overlap. Make sure objects have the Radius property set.
	bool OverlapsCircle(VBase* a, VBase* b = nullptr, std::function<void(VObject*, VObject*)>const& responseCall = nullptr, std::function<bool(VObject*, VObject*)>const& processCall = nullptr);
	//Test if two objects are overlapping as a circle, and then separates objects when true, use responseCall to handle collision response. Make sure objects have the Radius property set.
	bool CollidesCircle(VBase* a, VBase* b = nullptr, std::function<void(VObject*, VObject*)>const& responseCall = nullptr);

	//Cleanups Global data.
	static void Cleanup();

private:
	std::unique_ptr<VStateManager> gsm;
	VState* nextState = nullptr;
	bool fullscreen = false;
	bool mouseCursorVisible = true;

	std::unique_ptr<VCollision> collision;
	std::function<bool(VObject*, VObject*)> rectCollision;
	std::function<bool(VObject*, VObject*)> circleCollision;
};

