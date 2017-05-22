#include "VContent.h"
#include "VGlobal.h"

bool VContent::FindTexture(const sf::String& name)
{
	return textureDir.find(name) != textureDir.end();
}

bool VContent::FindImage(const sf::String& name)
{
	return imageDir.find(name) != imageDir.end();
}

bool VContent::FindFont(const sf::String& name)
{
	return fontDir.find(name) != fontDir.end();
}

bool VContent::FindSound(const sf::String& name)
{
	return soundDir.find(name) != soundDir.end();
}

bool VContent::LoadTexture(const sf::String& name, sf::Texture &texture)
{
	if (FindTexture(name))
	{
		texture = textureDir[name];
		return true;
	}
	else
	{
		if (texture.loadFromFile(name))
		{
			VLog("%s loaded", name.toAnsiString().c_str());
			textureDir.insert(textureDir.begin(), std::pair<sf::String, sf::Texture>(name, texture));
			return true;
		}
	}

	VLog("Error loading texture: %s", name.toAnsiString().c_str());

	return false;
}

bool VContent::LoadImage(const sf::String& name, sf::Image &image)
{
	if (FindImage(name))
	{
		image = imageDir[name];
		return true;
	}
	else
	{
		if (image.loadFromFile(name))
		{
			VLog("%s loaded", name.toAnsiString().c_str());
			imageDir.insert(imageDir.begin(), std::pair<sf::String, sf::Image>(name, image));
			return true;
		}
	}

	VLog("Error loading image: %s", name.toAnsiString().c_str());

	return false;
}

bool VContent::LoadFont(const sf::String& name, sf::Font &font)
{
	if (FindFont(name))
	{
		font = fontDir[name];
		return true;
	}
	else
	{
		if (font.loadFromFile(name))
		{
			VLog("%s loaded", name.toAnsiString().c_str());
			fontDir.insert(fontDir.begin(), std::pair<sf::String, sf::Font>(name, font));
			return true;
		}
	}

	VLog("Error loading font: %s", name.toAnsiString().c_str());

	return false;
}

bool VContent::LoadSound(const sf::String& name, sf::SoundBuffer &sound)
{
	if (FindSound(name))
	{
		sound = soundDir[name];
		return true;
	}
	else
	{
		if (sound.loadFromFile(name))
		{

			VLog("%s loaded", name.toAnsiString().c_str());
			soundDir.insert(soundDir.begin(), std::pair<sf::String, sf::SoundBuffer>(name, sound));
			return true;
		}
	}

	VLog("Error loading sound: %s", name.toAnsiString().c_str());

	return false;
}

bool VContent::UnloadTexture(const sf::String& name)
{
	if (FindTexture(name))
	{
		textureDir[name].~Texture();
		textureDir.erase(textureDir.find(name));
		return true;
	}

	VLog("Couldn't find texture: %s", name.toAnsiString().c_str());

	return false;
}

bool VContent::UnloadImage(const sf::String& name)
{
	if (FindImage(name))
	{
		imageDir[name].~Image();
		imageDir.erase(imageDir.find(name));
		return true;
	}

	VLog("Couldn't find image: %s", name.toAnsiString().c_str());

	return false;
}

bool VContent::UnloadFont(const sf::String& name)
{
	if (FindFont(name))
	{
		fontDir[name].~Font();
		fontDir.erase(fontDir.find(name));
		return true;
	}

	VLog("Couldn't find font: %s", name.toAnsiString().c_str());

	return false;
}

bool VContent::UnloadSound(const sf::String& name)
{
	if (FindSound(name))
	{
		soundDir[name].~SoundBuffer();
		soundDir.erase(soundDir.find(name));
		return true;
	}

	VLog("Couldn't find sound: %s", name.toAnsiString().c_str());

	return false;
}


void VContent::UnloadAll()
{
	textureDir.clear();
	imageDir.clear();
	fontDir.clear();
	soundDir.clear();

	VLog("All content directories cleared");
}

bool VContent::StoreTexture(const sf::String& name, const sf::Texture& texture)
{
	if (!FindTexture(name))
	{
		VLog("%s stored", name.toAnsiString().c_str());
		textureDir.insert(textureDir.begin(), std::pair<sf::String, sf::Texture>(name, texture));		
		return true;
	}

	VLog("Error storing texture: %s", name.toAnsiString().c_str());

	return false;
}

bool VContent::StoreImage(const sf::String& name, const sf::Image& image)
{
	if (!FindImage(name))
	{
		VLog("%s stored", name.toAnsiString().c_str());
		imageDir.insert(imageDir.begin(), std::pair<sf::String, sf::Image>(name, image));
		return true;
	}

	VLog("Error storing image: %s", name.toAnsiString().c_str());

	return false;
}

bool VContent::StoreFont(const sf::String& name, const sf::Font& font)
{
	if (!FindFont(name))
	{
		VLog("%s stored", name.toAnsiString().c_str());
		fontDir.insert(fontDir.begin(), std::pair<sf::String, sf::Font>(name, font));
		return true;
	}

	VLog("Error storing font: %s", name.toAnsiString().c_str());

	return false;
}

bool VContent::StoreSound(const sf::String& name, const sf::SoundBuffer& sound)
{
	if (!FindSound(name))
	{
		VLog("%s stored", name.toAnsiString().c_str());
		soundDir.insert(soundDir.begin(), std::pair<sf::String, sf::SoundBuffer>(name, sound));
		return true;
	}

	VLog("Error storing sound: %s", name.toAnsiString().c_str());

	return false;
}