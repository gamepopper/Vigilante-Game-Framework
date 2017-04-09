#include "../VFrame/VGame.h"
#include "../Example/States.h"

int main()
{
	VGame* game = new VGame();

	sf::ContextSettings settings;
	settings.depthBits = 32;
	settings.stencilBits = 0;
	settings.antialiasingLevel = 0;
	settings.majorVersion = 2;
	settings.minorVersion = 1;

	int r = game->Run("VFrame Stuff", new DemoStatesManager(), 640, 360, 60.0f, 7, settings);
	delete game;
	return r;
}
