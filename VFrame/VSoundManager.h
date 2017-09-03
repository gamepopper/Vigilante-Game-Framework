#pragma once
#include <SFML/Audio.hpp>
#include <map>

class VSoundManager
{
private:
	float masterVolume = 100.0f;
	float masterPitch = 1.0f;
	std::map<sf::String, sf::SoundBuffer*>	soundBuffers;
	std::map<sf::String, sf::Sound>			sounds;

public:
	VSoundManager() {}
	~VSoundManager()
	{
		if (sounds.size())
			Cleanup();
	}

	//Sets volume that will be applied to all sound effects (0 will mute all sounds, 50 will play all sounds 50% of individual volumes ect)
	void SetMasterVolume(float volume);
	//Sets pitch that will be applied to all sound effects (0 will play all sounds at 0 pitch, 2 will play all sounds at double of individual pitch ect)
	void SetMasterPitch(float pitch);
	//Load sound at set to specific id.
	bool Load(sf::String filename, sf::String id);
	//Play sound with specific id, with specified volume and pitch.
	void Play(sf::String id, float volume = 100.0f, float pitch = 1.0f);
	//Stop sound with specific id (good if sound loops).
	void Stop(sf::String id);
	//Stop all sound effects.
	void StopAll();
	//Gets the direct sound object for direct manipulation.
	sf::Sound* GetSound(sf::String id);
	//Cleanup Sound and SoundBuffer lists.
	void Cleanup();
};

