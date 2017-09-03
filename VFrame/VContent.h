#pragma once
#include <map>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#include <SFML/System/String.hpp>
#include <memory>
#include "VBase.h"

class VContent
{
public:
	VContent(){};
	~VContent()
	{
		UnloadAll();
	}

	bool TextureExists(const sf::String& name);
	bool ImageExists(const sf::String& name);
	bool FontExists(const sf::String& name);
	bool SoundExists(const sf::String& name);

	//Loads/Gets texture by name to sf::Texture object.
	sf::Texture& LoadTexture(const sf::String& name);
	//Loads/Gets image by name to sf::Image object.
	sf::Image& LoadImage(const sf::String& name);
	//Loads/Gets font by name to sf::Font object.
	sf::Font& LoadFont(const sf::String& name);
	//Loads/Gets sound by name to sf::SoundBuffer object.
	sf::SoundBuffer& LoadSound(const sf::String& name);

	//Store preloaded Texture with a name.
	bool StoreTexture(const sf::String& name, const sf::Texture& texture);
	//Store preloaded Image with a name.
	bool StoreImage(const sf::String& name, const sf::Image& image);
	//Store preloaded Font with a name.
	bool StoreFont(const sf::String& name, const sf::Font& font);
	//Store preloaded Sound with a name.
	bool StoreSound(const sf::String& name, const sf::SoundBuffer& sound);

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

private:
	std::map<sf::String, std::unique_ptr<sf::Texture>>		textureDir;
	std::map<sf::String, std::unique_ptr<sf::Image>>		imageDir;
	std::map<sf::String, std::unique_ptr<sf::Font>>			fontDir;
	std::map<sf::String, std::unique_ptr<sf::SoundBuffer>>	soundDir;
};

