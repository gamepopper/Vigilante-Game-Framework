#include "VSoundManager.h"
#include "VGlobal.h"

VSoundManager::VSoundManager() {}
VSoundManager::~VSoundManager()
{
	if (sounds.size())
		Cleanup();
}

void VSoundManager::SetMasterVolume(float volume)
{
	masterVolume = volume;
}

void VSoundManager::SetMasterPitch(float pitch)
{
	masterPitch = pitch;
}

float VSoundManager::MasterVolume()
{
	return masterVolume;
}

float VSoundManager::MasterPitch()
{
	return masterPitch;
}

bool VSoundManager::Load(const sf::String& filename, const sf::String& id)
{
	if (sounds.find(id) == sounds.end() || sounds[id].getBuffer() == nullptr)
	{
		soundBuffers[id] = &VGlobal::p()->Content->LoadSound(filename);
		sounds[id] = sf::Sound(*soundBuffers[id]);
	}

	return true;
}

void VSoundManager::Play(const sf::String& id, float volume, float pitch, bool loop)
{
	if (sounds.find(id) != sounds.end())
	{
		if (sounds[id].getVolume() != ((volume / 100) * masterVolume)) 
			sounds[id].setVolume((volume / 100) * masterVolume);
		if (sounds[id].getPitch() != pitch * masterPitch) 
			sounds[id].setPitch(pitch * masterPitch);
		if (sounds[id].getLoop() != loop) 
			sounds[id].setLoop(loop);

		sounds[id].play();
	}
}

void VSoundManager::Stop(const sf::String& id)
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

sf::SoundStream::Status VSoundManager::Status(const sf::String& id)
{
	if (sounds.find(id) != sounds.end())
	{
		return sounds[id].getStatus();
	}

	return sf::Sound::Stopped;
}

sf::Sound* VSoundManager::GetSound(const sf::String& id)
{
	if (sounds.find(id) != sounds.end())
	{
		return &sounds[id];
	}

	return nullptr;
}

void VSoundManager::Cleanup()
{
	sounds.clear();
	soundBuffers.clear();
}
