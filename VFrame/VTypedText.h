#pragma once
#include "VText.h"
#include <functional>
#include <SFML/Audio/Sound.hpp>
#include <SFML/Audio/SoundBuffer.hpp>

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
	char CursorChar = '_';
	float CursorBlinkSpeed = 0.5f;
	sf::String Prefix = "";
	float WaitTime = 1;
	bool Paused = false;
	bool AutoErase = false;

	VTypedText(float x = 0, float y = 0, float width = 0, const sf::String& text = "", int charSize = 8) : VText(x, y, width, "", charSize)
	{
		ResetText(text);
	}
	virtual ~VTypedText()
	{
		Destroy();
	}

	void SetSound(sf::String id, float volume = 100.0f, sf::String filename = "");
	void SetTimingVariation(bool TypingVariation, float TypingVarPercent = 0.5f);
	void Start(float Delay, bool ForceRestart = false, bool AutoErase = false, std::function<void()> OnComplete = nullptr);
	void Erase(float Delay, bool ForceRestart = false, std::function<void()> OnErased = nullptr);
	bool IsActive() { return typing || waiting || erasing; }
	void ResetText(sf::String Text);
	void Skip();
	bool IsTyping()		{ return typing; }
	bool IsErasing()	{ return erasing; }
	bool IsWaiting()	{ return waiting; }
	virtual void Destroy() override;
	virtual void Update(float dt) override;
};

