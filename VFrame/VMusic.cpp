#include "VMusic.h"
#include "VInterpolate.h"

VMusic::VMusic() {}
VMusic::~VMusic() {}

/**
* Opens music data from file.
*/
VMusic* VMusic::OpenMusicFile(sf::String filename)
{
	if (music.openFromFile(filename))
	{
		valid = true;
		return this;
	}

	valid = false;
	return nullptr;
}

/**
* Opens music data from memory.
*/
VMusic* VMusic::OpenMusicMemory(const void* data, size_t size)
{
	if (music.openFromMemory(data, size))
	{
		valid = true;
		return this;
	}

	valid = false;
	return nullptr;
}

/**
* Opens music data from a stream.
*/
VMusic* VMusic::OpenMusicStream(sf::InputStream& stream)
{
	if (music.openFromStream(stream))
	{
		valid = true;
		return this;
	}

	valid = false;
	return nullptr;
}

void VMusic::Update(float dt)
{
	if (!valid)
		return;

	if (Status() == music.Playing)
	{
		if (fadein)
		{
			fadeTimer += dt / fadeTime;
			music.setVolume(VInterpolate::Float(startVolume, finishVolume, fadeTimer) * (masterVolume / 100.0f));

			if (fadeTimer > 1.0f)
			{
				fadein = false;
				fadeTimer = 0;
			}
		}

		if (fadeout)
		{
			fadeTimer += dt / fadeTime;
			music.setVolume(VInterpolate::Float(startVolume, finishVolume, fadeTimer) * (masterVolume / 100.0f));

			if (fadeTimer > 1.0f)
			{
				fadeout = false;
				fadeTimer = 0;
			}
		}

		if (looping && loopEndPoint > 0)
		{
			if (music.getPlayingOffset().asSeconds() >= loopEndPoint)
			{
				SetPlayOffset(loopStartPoint);
			}
		}
	}
	else
	{
		fadein = false;
		fadeout = false;
	}
}

void VMusic::Play()
{
	if (!valid)
		return;

	music.play();
}

void VMusic::Pause()
{
	if (!valid)
		return;

	music.pause();
}

void VMusic::Stop()
{
	if (!valid)
		return;

	music.stop();
}

void VMusic::Fade(bool fadeIn, float fadeLength, float maxVolume, float minVolume)
{
	if ((fadeIn && !fadein) || (!fadeIn && !fadeout))
	{
		fadeTime = fadeLength;
		fadeTimer = 0;

		if (fadeIn)
		{
			startVolume = minVolume;
			finishVolume = maxVolume;
			fadein = true;
			fadeout = false;
			music.setVolume(minVolume * (masterVolume / 100.0f));
		}
		else
		{
			startVolume = maxVolume;
			finishVolume = minVolume;
			fadein = false;
			fadeout = true;
			music.setVolume(maxVolume * (masterVolume / 100.0f));
		}
	}
}

sf::Time VMusic::Duration() 
{
	if (!valid)
		return sf::Time::Zero;

	return music.getDuration();
}

unsigned int VMusic::ChannelCount() 
{
	if (!valid)
		return 0;

	return music.getChannelCount();
}

unsigned int VMusic::SampleRate() 
{
	if (!valid)
		return 0;

	return music.getSampleRate();
}

sf::SoundStream::Status VMusic::Status() 
{
	if (!valid)
		return sf::SoundStream::Stopped;

	return music.getStatus();
}

float VMusic::Attenuation() 
{
	if (!valid)
		return 0;

	return music.getAttenuation();
}

bool VMusic::Loop()
{
	if (!valid)
		return false;

	return looping;
}

float VMusic::Pitch()
{
	if (!valid)
		return 0;

	return music.getPitch();
}

float VMusic::Volume()
{
	if (!valid)
		return 0;

	return masterVolume;
}

sf::Vector3f VMusic::Position()
{
	if (!valid)
		return sf::Vector3f();

	return music.getPosition();
}

bool VMusic::RelativeToListener()
{
	if (!valid)
		return false;

	return music.isRelativeToListener();
}

float VMusic::MinDistance()
{
	if (!valid)
		return 0;

	return music.getMinDistance();
}

float VMusic::PlayOffset()
{
	if (!valid)
		return 0;

	return music.getPlayingOffset().asSeconds();
}

void VMusic::SetAttenuation(float attenuation)
{
	if (!valid)
		return;

	music.setAttenuation(attenuation);
}

void VMusic::SetLoop(bool loop, float loopStart, float loopEnd)
{
	if (!valid)
		return;

	if (loop)
	{
		if (!looping || (loopStartPoint != loopStart) || (loopEndPoint != loopEnd))
		{
			looping = loop;

			if (loopStart == 0 && loopEnd == 0)
			{
				music.setLoop(loop);
				loopStartPoint = 0;
				loopEndPoint = 0;
			}
			else
			{
				loopStartPoint = loopStart;
				if (loopEnd != 0)
					loopEndPoint = loopEnd;
				else
					loopEndPoint = music.getDuration().asSeconds();
			}
		}
	}
	else
	{
		looping = loop;
		music.setLoop(loop);
	}
}

void VMusic::SetPitch(float pitch)
{
	if (!valid)
		return;

	music.setPitch(pitch);
}

void VMusic::SetVolume(float volume)
{
	if (!valid)
		return;

	if (volume < 0) 
		volume = 0;

	masterVolume = volume;
	music.setVolume(masterVolume);
}

void VMusic::SetPosition(float x, float y, float z)
{
	if (!valid)
		return;

	music.setPosition(x, y, z);
}

void VMusic::SetPosition(const sf::Vector3f& position)
{
	if (!valid)
		return;

	music.setPosition(position);
}

void VMusic::SetRelativeToListener(bool relative)
{
	if (!valid)
		return;

	music.setRelativeToListener(relative);
}

void VMusic::SetMinDistance(float distance)
{
	if (!valid)
		return;

	music.setMinDistance(distance);
}

void VMusic::SetPlayOffset(float offset)
{
	if (!valid)
		return;

	music.setPlayingOffset(sf::seconds(offset));
}