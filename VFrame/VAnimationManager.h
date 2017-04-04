#pragma once
#include "VAnimation.h"
#include <SFML\System\String.hpp>
#include <iostream>
#include <map>
#include <vector>

class VAnimationManager
{
private:
	std::map<sf::String, VAnimation*> animationList;
	sf::String currentAnim = "";
	int current = 0;
	int lastFrame = -1;
	int animWidth = 0, animHeight = 0;
	int frameWidth = 0, frameHeight = 0;
	int offsetX = 0, offsetY = 0;
	bool pause = false;

public:
	VAnimationManager() {}
	VAnimationManager(int TextureWidth, int TextureHeight, int FrameWidth, int FrameHeight, int OffsetX, int OffsetY)
	{
		animWidth = TextureWidth / FrameWidth;
		animHeight = TextureHeight / FrameHeight;
		frameWidth = FrameWidth;
		frameHeight = FrameHeight;
		offsetX = OffsetX;
		offsetY = OffsetY;
	}
	virtual ~VAnimationManager()
	{
		Clear();
	}

	void AddAnimation(sf::String name, std::vector<int> Frames, float FramesPerSecond = 6.0f, bool Looping = false, bool Reverse = false)
	{
		if (animationList.count(name) == NULL)
		{
			animationList.insert(std::pair<sf::String, VAnimation*>(name, new VAnimation(Frames, FramesPerSecond, Looping, Reverse)));
		}
	}

	void Play(sf::String name)
	{
		if (animationList.count(name) != NULL && 
			currentAnim != name)
		{
			lastFrame = -1;
			currentAnim = name;
			Reset();
		}
	}

	void IsPaused(bool value)
	{
		pause = value;
	}

	void Stop()
	{
		currentAnim = "";
	}

	void Reset()
	{
		Reset(currentAnim);
	}

	void Reset(sf::String name)
	{
		animationList[name]->Reset();
	}

	void Update(float dt)
	{
		if (currentAnim != "" && !pause)
		{ 
			lastFrame = animationList[currentAnim]->GetCurrentFrame();
			animationList[currentAnim]->Update(dt);
		}
	}

	void SetCurrentFrame(int NewFrame)
	{
		if (currentAnim != "")
		{
			lastFrame = -1;
			animationList[currentAnim]->SetCurrentFrame(NewFrame);
		}
		else
		{
			current = NewFrame;
			lastFrame = -1;
		}
	}

	int GetCurrentFrame()
	{
		if (currentAnim != "")
		{
			lastFrame = animationList[currentAnim]->GetCurrentFrame();
			return lastFrame;
		}
		lastFrame = current;
		return lastFrame;
	}

	int GetLastFrame()
	{
		return lastFrame;
	}

	void SetLooping(bool Looping)
	{
		if (currentAnim != "")
		{
			lastFrame = -1;
			animationList[currentAnim]->SetLooping(Looping);
		}
	}

	void SetReverse(bool Reverse)
	{
		if (currentAnim != "")
		{
			lastFrame = -1;
			animationList[currentAnim]->SetReverse(Reverse);
		}
	}

	int GetU()
	{
		current = GetCurrentFrame();
		if (current > 0)
		{
			int U;
			U = current % animWidth;
			U *= frameWidth;

			return U + offsetX;
		}
		return offsetX;
	}

	int GetV()
	{
		current = GetCurrentFrame();
		if (current > 0)
		{
			int V;
			V = current / animWidth;
			V *= frameHeight;

			return V + offsetY;
		}

		return offsetY;
	}

	void Clear()
	{
		for each (std::pair<sf::String, VAnimation*> anim in animationList)
		{
			delete anim.second;
			anim.second = NULL;
		}
		animationList.clear();
		currentAnim = "";
	}

	int Count()
	{
		return animationList.size();
	}
};

