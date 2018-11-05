/**
* @file    VAnimationManager.h
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
* Class for handling a set of VAnimation objects for a renderable object such as VSprite.
*/

#pragma once
#include "VAnimation.h"
#include <SFML/System/String.hpp>
#include <iostream>
#include <map>
#include <vector>
#include <numeric>

///Handles storing and switching between multiple VAnimations.
class VAnimationManager
{
private:
	///List of animations, categorised by an sf::String ID value.
	std::map<sf::String, VAnimation*> animationList;
	///The current animation that is currently being played. String is blank if no animation is currently being used.
	sf::String currentAnim;
	///The current frame number to display.
	int current = 0;
	///The number of the last frame that was rendered. Used in the manager to perform certain actions if the current frame has been changed.
	int lastFrame = -1;
	///The total amount of frames horizontal and vertical down the texture.
	int animWidth = 0, animHeight = 0;
	///The width and height of a single frame in animation.
	int frameWidth = 0, frameHeight = 0;
	///An offset in the X and Y axis in pixels, in case an animation doesn't encompass an entire texture.
	int offsetX = 0, offsetY = 0;
	///The current animation will be paused if true, otherwise the animation will play as normal.
	bool pause = false;

public:
	VAnimationManager() {}

	/**
	* @param TextureWidth The width of the texture in pixels being used, or the partial width if the entire texture won't be used.
	* @param TextureHeight The height of the texture in pixels being used, or the partial height if the entire texture won't be used.
	* @param FrameWidth The width of a single frame in pixels.
	* @param FrameHeight The height of a single frame in pixels.
	* @param OffsetX The offset of the X axis in pixels, imporant if the area of animation is not the entire texture.
	* @param OffsetY The offset of the Y axis in pixels, imporant if the area of animation is not the entire texture.
	*/
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

	/**
	* @return An sf::IntRect of the entire area of the texture where the animation is used.
	*/
	sf::IntRect GetTextureArea()
	{
		return sf::IntRect(offsetX, offsetY, frameWidth, frameHeight);
	}

	/**
	* Adds a new animation to the manager. If an animation under the name already exists, then a new VAnimation will replace it.
	* @param name Name of animation, used for the ID in the animation list.
	* @param Frames Frames The list of Frames the animation will have, you can pass in an std::vector<int> object or an array of ints using braces (e.g. { 0, 3, 7, 8 }).
	* @param FramesPerSecond The amount of frames to play per second in time. This gets converted to a frameDelay using 1 / FramesPerSecond. Must be greater than 0.
	* @param Looping Whether the animation should loop or not.
	* @param Reverse Whether the animation should reverse or not.
	*/
	void AddAnimation(const sf::String& name, const std::vector<int>& Frames, float FramesPerSecond = 6.0f, bool Looping = false, bool Reverse = false)
	{
		if (animationList.find(name) == animationList.end())
		{
			animationList.emplace(std::make_pair(name, new VAnimation(Frames, FramesPerSecond, Looping, Reverse)));
		}
		else
		{
			delete animationList[name];
			animationList[name] = new VAnimation(Frames, FramesPerSecond, Looping, Reverse);
		}
	}

	/**
	* Adds a new animation to the manager. If an animation under the name already exists, then a new VAnimation will replace it.
	* Unlike the other method, this one allows will set up a list of frames is of a range of numbers, to save passing in a std::vector<int>.
	* @param name Name of animation, used for the ID in the animation list.
	* @param Begin The beginning frame number of the animation.
	* @param Length The total amount of frames this animation will have. (e.g. if Begin is 0 and Length is 5, the frame list created will be { 0, 1, 2, 3, 4 })
	* @param FramesPerSecond The amount of frames to play per second in time. This gets converted to a frameDelay using 1 / FramesPerSecond. Must be greater than 0.
	* @param Looping Whether the animation should loop or not.
	* @param Reverse Whether the animation should reverse or not.
	*/
	void AddAnimation(const sf::String& name, int Begin, int Length, float FramesPerSecond = 6.0f, bool Looping = false, bool Reverse = false)
	{
		std::vector<int> frames(Length);
		std::iota(frames.begin(), frames.end(), Begin);
		AddAnimation(name, frames, FramesPerSecond, Looping, Reverse);
	}

	/**
	*  @return returns the String name/id of the currently playing animation.
	*/
	sf::String CurrentAnimationName()
	{
		return currentAnim;
	}

	/**
	* Plays an animation. If the animation specified is already being played then nothing will happen. If you wish to replay the current animation, use the Reset function.
	* @param name The name/id of the animation you wish to play. Must be an animation previously added to the scene.
	* @param onComplete A callback function, used when the animation is completed.
	*/
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

	/**
	* @param value True pauses the current animation, False resumes the current animation.
	*/
	void SetPaused(bool value)
	{
		pause = value;
	}

	///Stop all animations.
	void Stop()
	{
		currentAnim = "";
	}

	/**
	* Resets the current animation being played.
	* @param onComplete A callback function, used when the animation is completed.
	*/
	void Reset(std::function<void()> onComplete = nullptr)
	{
		Reset(currentAnim, onComplete);
	}

	/**
	* Resets one of the animations in the manager.
	* @param name The name/id of the animation to use.
	* @param onComplete A callback function, used when the animation is completed.
	*/
	void Reset(const sf::String& name, std::function<void()> onComplete = nullptr)
	{
		animationList[name]->Reset(onComplete);
	}

	/**
	* Updates the current animation, assuming pause isn't set to true.
	* @param dt Delta Time between the current and previous frame in the game.
	*/
	void Update(float dt)
	{
		if (currentAnim != "" && !pause)
		{
			if (animationList[currentAnim]->GetFrameCount() > 1)
			{
				//lastFrame = animationList[currentAnim]->GetCurrentFrame();
				animationList[currentAnim]->Update(dt);
			}
		}
	}

	/**
	* Sets the current frame index the current animation. Do not use this to set the current animation's frame number, as this is specified in the current animation's frame list.
	* The only exception is if no current animation has been set, then the frame number can be passed in.
	* @param NewFrame The frame index the current animation should use. This must be greater than 0 and less than the total amount of frames specified in the current animation's frame list.
	*/
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

	/**
	*  @return The currently used frame number, regardless of whether an animation is currently being used or not.
	*/
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

	/**
	*  @return The previous frame used in the last game's loop.
	*/
	int GetLastFrame()
	{
		return lastFrame;
	}

	/**
	* @param Looping If true, the current animation will now loop, else it won't loop.
	*/
	void SetLooping(bool Looping)
	{
		if (currentAnim != "")
		{
			lastFrame = -1;
			animationList[currentAnim]->SetLooping(Looping);
		}
	}

	/**
	* @param Reverse If true, the current animation will now play in reverse, else it won't.
	*/
	void SetReverse(bool Reverse)
	{
		if (currentAnim != "")
		{
			lastFrame = -1;
			animationList[currentAnim]->SetReverse(Reverse);
		}
	}

	/**
	*  @return Returns the current U/X coordinate of the texture for rendering purposes.
	*/
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

	/**
	*  @return Returns the current V/Y coordinate of the texture for rendering purposes.
	*/
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

	///Clears entire animation list.
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

	/**
	*  @return Returns the amount of animations being stored in the animation manager.
	*/
	int Count()
	{
		return animationList.size();
	}
};

