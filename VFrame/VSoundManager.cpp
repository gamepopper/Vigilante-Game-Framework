#include "VSoundManager.h"
#include "VGlobal.h"

void VSoundManager::SetMasterVolume(float volume)
{
	masterVolume = volume;
}

void VSoundManager::SetMasterPitch(float pitch)
{
	masterPitch = pitch;
}

bool VSoundManager::Load(sf::String filename, sf::String id)
{
	if (sounds.find(id) == sounds.end())
	{
		if (VGlobal::p()->Content->LoadSound(filename, soundBuffers[id]))
		{
			sounds[id] = sf::Sound(soundBuffers[id]);
		}
	}

	return true;
}

void VSoundManager::Play(sf::String id, float volume, float pitch)
{
	if (sounds.find(id) != sounds.end())
	{
		sounds[id].setVolume((volume / 100) * masterVolume);
		sounds[id].setPitch(pitch * masterPitch);
		sounds[id].play();
	}
}

void VSoundManager::Stop(sf::String id)
{
	if (sounds.find(id) != sounds.end())
	{
		sounds[id].stop();
	}
}

void VSoundManager::StopAll()
{
	for (std::pair<std::string, sf::Sound> pair : sounds)
	{
		sounds[pair.first].stop();
	}
}

sf::Sound* VSoundManager::GetSound(sf::String id)
{
	if (sounds.find(id) != sounds.end())
	{
		return &sounds[id];
	}

	return NULL;
}

void VSoundManager::Cleanup()
{
	sounds.clear();
	soundBuffers.clear();
}
