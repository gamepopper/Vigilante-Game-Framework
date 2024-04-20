#include "../VFrame/VGame.h"
#include "../Example/States.h"

int main()
{
	sf::ContextSettings settings;
	settings.depthBits = 24;
	settings.stencilBits = 8;
	settings.antialiasingLevel = 0;
	settings.majorVersion = 4;
	settings.minorVersion = 5;

	int r = std::make_unique<VGame>()->Run(
		"VFrame Stuff with SFML " + std::to_string(SFML_VERSION_MAJOR) + "." + std::to_string(SFML_VERSION_MINOR) + "." + std::to_string(SFML_VERSION_PATCH), 
		new DemoStatesManager(), 640, 360, 60.0f, 7, settings);

	return r;
}
