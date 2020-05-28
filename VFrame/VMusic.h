/**
* @file    VMusic.h
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
* Handler for streamed music.
*/

#pragma once
#include <SFML/Audio/Music.hpp>
#include <SFML/System/String.hpp>
#include <iostream>

///Class for managing streamed music using an sf::Music object.
class VMusic
{
private:
	///Current streamed music
	sf::Music music;
	///Music file is value if true.
	bool valid = false;
	/*///Music is looping if true.
	bool looping = false;
	///The point in time when the music will start looping.
	float loopStartPoint = 0;
	///The point in time when the music will go back to the loop start point.
	float loopEndPoint = 0;*/
	///Music volume will fade in if true.
	bool fadein = false;
	///Music volume will fade out if true.
	bool fadeout = false;
	///Time in seconds to fade.
	float fadeTime = 0;
	///Timer to track fading.
	float fadeTimer = 0;
	///Volume at the beginning of the fade.
	float startVolume = 100.0f;
	///Volume at the end of the fade.
	float finishVolume = 0.0f;
	///Master volume for all music.
	float masterVolume = 100.0f;

public:
	VMusic();
	virtual ~VMusic();

	/**
	* Opens music data from file.
	* Various properties will only have an effect once music data is open.
	*/
	VMusic* OpenMusicFile(sf::String filename);

	/**
	* Opens music data from memory.
	* Various properties will only have an effect once music data is open.
	*/
	VMusic* OpenMusicMemory(const void* data, size_t size);

	/**
	* Opens music data from a stream.
	* Various properties will only have an effect once music data is open.
	*/
	VMusic* OpenMusicStream(sf::InputStream& stream);

	/**
	* Updates music and volume.
	* @param dt Delta Time between the previous and current frame.
	*/
	void Update(float dt);

	///Plays current music file.
	void Play();
	///Pauses current music file.
	void Pause();
	///Stops current music file and sets position back to 0.
	void Stop();

	/**
	* Fade current music by volume.
	* @param fadeIn If true, the volume will fade in.
	* @param fadeLength Length of time for the fade to process.
	* @param maxVolume The highest volume for fade (Initial volume for Fade Out).
	* @param minVolume The lowest volume for fade (Initial volume for Fade In).
	*/
	void Fade(bool fadeIn, float fadeLength, float maxVolume = 100.0f, float minVolume = 0.0f);

	///@return Total music length as an sf::Time object.
	sf::Time Duration();
	///@return Amount of audio channels.
	unsigned int ChannelCount();
	///@return Sample Rate (i.e. 44100Hz).
	unsigned int SampleRate();
	///@return Status of current music (i.e. Playing, Paused, Stopped).
	sf::SoundStream::Status Status();
	///@return Attenuation rate of music (how much the volume changes based on distance in 3D space).
	float Attenuation();
	///@return If current music should loop.
	bool Loop();
	///@return Pitch of audio (and tempo).
	float Pitch();
	///@return Current volume.
	float Volume();
	///@return Position of music being played in 3D space.
	sf::Vector3f Position();
	///@return Is music position relative to listener.
	bool RelativeToListener();
	///@return Minimum distance before attenuation takes effect.
	float MinDistance();
	///@return Current playing position of music.
	float PlayOffset();

	///@param attenuation Attenuation rate of music (how much the volume changes based on distance in 3D space).
	void SetAttenuation(float attenuation);
	
	/**
	* @param loop If true, music will now loop.
	* @param loopStart The starting point of the loop as time in seconds.
	* @param loopEnd The ending point of a loop as time in seconds. If 0, loop End will be the total length of the music track.
	*/
	void SetLoop(bool loop, float loopStart = 0, float loopEnd = 0);
	///@param pitch Pitch of audio (and tempo)
	void SetPitch(float pitch);
	///@param volume New volume.
	void SetVolume(float volume);

	/**
	* @param x X Position of music being played in 3D space.
	* @param y Y Position of music being played in 3D space.
	* @param z Z Position of music being played in 3D space.
	*/
	void SetPosition(float x, float y, float z);
	///@param position New Position of music being played in 3D space.
	void SetPosition(const sf::Vector3f& position);
	///@param relative If position is relative to listener.
	void SetRelativeToListener(bool relative);
	///@param distance New Minimum distance before attenuation takes effect.
	void SetMinDistance(float distance);
	///@param offset New position time of music.
	void SetPlayOffset(float offset);
};

