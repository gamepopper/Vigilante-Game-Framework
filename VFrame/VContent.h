/**
* @file    VContent.h
* @author  Tim Stoddard <tim@gamepopper.co.uk>
*
* @section LICENSE
*
* MIT License
*
* Copyright (c) 2020 Tim Stoddard
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*
* @section DESCRIPTION
*
* Content management system for handling the loading and unloading of textures, fonts and sounds.
*/

#pragma once
#include <map>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#include <SFML/System/String.hpp>
#include <memory>
#include "VBase.h"

///Content Management System for loading and storing Textures, Fonts and Sound Effects.
class VContent
{
public:
	VContent();
	~VContent();

	/**
	*@param name File path of the texture used for the sf::Texture object.
	*@return True if sf::Texture is stored in the content handler.
	*/
	bool TextureExists(const sf::String& name);

	/**
	*@param name File path of the texture used for the sf::Image object.
	*@return True if sf::Image is stored in the content handler.
	*/
	bool ImageExists(const sf::String& name);

	/**
	*@param name File path of the font used for the sf::Font object.
	*@return True if sf::Font is stored in the content handler.
	*/
	bool FontExists(const sf::String& name);

	/**
	*@param name File path of the sound file used for the sf::SoundBuffer object.
	*@return True if sf::SoundBuffer is stored in the content handler.
	*/
	bool SoundExists(const sf::String& name);

	/**
	*Loads/Gets texture by name to an sf::Texture object.
	*@param name File path of the texture used for the sf::Texture object.
	*@return Reference of the sf::Texture if found. A runtime error will be thrown if not found or cannot be loaded.
	*/
	sf::Texture& LoadTexture(const sf::String& name);
	
	/**
	*Loads/Gets image by name to an sf::Image object.
	*@param name File path of the texture used for the sf::Image object.
	*@return True if sf::Image is stored in the content handler.
	*/
	sf::Image& LoadImage(const sf::String& name);
	
	/**
	*Loads/Gets font by name to an sf::Font object.
	*@param name File path of the font used for the sf::Font object.
	*@return True if sf::Font is stored in the content handler.
	*/
	sf::Font& LoadFont(const sf::String& name);
	
	/**
	*Loads/Gets sound by name to an sf::SoundBuffer object.
	*@param name File path of the sound file used for the sf::SoundBuffer object.
	*@return True if sf::SoundBuffer is stored in the content handler.
	*/
	sf::SoundBuffer& LoadSound(const sf::String& name);

	/**
	*Store preloaded Texture with a name.
	*@param name File path of the texture used for the sf::Texture object.
	*@param texture Texture object to store.
	*@return True if sf::Texture is stored in the content handler.
	*/
	bool StoreTexture(const sf::String& name, const sf::Texture& texture);
	
	/**
	*Store preloaded Image with a name.
	*@param name File path of the image used for the sf::Image object.
	*@param image Image object to store.
	*@return True if sf::Image is stored in the content handler.
	*/
	bool StoreImage(const sf::String& name, const sf::Image& image);
	
	/**
	*Store preloaded Font with a name.
	*@param name File path of the texture used for the sf::Font object.
	*@param font Font object to store.
	*@return True if sf::Font is stored in the content handler.
	*/
	bool StoreFont(const sf::String& name, const sf::Font& font);
	
	/**
	*Store preloaded Sound with a name.
	*@param name File path of the texture used for the sf::SoundBuffer object.
	*@param sound Sound object to store.
	*@return True if sf::SoundBuffer is stored in the content handler.
	*/
	bool StoreSound(const sf::String& name, const sf::SoundBuffer& sound);

	/**
	*Unload Texture
	*@param name File path of the texture used for the sf::Texture object.
	*@return True if sf::Texture is stored in the content handler.
	*/
	bool UnloadTexture(const sf::String& name);
	
	/**
	*Unload Image
	*@param name File path of the image used for the sf::Image object.
	*@return True if sf::Image is stored in the content handler.
	*/
	bool UnloadImage(const sf::String& name);
	
	/**
	*Unload Font
	*@param name File path of the font used for the sf::Font object.
	*@return True if sf::Font is stored in the content handler.
	*/
	bool UnloadFont(const sf::String& name);
	
	/**
	*Unload Sound
	*@param name File path of the sound file used for the sf::SoundBuffer object.
	*@return True if sf::SoundBuffer is stored in the content handler.
	*/
	bool UnloadSound(const sf::String& name);

	///Unload All Content
	void UnloadAll();

private:
	///Texture Set List
	std::map<sf::String, std::unique_ptr<sf::Texture>>		textureDir;
	///Image Set List
	std::map<sf::String, std::unique_ptr<sf::Image>>		imageDir;
	///Font Set List
	std::map<sf::String, std::unique_ptr<sf::Font>>			fontDir;
	///Sound Set List
	std::map<sf::String, std::unique_ptr<sf::SoundBuffer>>	soundDir;
};

