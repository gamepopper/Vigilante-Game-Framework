#pragma once
#include <map>
#include <SFML\Graphics\Texture.hpp>
#include <SFML\Graphics\Image.hpp>
#include <SFML\Graphics\Font.hpp>
#include <SFML\Audio\SoundBuffer.hpp>
#include <SFML\System\String.hpp>
#include "VBase.h"

class VContent : public VBase
{
public:
	VContent(){};
	~VContent()
	{
		UnloadAll();
	}

	bool LoadTexture(sf::String name, sf::Texture &texture);
	bool LoadImage(sf::String name, sf::Image &image);
	bool LoadFont(sf::String name, sf::Font &font);
	bool LoadSound(sf::String name, sf::SoundBuffer &sound);

	bool UnloadTexture(sf::String name);
	bool UnloadImage(sf::String name);
	bool UnloadFont(sf::String name);
	bool UnloadSound(sf::String name);

	void UnloadAll();

	bool StoreTexture(sf::String name, const sf::Texture& texture);
	bool StoreImage(sf::String name, const sf::Image& image);
	bool StoreFont(sf::String name, const sf::Font& font);
	bool StoreSound(sf::String name, const sf::SoundBuffer& sound);

	bool FindTexture(sf::String name);
	bool FindImage(sf::String name);
	bool FindFont(sf::String name);
	bool FindSound(sf::String name);

private:
	std::map<sf::String, sf::Texture>		textureDir;
	std::map<sf::String, sf::Image>			imageDir;
	std::map<sf::String, sf::Font>			fontDir;
	std::map<sf::String, sf::SoundBuffer>	soundDir;
};

