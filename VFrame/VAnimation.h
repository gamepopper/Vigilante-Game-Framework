/**
* @file    VAnimation.h
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
* Class for handling a single texel frame-based animation. Frames are organised left to right, top to bottom.
*/

#pragma once
#include <vector>
#include <functional>

///Handler for a single sprite based frame animation.
class VAnimation
{
private:
	///Total amount of possible frames in a texture.
	int totalFrames = 0;
	///Current frame index that is being rendered for the object.
	int currentFrame = 0;
	///The amount of time to delay before changing from one frame to another.
	float frameDelay = 0;
	///Current time between frames.
	float frameTime = 0;
	///List of frame numbers to show in order.
	std::vector<int> frames;
	///Function to call when an animation is finished, assuming that looping has been set to false.
	std::function<void()> onCompleteCallback;
	///If true, an animation will loop back to the start. Else the animation will stop at the last frame.
	bool looping = false;
	///If true, the animation will play in the reverse order. Even when reversed, looping and onCompleteCallback can effect the animation the same as not.
	bool reverse = false;

public:

	/**
	* @param Frames The list of Frames the animation will have, you can pass in an std::vector<int> object or an array of ints using braces (e.g. { 0, 3, 7, 8 }).
	* @param FramesPerSecond The amount of frames to play per second in time. This gets converted to a frameDelay using 1 / FramesPerSecond. Must be greater than 0.
	* @param Looping Whether the animation should loop or not.
	* @param Reverse Whether the animation should reverse or not.
	*/
	VAnimation(const std::vector<int>& Frames, float FramesPerSecond, bool Looping, bool Reverse);

	virtual ~VAnimation();

	/**
	* Resets the animation to the beginning.
	* @param onComplete A callback function used when an animation reaches its end (assuming the animation isn't Looping).
	*/
	void Reset(std::function<void()> onComplete = nullptr);

	/**
	* Sets the current frame index the animation uses. Do not use this to set the animation's frame number, as this is specified in the frame list.
	* @param NewFrame the frame index the animation should use. This must be greater than 0 and less than the total amount of frames specified in the frame list.
	*/
	void SetCurrentFrame(int NewFrame);

	/**
	*  @return returns the currently used frame number.
	*/
	int GetCurrentFrame();

	/**
	*  @return returns the total amount of frames in the frame list.
	*/
	int GetFrameCount();

	/**
	* @param Looping If true, the animation will now loop, else it won't loop.
	*/
	void SetLooping(bool Looping);

	/**
	* @param Reverse If true, the animation will now play in reverse, else it won't.
	*/
	void SetReverse(bool Reverse);

	/**
	* Updates the animation, handles changing of frame, callbacks and passing of time.
	* @param dt Delta Time between the current and previous frame in the game.
	*/
	void Update(float dt);
};

