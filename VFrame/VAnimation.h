#pragma once
#include <vector>

class VAnimation
{
private:
	int totalFrames = 0;
	int currentFrame = 0;

	float frameDelay = 0;
	float frameTime = 0;

	std::vector<int> frames;

	bool looping = false;
	bool reverse = false;

public:
	VAnimation(const std::vector<int>& Frames, float FramesPerSecond, bool Looping, bool Reverse) : frames(Frames)
	{
		if (FramesPerSecond > 0)
			frameDelay = 1 / FramesPerSecond;
		else
			frameDelay = 0;

		totalFrames = frames.size();
		looping = Looping;
		reverse = Reverse;
	}

	virtual ~VAnimation() 
	{
		frames.clear();
		frames.shrink_to_fit();
	};

	void Reset()
	{
		currentFrame = 0;
		frameTime = 0;
	}

	void SetCurrentFrame(int NewFrame)
	{
		if (NewFrame < totalFrames)
			currentFrame = NewFrame;
	}

	int GetCurrentFrame()
	{
		return frames[currentFrame];
	}

	int GetFrameCount()
	{
		return totalFrames;
	}

	void SetLooping(bool Looping)
	{
		looping = Looping;
	}

	void SetReverse(bool Reverse)
	{
		reverse = Reverse;
	}

	void Update(float dt)
	{
		if (frameDelay > 0)
		{
			frameTime += dt;

			if (frameTime >= frameDelay)
			{
				int amount = 0;
				if (!reverse)
				{
					amount = 1;
				}
				else
				{
					amount = -1;
				}

				if (looping)
				{
					currentFrame = (currentFrame + totalFrames + amount) % totalFrames;
				}
				else
				{
					currentFrame = currentFrame + amount < 0 ? 
						0 : currentFrame + amount >= totalFrames ? 
						totalFrames - 1 : currentFrame + amount;
				}

				frameTime = 0;
			}
		}
	}
};

