#pragma once
#include "VText.h"
#include <functional>
#include <SFML/Audio/Sound.hpp>
#include <SFML/Audio/SoundBuffer.hpp>

//Special Text class that displays text in a typewriter effect.
class VTypedText : public VText
{
private:
	std::wstring finalText = L"";
	float timer = 0;
	float cursorTimer = 0;
	float waitTimer = 0;
	int length = 0;

	bool typing = false;
	bool erasing = false;
	bool waiting = false;

	bool typingVariation = false;
	float typingVarPercent = 0.5f;

	std::function<void()> OnComplete;
	std::function<void()> OnErased;

	void onComplete();
	void onErased();

	sf::String soundId = "";
	float soundVolume = 100.0f;

public:
	typedef VText VSUPERCLASS;
	float Delay = 0.05f;
	float EraseDelay = 0.02f;
	int CharactersPerIteration = 1;
	bool ShowCursor = false;
	wchar_t CursorChar = '_';
	float CursorBlinkSpeed = 0.5f;
	sf::String Prefix = "";
	float WaitTime = 1;
	bool Paused = false;
	bool AutoErase = false;

	VTypedText(float x = 0, float y = 0, float width = 0, const sf::String& text = "", int charSize = 8) : VText(x, y, width, "", charSize)
	{
		ResetText(text);
	}
	
	//Set a sound to play on each letter when they appear/disappear.
	void SetSound(sf::String id, float volume = 100.0f, sf::String filename = "");
	//Set a random delay when each letter is typed.
	void SetTimingVariation(bool TypingVariation, float TypingVarPercent = 0.5f);
	//Start a new text typeout.
	void Start(float Delay, bool ForceRestart = false, bool AutoErase = false, std::function<void()> OnComplete = nullptr);
	//Erase text like a reverse typeout.
	void Erase(float Delay, bool ForceRestart = false, std::function<void()> OnErased = nullptr);
	//Returns true if text is currently being processed.
	bool IsActive() { return typing || waiting || erasing; }
	//Reset the text that will be typed out, use this when changing text!
	void ResetText(sf::String Text);
	//Skip the current typing out process.
	void Skip();
	//Returns true if currently typing out.
	bool IsTyping()		{ return typing; }
	//Returns true if text is currently erasing.
	bool IsErasing()	{ return erasing; }
	//Returns true if text if currently waiting before erasing.
	bool IsWaiting()	{ return waiting; }
	virtual void Destroy() override;
	virtual void Update(float dt) override;
};

