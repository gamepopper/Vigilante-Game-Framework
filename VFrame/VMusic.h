#pragma once
#include <SFML/Audio/Music.hpp>
#include <SFML/System/String.hpp>
#include <iostream>

class VMusic
{
protected:
	sf::Music music;
	bool valid = false;
	bool looping = false;
	float loopStartPoint = 0;
	float loopEndPoint = 0;

	bool fadein = false;
	bool fadeout = false;
	float fadeTime = 0;
	float fadeTimer = 0;
	float startVolume = 100.0f;
	float finishVolume = 0.0f;

public:
	VMusic() {}
	virtual ~VMusic() {}

	VMusic* OpenMusicFile(sf::String filename)
	{
		if (music.openFromFile(filename))
		{
			valid = true;
			return this;
		}

		valid = false;
		return NULL;
	}

	VMusic* OpenMusicMemory(const void* data, size_t size)
	{
		if (music.openFromMemory(data, size))
		{
			valid = true;
			return this;
		}

		valid = false;
		return NULL;
	}

	VMusic* OpenMusicStream(sf::InputStream& stream)
	{
		if (music.openFromStream(stream))
		{
			valid = true;
			return this;
		}

		valid = false;
		return NULL;
	}

	void Update(float dt);

	void Play();
	void Pause();
	void Stop();

	void Fade(bool fadeIn, float fadeLength, float maxVolume = 100.0f, float minVolume = 0.0f);

	sf::Time Duration();
	unsigned int ChannelCount();
	unsigned int SampleRate();
	sf::SoundStream::Status Status();
	float Attenuation();
	bool Loop();
	float Pitch();
	float Volume();
	sf::Vector3f Position();
	bool RelativeToListener();
	float MinDistance();
	float PlayOffset();

	void SetAttenuation(float attenuation);
	void SetLoop(bool loop, float loopStart = 0, float loopEnd = 0);
	void SetPitch(float pitch);
	void SetVolume(float volume);
	void SetPosition(float x, float y, float z);
	void SetPosition(const sf::Vector3f& position);
	void SetRelativeToListener(bool relative);
	void SetMinDistance(float distance);
	void SetPlayOffset(float offset);
};

