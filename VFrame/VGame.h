#pragma once
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/System/String.hpp>
#include <SFML/Window/WindowStyle.hpp>
#include <SFML/Window/ContextSettings.hpp>

#include "VState.h"

class VGame : public VBase
{
public:
	VGame() = default;
	virtual ~VGame();
	int Run(const sf::String& title, VState* initialState, int windowwidth, int windowheight, float fps = 60, int flags = sf::Style::Default, const sf::ContextSettings& settings = sf::ContextSettings(32U));
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

