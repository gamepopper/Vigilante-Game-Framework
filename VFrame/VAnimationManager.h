#pragma once
#include "VAnimation.h"
#include <SFML/System/String.hpp>
#include <iostream>
#include <map>
#include <vector>
#include <numeric>

class VAnimationManager
{
private:
	std::map<sf::String, VAnimation*> animationList;
	sf::String currentAnim;
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
        if (FrameWidth > 0 && FrameHeight > 0)
        {
            animWidth = TextureWidth / FrameWidth;
            animHeight = TextureHeight / FrameHeight;
		}

		frameWidth = FrameWidth;
		frameHeight = FrameHeight;
		offsetX = OffsetX;
		offsetY = OffsetY;
	}

	virtual ~VAnimationManager()
	{
		Clear();
	}

	sf::IntRect GetTextureArea()
	{
		return sf::IntRect(offsetX, offsetY, frameWidth, frameHeight);
	}

	/*
	If animation under name does not exist, creates new VAnimation object for animation.
	name: Name of animation.
	Frames: Frame sequence of animation (based on sprite sheet atlas in order left to right).
	FramesPerSecond: Amount of frames to show per seconds.
	Looping: Whether animation should loop or not.
	Reverse: Whether animation should be reversed or not.
	*/
	void AddAnimation(const sf::String& name, const std::vector<int>& Frames, float FramesPerSecond = 6.0f, bool Looping = false, bool Reverse = false)
	{
		if (animationList.find(name) == animationList.end())
		{
			animationList.insert(std::pair<sf::String, VAnimation*>(name, new VAnimation(Frames, FramesPerSecond, Looping, Reverse)));
		}
	}

	/*
	If animation under name does not exist, creates new VAnimation object for animation. This one sets up animation from a range of ints.
	name: Name of animation.
	Begin: Initial Frame of Range
	End: Final Frame of Range
	FramesPerSecond: Amount of frames to show per seconds.
	Looping: Whether animation should loop or not.
	Reverse: Whether animation should be reversed or not.
	*/
	void AddAnimation(const sf::String& name, int Begin, int Length, float FramesPerSecond = 6.0f, bool Looping = false, bool Reverse = false)
	{
		std::vector<int> frames(Length);
		std::iota(frames.begin(), frames.end(), Begin);
		AddAnimation(name, frames, FramesPerSecond, Looping, Reverse);
	}

	//Name of current playing animation.
	sf::String CurrentAnimationName()
	{
		return currentAnim;
	}

	//Play an animation using its name.
	void Play(const sf::String& name, std::function<void()> onComplete = nullptr)
	{
		if (animationList.find(name) != animationList.end() &&
			currentAnim != name)
		{
			lastFrame = -1;
			currentAnim = name;
			Reset(onComplete);
		}
	}

	//Set whether animation should be paused.
	void SetPaused(bool value)
	{
		pause = value;
	}

	//Stop all animations.
	void Stop()
	{
		currentAnim = "";
	}

	//Resets current animation.
	void Reset(std::function<void()> onComplete = nullptr)
	{
		Reset(currentAnim, onComplete);
	}

	//Resets specific animations.
	void Reset(const sf::String& name, std::function<void()> onComplete = nullptr)
	{
		animationList[name]->Reset(onComplete);
	}

	void Update(float dt)
	{
		if (currentAnim != "" && !pause)
		{
			if (animationList[currentAnim]->GetFrameCount() > 1)
			{
				lastFrame = animationList[currentAnim]->GetCurrentFrame();
				animationList[currentAnim]->Update(dt);
			}
		}
	}

	//Set current frame in animation.
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

	//Gets the previous frame of animation.
	int GetLastFrame()
	{
		return lastFrame;
	}

	//Sets current playing animation to looping or not.
	void SetLooping(bool Looping)
	{
		if (currentAnim != "")
		{
			lastFrame = -1;
			animationList[currentAnim]->SetLooping(Looping);
		}
	}

	//Sets current playing animation to be reversed or not.
	void SetReverse(bool Reverse)
	{
		if (currentAnim != "")
		{
			lastFrame = -1;
			animationList[currentAnim]->SetReverse(Reverse);
		}
	}

	//Gets U (Texture X) coordinate.
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

	//Gets V (Texture Y) coordinate.
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

	//Clears entire animation list.
	void Clear()
	{
		for (std::pair<sf::String, VAnimation*> anim : animationList)
		{
			delete anim.second;
			anim.second = nullptr;
		}
		animationList.clear();
		currentAnim = "";
	}

	//Returns total amount of animations.
	int Count()
	{
		return animationList.size();
	}
};

