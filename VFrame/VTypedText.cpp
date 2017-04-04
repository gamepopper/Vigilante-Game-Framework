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
		Text = "";
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
		Text = finalText;
		length = finalText.size();
	}

	this->OnErased = OnErased;
}

void VTypedText::ResetText(sf::String text)
{
	Text = "";
	finalText = text;
	typing = false;
	erasing = false;
	Paused = false;
	waiting = false;
	timer = 0;
}

void VTypedText::Skip()
{
	length = finalText.size();
	typing = false;
	erasing = false;
	Paused = false;
	waiting = false;
	timer = 0;
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
		}

		if (erasing && timer >= EraseDelay)
		{
			length -= CharactersPerIteration;
			if (soundId != "") VGlobal::p()->Sound->Play(soundId, soundVolume);
		}

		if ((typing && timer >= Delay) || (erasing && timer >= EraseDelay))
		{
			if (typingVariation)
			{
				if (typing)
				{
					timer = VGlobal::p()->Random.GetFloat(Delay * typingVarPercent / 2, -Delay * typingVarPercent / 2);
				}
				else
				{
					timer = VGlobal::p()->Random.GetFloat(EraseDelay * typingVarPercent / 2, -EraseDelay * typingVarPercent / 2);
				}
			}
			else
			{
				timer = 0;
			}
		}
	}

	std::wstringstream helperString;
	helperString << Prefix.toWideString() << finalText.substr(0, length);

	if (ShowCursor && length >= (int)finalText.size())
	{
		cursorTimer += dt;

		if (cursorTimer > CursorBlinkSpeed / 2)
		{
			helperString << CursorChar;
		}

		if (cursorTimer > CursorBlinkSpeed)
		{
			cursorTimer = 0;
		}
	}

	if (helperString.str() != Text)
	{
		helperText = helperString.str();

		Text = helperText;
		helperText = { (char)0 };

		if (Text.getSize() != finalText.size() && Text.getSize() > 0)
		{
			float advance = 0.0f;
			int lastWord = 0;
			bool bold = Style & sf::Text::Bold;
			for (unsigned int i = 0; i < Text.getSize(); i++)
			{
				auto currentChar = Text[i];

				if (currentChar == ' ')
				{
					lastWord = i;
				}

				if (currentChar == '\n')
				{
					advance = 0;
				}

				sf::Glyph glyph = font.getGlyph(currentChar, FontSize, bold);
				advance += glyph.advance;

				if (advance > Size.x)
				{
					advance = 0;
				}
			}

			unsigned int nextWord = lastWord + 1;

			while (Text[nextWord] != ' ' && nextWord < Text.getSize())
			{
				sf::Glyph glyph = font.getGlyph(Text[nextWord], FontSize, bold);
				advance += glyph.advance;

				nextWord++;
			}

			if (advance > Size.x)
			{
				Text[nextWord] = '\n';
				advance = 0;
			}
		}
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