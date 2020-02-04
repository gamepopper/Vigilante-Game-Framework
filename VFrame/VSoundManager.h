/**
* @file    VSoundManager.h
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
* Sound effect handling system. Handles the playback of sound effects.
*/

#pragma once
#include <SFML/Audio.hpp>
#include <map>

///A manager for loaded sound effect files.
class VSoundManager
{
private:
	float masterVolume = 100.0f;
	float masterPitch = 1.0f;
	std::map<sf::String, sf::SoundBuffer*>	soundBuffers;
	std::map<sf::String, sf::Sound>			sounds;

public:
	VSoundManager();
	~VSoundManager();

	///@param volume The volume that will be applied to all sound effects (0 will mute all sounds, 50 will play all sounds 50% of individual volumes ect)
	void SetMasterVolume(float volume);
	///@param pitch The pitch that will be applied to all sound effects (0 will play all sounds at 0 pitch, 2 will play all sounds at double of individual pitch ect)
	void SetMasterPitch(float pitch);
	///@return The Master Volume used for all sounds.
	float MasterVolume();
	///@return The Master Pitch used for all sounds.
	float MasterPitch();

	/**
	* Load sound at set to specific id.
	* @param filename The file path and name of the sound file.
	* @param id A simple ID name to assign the file for easier access.
	* @return True if file has been loaded successfully.
	*/
	bool Load(const sf::String& filename, const sf::String& id);

	/**
	* Play sound with specific id, with specified volume and pitch.
	* @param id A simple ID name to easily access the sound.
	* @param volume The volume specifically for this sound effect (will use as a percentage of the master volume).
	* @param pitch The pitch specifically for this sound effect (will use as a percentage of the master pitch).
	* @param loop If the sound is looping.
	*/
	void Play(const sf::String& id, float volume = 100.0f, float pitch = 1.0f, bool loop = false);
	
	/**
	* Get the status of the specific sound.
	* @return Status of the sound. (i.e. Playing, Paused, Stopped).
	*/
	sf::SoundStream::Status Status(const sf::String& id);

	/**
	* Stop sound with specific id (good if sound loops).
	* @param id A simple ID name to easily access the sound.
	*/
	void Stop(const sf::String& id);

	///Stop all sound effects.
	void StopAll();
	
	/**
	* @param id The ID name of the sound.
	* @return The direct sound object for direct manipulation.
	*/
	sf::Sound* GetSound(const sf::String& id);

	///Cleanup Sound and SoundBuffer lists.
	void Cleanup();
};

