#include "VAnimationManager.h"

VAnimation::VAnimation(const std::vector<int>& Frames, float FramesPerSecond, bool Looping, bool Reverse) : frames(Frames), looping(Looping), reverse(Reverse)
{
	if (FramesPerSecond > 0)
		frameDelay = 1 / FramesPerSecond;
	else
		frameDelay = 0;

	totalFrames = frames.size();
}

VAnimation::~VAnimation()
{
	frames.clear();
	frames.shrink_to_fit();
};

void VAnimation::Reset(std::function<void()> onComplete)
{
	currentFrame = 0;
	frameTime = 0;
	onCompleteCallback = onComplete;
}

void VAnimation::SetCurrentFrame(int NewFrame)
{
	if (NewFrame < totalFrames)
		currentFrame = NewFrame;
}

int VAnimation::GetCurrentFrame()
{
	return frames[currentFrame];
}

int VAnimation::GetFrameCount()
{
	return totalFrames;
}

void VAnimation::SetLooping(bool Looping)
{
	looping = Looping;
}

void VAnimation::SetReverse(bool Reverse)
{
	reverse = Reverse;
}

void VAnimation::Update(float dt)
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
				if (onCompleteCallback != nullptr)
				{
					if ((reverse && currentFrame == 0) ||
						(!reverse && currentFrame == totalFrames - 1))
					{
						onCompleteCallback();
						onCompleteCallback = nullptr;
					}
				}

				currentFrame = currentFrame + amount < 0 ?
					0 : currentFrame + amount >= totalFrames ?
					totalFrames - 1 : currentFrame + amount;
			}

			frameTime = 0;
		}
	}
}

VAnimationManager::VAnimationManager() {}

VAnimationManager::VAnimationManager(int TextureWidth, int TextureHeight, int FrameWidth, int FrameHeight, int OffsetX, int OffsetY)
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

VAnimationManager::~VAnimationManager()
{
	Clear();
}

sf::IntRect VAnimationManager::GetTextureArea()
{
	return sf::IntRect(offsetX, offsetY, frameWidth, frameHeight);
}

void VAnimationManager::AddAnimation(const sf::String& name, const std::vector<int>& Frames, float FramesPerSecond, bool Looping, bool Reverse)
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

void VAnimationManager::AddAnimation(const sf::String& name, int Begin, int Length, float FramesPerSecond, bool Looping, bool Reverse)
{
	std::vector<int> frames(Length);
	std::iota(frames.begin(), frames.end(), Begin);
	AddAnimation(name, frames, FramesPerSecond, Looping, Reverse);
}

sf::String VAnimationManager::CurrentAnimationName()
{
	return currentAnim;
}

void VAnimationManager::Play(const sf::String& name, std::function<void()> onComplete)
{
	if (animationList.find(name) != animationList.end() &&
		currentAnim != name)
	{
		lastFrame = -1;
		currentAnim = name;
		Reset(onComplete);
	}
}

void VAnimationManager::SetPaused(bool value)
{
	pause = value;
}

void VAnimationManager::Stop()
{
	currentAnim = "";
}

void VAnimationManager::Reset(std::function<void()> onComplete)
{
	Reset(currentAnim, onComplete);
}

void VAnimationManager::Reset(const sf::String& name, std::function<void()> onComplete)
{
	animationList[name]->Reset(onComplete);
}

void VAnimationManager::Update(float dt)
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

void VAnimationManager::SetCurrentFrame(int NewFrame)
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

int VAnimationManager::GetCurrentFrame()
{
	if (currentAnim != "")
	{
		lastFrame = animationList[currentAnim]->GetCurrentFrame();
		return lastFrame;
	}

	lastFrame = current;
	return lastFrame;
}

int VAnimationManager::GetLastFrame()
{
	return lastFrame;
}

void VAnimationManager::SetLooping(bool Looping)
{
	if (currentAnim != "")
	{
		lastFrame = -1;
		animationList[currentAnim]->SetLooping(Looping);
	}
}

void VAnimationManager::SetReverse(bool Reverse)
{
	if (currentAnim != "")
	{
		lastFrame = -1;
		animationList[currentAnim]->SetReverse(Reverse);
	}
}

int VAnimationManager::GetU()
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

int VAnimationManager::GetV()
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

void VAnimationManager::Clear()
{
	for (std::pair<sf::String, VAnimation*> anim : animationList)
	{
		delete anim.second;
		anim.second = nullptr;
	}
	animationList.clear();
	currentAnim = "";
}

int VAnimationManager::Count()
{
	return animationList.size();
}