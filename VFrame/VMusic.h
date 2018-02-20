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
	float masterVolume = 100.0f;

public:
	VMusic() {}
	virtual ~VMusic() {}

	//Opens music data from file.
	VMusic* OpenMusicFile(sf::String filename)
	{
		if (music.openFromFile(filename))
		{
			valid = true;
			return this;
		}

		valid = false;
		return nullptr;
	}

	//Opens music data from memory.
	VMusic* OpenMusicMemory(const void* data, size_t size)
	{
		if (music.openFromMemory(data, size))
		{
			valid = true;
			return this;
		}

		valid = false;
		return nullptr;
	}

	//Opens music data from a stream.
	VMusic* OpenMusicStream(sf::InputStream& stream)
	{
		if (music.openFromStream(stream))
		{
			valid = true;
			return this;
		}

		valid = false;
		return nullptr;
	}

	void Update(float dt);

	//Plays current music file.
	void Play();
	//Pauses current music file.
	void Pause();
	//Stops current music file and sets position back to 0.
	void Stop();

	//Fade current music by volume.
	void Fade(bool fadeIn, float fadeLength, float maxVolume = 100.0f, float minVolume = 0.0f);

	//Total music length.
	sf::Time Duration();
	//Amount of audio channels
	unsigned int ChannelCount();
	//Sample Rate (i.e. 44100Hz)
	unsigned int SampleRate();
	//Status of current music (i.e. Playing, Paused, Stopped)
	sf::SoundStream::Status Status();
	//Attenuation rate of music (how much the volume changes based on distance in 3D space).
	float Attenuation();
	//Looping
	bool Loop();
	//Pitch of audio (and tempo)
	float Pitch();
	//Current volume.
	float Volume();
	//Position of music being played in 3D space.
	sf::Vector3f Position();
	//Is music position relative to listener.
	bool RelativeToListener();
	//Minimum distance before attenuation takes effect.
	float MinDistance();
	//Current playing position of music.
	float PlayOffset();

	//Set Attenuation rate of music (how much the volume changes based on distance in 3D space).
	void SetAttenuation(float attenuation);
	//Set Looping, as well as start and end points of loop (defaults to beginning and end of entire music)
	void SetLoop(bool loop, float loopStart = 0, float loopEnd = 0);
	//Set Pitch of audio (and tempo)
	void SetPitch(float pitch);
	//Set Current volume.
	void SetVolume(float volume);
	//Set Position of music being played in 3D space.
	void SetPosition(float x, float y, float z);
	//Set Position of music being played in 3D space.
	void SetPosition(const sf::Vector3f& position);
	//Set music position relative to listener.
	void SetRelativeToListener(bool relative);
	//Set Minimum distance before attenuation takes effect.
	void SetMinDistance(float distance);
	//Set Current playing position of music.
	void SetPlayOffset(float offset);
};

