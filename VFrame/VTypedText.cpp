#include "VTypedText.h"
#include "VGlobal.h"
#include <sstream>

VTypedText::VTypedText(float x, float y, float width, const sf::String& text, int charSize) : VText(x, y, width, "", charSize)
{
	ResetText(text);
}

void VTypedText::SetSound(const sf::String& id, float volume, const sf::String& filename)
{
	if (filename != "")
	{
		VGlobal::p()->Sound->Load(filename, id);
	}

	soundId = id;
	soundVolume = volume;
}

void VTypedText::SetTimingVariation(bool TypingVariation, float TypingVarPercent)
{
	typingVarPercent = TypingVarPercent <= 1 ? TypingVarPercent >= 0 ? TypingVarPercent : 0 : 1;
	typingVariation = TypingVariation;
}

void VTypedText::SetPrefixText(const sf::String& text)
{
	if (prefix != text)
	{
		prefix = text;
		length = (int)prefix.size();
		dirty = true;
	}
}

const std::wstring& VTypedText::GetPrefixText()
{
	return prefix;
}

void VTypedText::Start(float Delay, bool ForceRestart, bool AutoErase, std::function<void()> OnComplete)
{
	this->Delay = Delay;

	if (!typing)
		timer = 0;

	dirty = true;
	typing = true;
	erasing = false;
	Paused = false;
	waiting = false;

	if (ForceRestart)
	{
		text = "";
		length = (int)prefix.size();
	}

	if (AutoErase)
	{
		this->EraseDelay = Delay;
	}

	this->AutoErase = AutoErase;
	this->OnComplete = OnComplete;
}

void VTypedText::Erase(float Delay, bool ForceRestart, std::function<void()> OnErased)
{
	this->EraseDelay = Delay;

	if (!erasing)
		timer = 0;

	typing = false;
	erasing = true;
	Paused = false;
	waiting = false;

	if (ForceRestart)
	{
		text = prefix + finalText;
		length = (int)(prefix.size() + finalText.size());
	}

	this->OnErased = OnErased;
}

void VTypedText::ResetText(const sf::String& text)
{
	this->text = "";
	finalText = text;
	typing = false;
	erasing = false;
	Paused = false;
	waiting = false;
	timer = 0;
	length = prefix.size();
}

void VTypedText::Skip()
{
	length = (int)(prefix.size() + finalText.size());
	typing = false;
	erasing = false;
	Paused = false;
	waiting = false;
	timer = 0;

	dirty = true;
}

void VTypedText::onComplete()
{
	typing = false;
	erasing = false;
	timer = 0;

	if (AutoErase && WaitTime <= 0)
	{
		erasing = true;
	}
	else if (AutoErase)
	{
		waitTimer = WaitTime;
		waiting = true;
	}
	else
	{
		if (OnComplete != nullptr)
		{
			OnComplete();
		}
	}
}

void VTypedText::onErased()
{
	typing = false;
	erasing = false;
	timer = 0;
	finalText = L"";

	if (AutoErase)
	{
		if (OnComplete != nullptr)
		{
			OnComplete();
		}
	}
	else
	{
		if (OnErased != nullptr)
		{
			OnErased();
		}
	}
}

void VTypedText::Update(float dt)
{
	if (waiting && !Paused)
	{
		waitTimer -= dt;

		if (waitTimer <= 0)
		{
			waiting = false;
			erasing = true;
		}
	}

	if (!waiting && !Paused)
	{
		if (length < (int)(prefix.size() + finalText.size()) && typing)
		{
			timer += dt;
		}

		if (length >= 0 && erasing)
		{
			timer += dt;
		}
	}

	if (typing || erasing)
	{
		if (typing && timer >= Delay)
		{
			length += CharactersPerIteration;
			if (soundId != "") VGlobal::p()->Sound->Play(soundId, soundVolume);
			dirty = true;
		}

		if (erasing && timer >= EraseDelay)
		{
			length -= CharactersPerIteration;
			if (soundId != "") VGlobal::p()->Sound->Play(soundId, soundVolume);
			dirty = true;
		}

		if ((typing && timer >= Delay) || (erasing && timer >= EraseDelay))
		{
			if (typingVariation)
			{
				if (typing)
				{
					timer = VGlobal::p()->Random->GetFloat(Delay * typingVarPercent / 2, -Delay * typingVarPercent / 2);
				}
				else
				{
					timer = VGlobal::p()->Random->GetFloat(EraseDelay * typingVarPercent / 2, -EraseDelay * typingVarPercent / 2);
				}
			}
			else
			{
				timer = 0;
			}
		}
	}

	bool cursorBlink = false;
	if (ShowCursor && length >= (int)(prefix.size() + finalText.size()))
	{
		float futureCursorTime = cursorTimer + dt;

		if (futureCursorTime > CursorBlinkSpeed / 2)
		{
			cursorBlink = true;
			length = (int)(prefix.size() + finalText.size()) + 1;
			dirty = true;
		}

		if (futureCursorTime > CursorBlinkSpeed)
		{
			cursorBlink = false;
			length = (int)(prefix.size() + finalText.size());
			dirty = true;
			futureCursorTime = 0;
		}

		cursorTimer = futureCursorTime;
	}

	if (dirty)
	{
		text = prefix + finalText + CursorChar;
	}

	if (length >= (int)(prefix.size() + finalText.size()) && typing && !waiting && !erasing)
	{
		onComplete();
	}
	if (length <= (int)prefix.size() && erasing && !typing && !waiting)
	{
		onErased();
	}

	VSUPERCLASS::Update(dt);
}