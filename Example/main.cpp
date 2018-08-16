#include "../VFrame/VGame.h"
#include "../Example/States.h"

int main()
{
	VGame* game = new VGame();

	sf::ContextSettings settings;
	settings.depthBits = 32;
	settings.stencilBits = 8;
	settings.antialiasingLevel = 0;
	settings.majorVersion = 4;
	settings.minorVersion = 5;

	int r = game->Run("VFrame Stuff with SFML 2.5.0", new DemoStatesManager(), 640, 360, 60.0f, 7, settings);
	delete game;
	return r;
}
