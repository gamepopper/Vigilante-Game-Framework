#pragma once
#include <map>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#include <SFML/System/String.hpp>
#include "VBase.h"

class VContent : public VBase
{
public:
	VContent(){};
	~VContent()
	{
		UnloadAll();
	}

	//Loads/Gets texture by name to sf::Texture object.
	bool LoadTexture(const sf::String& name, sf::Texture &texture);
	//Loads/Gets image by name to sf::Image object.
	bool LoadImage(const sf::String& name, sf::Image &image);
	//Loads/Gets font by name to sf::Font object.
	bool LoadFont(const sf::String& name, sf::Font &font);
	//Loads/Gets sound by name to sf::SoundBuffer object.
	bool LoadSound(const sf::String& name, sf::SoundBuffer &sound);

	//Unload Texture
	bool UnloadTexture(const sf::String& name);
	//Unload Image
	bool UnloadImage(const sf::String& name);
	//Unload Font
	bool UnloadFont(const sf::String& name);
	//Unload Sound
	bool UnloadSound(const sf::String& name);

	//Unload All Content
	void UnloadAll();

	//Store preloaded Texture with a name.
	bool StoreTexture(const sf::String& name, const sf::Texture& texture);
	//Store preloaded Image with a name.
	bool StoreImage(const sf::String& name, const sf::Image& image);
	//Store preloaded Font with a name.
	bool StoreFont(const sf::String& name, const sf::Font& font);
	//Store preloaded Sound with a name.
	bool StoreSound(const sf::String& name, const sf::SoundBuffer& sound);

	//Check if specific Texture exists.
	bool FindTexture(const sf::String& name);
	//Check if specific Image exists.
	bool FindImage(const sf::String& name);
	//Check if specific Font exists.
	bool FindFont(const sf::String& name);
	//Check if specific Sound exists.
	bool FindSound(const sf::String& name);

private:
	std::map<sf::String, sf::Texture>		textureDir;
	std::map<sf::String, sf::Image>			imageDir;
	std::map<sf::String, sf::Font>			fontDir;
	std::map<sf::String, sf::SoundBuffer>	soundDir;
};

