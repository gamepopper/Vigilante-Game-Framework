#pragma once
#include <SFML\Audio.hpp>
#include <map>

class VSoundManager
{
private:
	float masterVolume = 100.0f;
	float masterPitch = 1.0f;
	std::map<sf::String, sf::SoundBuffer>	soundBuffers;
	std::map<sf::String, sf::Sound>			sounds;

public:
	VSoundManager() {}
	~VSoundManager()
	{
		if (sounds.size())
			Cleanup();
	}

	void SetMasterVolume(float volume);
	void SetMasterPitch(float pitch);
	bool Load(sf::String filename, sf::String id);
	void Play(sf::String id, float volume = 100.0f, float pitch = 1.0f);
	void Stop(sf::String id);
	void StopAll();
	sf::Sound* GetSound(sf::String id);
	void Cleanup();
};

