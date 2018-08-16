#include "VTypedText.h"
#include "VGlobal.h"
#include <sstream>

void VTypedText::SetSound(sf::String id, float volume, sf::String filename)
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

void VTypedText::Start(float Delay, bool ForceRestart, bool AutoErase, std::function<void()> OnComplete)
{
	this->Delay = Delay;

	if (!typing)
		timer = 0;

	typing = true;
	erasing = false;
	Paused = false;
	waiting = false;

	if (ForceRestart)
	{
		text = "";
		length = 0;
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
		text = finalText;
		length = finalText.size();
	}

	this->OnErased = OnErased;
}

void VTypedText::ResetText(sf::String text)
{
	this->text = "";
	finalText = text;
	typing = false;
	erasing = false;
	Paused = false;
	waiting = false;
	timer = 0;
	length = 0;
}

void VTypedText::Skip()
{
	length = finalText.size();
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

void VTypedText::Destroy()
{
	VSUPERCLASS::Destroy();
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
		if (length < (int)finalText.size() && typing)
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
	if (ShowCursor && length >= (int)finalText.size())
	{
		float futureCursorTime = cursorTimer + dt;

		if (futureCursorTime > CursorBlinkSpeed / 2)
		{
			cursorBlink = true;
			dirty = cursorTimer <= CursorBlinkSpeed / 2;
		}

		if (futureCursorTime > CursorBlinkSpeed)
		{
			cursorBlink = false;
			dirty = cursorTimer <= CursorBlinkSpeed;
			futureCursorTime = 0;
		}

		cursorTimer = futureCursorTime;
	}

	if (dirty)
	{
		if (cursorBlink)
			text = Prefix + finalText + CursorChar;
		else
			text = Prefix + finalText;
	}

	if (length >= (int)finalText.size() && typing && !waiting && !erasing)
	{
		onComplete();
	}
	if (length <= 0 && erasing && !typing && !waiting)
	{
		onErased();
	}

	VSUPERCLASS::Update(dt);
}