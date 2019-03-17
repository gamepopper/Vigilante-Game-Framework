/**
* @file    VTypedText.h
* @author  Tim Stoddard <tim@gamepopper.co.uk>
*
* @section LICENSE
*
* MIT License
*
* Copyright (c) 2018 Tim Stoddard
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
* Text rendering object that displays text one or a specified amount of characters at a time, like a typewriter or a computer terminal.
*/

#pragma once
#include "VText.h"
#include <functional>
#include <SFML/Audio/Sound.hpp>
#include <SFML/Audio/SoundBuffer.hpp>

///Special Text class that displays text in a typewriter effect.
class VTypedText : public VText
{
private:
	///Text that is displayed before the updated string, is not erased or typed out.
	std::wstring prefix = L"";
	///Final text that will displayed, might not be the complete text the player inputted.
	std::wstring finalText = L"";
	///Timer for the delay between character updates.
	float timer = 0;
	///Timer for cursor blinking.
	float cursorTimer = 0;
	///Timer for the wait between text being typed and text being erased.
	float waitTimer = 0;
	///Flag if the text is being typed.
	bool typing = false;
	///Flag if the text is being erased.
	bool erasing = false;
	///Flag if the text is waiting.
	bool waiting = false;

	///If true, there will be a random additional delay that can make text look like it's not being typed in a fixed sequence.
	bool typingVariation = false;
	///The variance of the delay as a decimal percentage, used if typingVariation is true.
	float typingVarPercent = 0.5f;
	///Callback function for when the typeout is complete.
	std::function<void()> OnComplete;
	///Callback function for when the text is completely erased.
	std::function<void()> OnErased;
	///Process for when the typeout is complete.
	void onComplete();
	///Process for when the text is completely erased.
	void onErased();
	///Id for the sound effect that would play when text is added or removed.
	sf::String soundId = "";
	///Volume of the sound effect.
	float soundVolume = 100.0f;

public:
	///Used to call parent class functions when they are overrided in class.
	typedef VText VSUPERCLASS;
	///Delay in seconds between characters being updated in typeout.
	float Delay = 0.05f;
	///Delay in seconds between characters being updated in erase.
	float EraseDelay = 0.02f;
	///Amount of characters to update.
	int CharactersPerIteration = 1;
	///If true, a cursor character will be displayed at the end of the string.
	bool ShowCursor = false;
	///Character to represent the cursor.
	wchar_t CursorChar = '_';
	///Delay for cursor blinking.
	float CursorBlinkSpeed = 0.5f;
	///Time in seconds to wait between typing out and erased.
	float WaitTime = 1;
	///If true, the typeout or erase will be paused.
	bool Paused = false;
	///If true, the text will automatically be erased after it's been typed out.
	bool AutoErase = false;

	/**
	* @param x X position coordinate.
	* @param y Y position coordinate.
	* @param width The width of the text area that will bound the text.
	* @param text The string that the text object will display.
	* @param charSize The font size the text will be displayed at.
	*/
	VTypedText(float x = 0, float y = 0, float width = 0, const sf::String& text = "", int charSize = 8) : VText(x, y, width, "", charSize)
	{
		ResetText(text);
	}

	/**
	* Sets the prefix of the typed string.
	* @param text Text that is displayed before the updated string, is not erased or typed out.
	*/
	void SetPrefixText(const sf::String& text);
	
	/**
	* Set a sound to play on each letter when they appear/disappear.
	* @param id The ID name of the string as it will appear when it's accessed from VSoundManager.
	* @param volume Volume as a percentage (Range 0 - 100).
	* @param filename File path and name of sound effect file.
	*/
	void SetSound(sf::String id, float volume = 100.0f, sf::String filename = "");

	/**
	* Set a random delay when each letter is typed.
	* @param TypingVariation If true, typing variation will be enabled.
	* @param TypingVarPercent The decimal percentage of the typing variace between delays.
	*/
	void SetTimingVariation(bool TypingVariation, float TypingVarPercent = 0.5f);

	/**
	* Start a new text typeout.
	* @param Delay The fixed delay between characters being typed out in seconds.
	* @param ForceRestart If true, the string will start from being completely empty.
	* @param AutoErase If true, the text will automatically be eraised after typeout is complete.
	* @param OnComplete A callback function that will be used when typeout is complete.
	*/
	void Start(float Delay, bool ForceRestart = false, bool AutoErase = false, std::function<void()> OnComplete = nullptr);

	/**
	* Erase text like a reverse typeout.
	* @param Delay The fixed delay between characters being erased in seconds.
	* @param ForceRestart If true, the string will start from being a complete string.
	* @param OnErased A callback function that will be used when erase is complete.
	*/
	void Erase(float Delay, bool ForceRestart = false, std::function<void()> OnErased = nullptr);

	///@return True if text is currently being processed.
	bool IsActive() { return typing || waiting || erasing; }

	const std::wstring& GetPrefixText();

	/**
	* Reset the text that will be typed out, use this when changing text!
	* @param Text The string to set the text to.
	*/
	void ResetText(sf::String Text);
	///Skip the current typing out process.
	void Skip();
	///@return True if currently typing out.
	bool IsTyping()		{ return typing; }
	///@return True if text is currently erasing.
	bool IsErasing()	{ return erasing; }
	///@return True if text if currently waiting before erasing.
	bool IsWaiting()	{ return waiting; }
	///Destroys the text rendering data (and font if disposable is set for true).
	virtual void Destroy() override;

	/**
	* Updates text transform and rendering info.
	* @param dt Delta Time between the previous and current frame.
	*/
	virtual void Update(float dt) override;
};

