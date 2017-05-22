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

	bool LoadTexture(const sf::String& name, sf::Texture &texture);
	bool LoadImage(const sf::String& name, sf::Image &image);
	bool LoadFont(const sf::String& name, sf::Font &font);
	bool LoadSound(const sf::String& name, sf::SoundBuffer &sound);

	bool UnloadTexture(const sf::String& name);
	bool UnloadImage(const sf::String& name);
	bool UnloadFont(const sf::String& name);
	bool UnloadSound(const sf::String& name);

	void UnloadAll();

	bool StoreTexture(const sf::String& name, const sf::Texture& texture);
	bool StoreImage(const sf::String& name, const sf::Image& image);
	bool StoreFont(const sf::String& name, const sf::Font& font);
	bool StoreSound(const sf::String& name, const sf::SoundBuffer& sound);

	bool FindTexture(const sf::String& name);
	bool FindImage(const sf::String& name);
	bool FindFont(const sf::String& name);
	bool FindSound(const sf::String& name);

private:
	std::map<sf::String, sf::Texture>		textureDir;
	std::map<sf::String, sf::Image>			imageDir;
	std::map<sf::String, sf::Font>			fontDir;
	std::map<sf::String, sf::SoundBuffer>	soundDir;
};

