#include "VCamera.h"

sf::View VCameraList::Default;

#include "VGlobal.h"
#include "VInterpolate.h"

void VCamera::Reset()
{
	view = VCameraList::Default;
	block.setSize(view.getSize());
	Position = view.getCenter();
	LastPosition = Position;
	Rotation = view.getRotation();
	Zoom = 1.0f;
	followObject = nullptr;
	isFollowing = false;
	isShaking = false;
}

void VCamera::Follow(VObject* target, float deadzone, CameraFollowMethod followmethod, float lead, float lerp)
{
	followObject = target;
	deadZone = deadzone;
	followMethod = followmethod;
	followLead = lead;
	followLerp = lerp;
	isFollowing = true;
	Position = followObject->Position + (followObject->Size / 2.0f);
}

void VCamera::StopFollow()
{
	followObject = nullptr;
	isFollowing = false;
}

bool VCamera::Shake(float intensity, float time, std::function<void()> OnComplete)
{
	if (!isShaking)
	{
		shakeX = shakeY = intensity;
		shakeLength = time;
		shakeTimer.restart();
		OnShakeComplete = OnComplete;
		isShaking = true;
		return true;
	}
	return false;
}


bool VCamera::Shake(sf::Vector2f intensity, float time, std::function<void()> OnComplete)
{
	if (!isShaking)
	{
		shakeX = intensity.x;
		shakeY = intensity.y;
		shakeLength = time;
		shakeTimer.restart();
		OnShakeComplete = OnComplete;
		isShaking = true;
		return true;
	}
	return false;
}

bool VCamera::Fade(sf::Color colour, float time, bool fadein, std::function<void()> OnComplete)
{
	if (!isFlashing && !isFading)
	{
		fadeColour = colour;
		fadeTime = time > 0 ? time : 0.00001f;
		fadeIn = fadein;
		fadeFlashTimer.restart();
		OnFadeComplete = OnComplete;
		isFading = true;
		isFlashing = false;
		return true;
	}
	return false;
}

bool VCamera::Flash(sf::Color colour, float time, std::function<void()> OnComplete)
{
	if (!isFlashing && !isFading)
	{
		fadeColour = colour;
		fadeTime = time > 0 ? time : 0.00001f;
		fadeFlashTimer.restart();
		OnFadeComplete = OnComplete;
		isFlashing = true;
		isFading = false;
		return true;
	}
	return false;
}

sf::View VCamera::GetDefaultView()
{
	sf::View defaultView;
	defaultView.reset(sf::FloatRect(0.0f, 0.0f, view.getSize().x, view.getSize().y));

	return defaultView;
}

void VCamera::Update(float dt)
{
	sf::Vector2f position = Position;
	float rotation = Rotation;
	float zoom = Zoom;

	sf::Vector2f HalfWidth = view.getSize() / 2.0f;

	if (isFollowing)
	{
		sf::Vector2f DeadZoneArea = HalfWidth * deadZone;
		sf::Vector2f FollowPos = followObject->Position + (followObject->Size / 2.0f);

		if (followMethod == LOCKON)
		{
			position = FollowPos;
		}
		else if (followMethod == TOPDOWN)
		{
			if (followLead > 0)
			{
				position = VInterpolate::Vector2f(position, FollowPos + (followObject->Velocity * followLead), 1 - powf(followLerp, dt));
			}
			else if (followLerp > 0)
			{
				position = VInterpolate::Vector2f(position, FollowPos, 1 - powf(followLerp, dt));
			}

			sf::Vector2f relativePos = FollowPos - position;

			if (relativePos.x - (followObject->Size.x/2.0f) < -DeadZoneArea.x)
				relativePos.x = -DeadZoneArea.x + (followObject->Size.x / 2.0f);
			if (relativePos.y - (followObject->Size.y / 2.0f) < -DeadZoneArea.y)
				relativePos.y = -DeadZoneArea.y + (followObject->Size.y / 2.0f);
			if (relativePos.x + (followObject->Size.x / 2.0f) > DeadZoneArea.x)
				relativePos.x = DeadZoneArea.x - (followObject->Size.x / 2.0f);
			if (relativePos.y + (followObject->Size.y / 2.0f) > DeadZoneArea.y)
				relativePos.y = DeadZoneArea.y - (followObject->Size.y / 2.0f);

			position = FollowPos - relativePos;
		}
		else if (followMethod == PLATFORMER)
		{
			if (followLead > 0)
			{
				position += followObject->Velocity * followLead * dt;
			}
			if (followLerp > 0)
			{
				position = VInterpolate::Vector2f(position, FollowPos, 1 - powf(followLerp, dt));
			}

			sf::Vector2f relativePos = FollowPos - position;

			if (relativePos.x - (followObject->Size.x / 2.0f) < -DeadZoneArea.x)
				relativePos.x = -DeadZoneArea.x + (followObject->Size.x / 2.0f);
			if (relativePos.x + (followObject->Size.x / 2.0f) > DeadZoneArea.x)
				relativePos.x = DeadZoneArea.x - (followObject->Size.x / 2.0f);
			if (relativePos.y + (followObject->Size.y / 2.0f) > DeadZoneArea.y)
				relativePos.y = DeadZoneArea.y - (followObject->Size.y / 2.0f);

			position = FollowPos - relativePos;
		}
		else if (followMethod == ROOM || followMethod == ROOMLERP)
		{
			sf::Vector2f relativePos;
			relativePos.x = floorf(FollowPos.x / VGlobal::p()->Width) * VGlobal::p()->Width;
			relativePos.y = floorf(FollowPos.y / VGlobal::p()->Height) * VGlobal::p()->Height;
			relativePos += sf::Vector2f(VGlobal::p()->Width, VGlobal::p()->Height) / 2.0f;

			if (followMethod == ROOM)
				position = relativePos;
			else
				position = VInterpolate::Vector2f(position, relativePos, 1 - powf(followLerp, dt));
		}
	}

	//Constrain Bounderies
	if (boundToWorld)
	{
		sf::FloatRect worldBounds = VGlobal::p()->WorldBounds;
		if (position.x - HalfWidth.x < worldBounds.left)
			position.x = HalfWidth.x;
		if (position.y - HalfWidth.y < worldBounds.top)
			position.y = HalfWidth.y;
		if (position.x + HalfWidth.x > worldBounds.left + worldBounds.width)
			position.x = worldBounds.width - HalfWidth.x;
		if (position.y + HalfWidth.y > worldBounds.top + worldBounds.height)
			position.y = worldBounds.height - HalfWidth.y;
	}

	LastPosition = position;
	Position = position;

	//Shake
	if (isShaking)
	{
		HalfWidth.x *= shakeX;
		HalfWidth.y *= shakeY;
		position += VGlobal::p()->Random->GetVector2f(HalfWidth, -HalfWidth);

		float t = shakeTimer.getElapsedTime().asSeconds();

		if (t > shakeLength)
		{
			if (OnShakeComplete)
				OnShakeComplete();

			isShaking = false;
		}
	}

	if (isFading)
	{
		float t = fadeFlashTimer.getElapsedTime().asSeconds();
		float fadePercentage = t / fadeTime;
		fadePercentage = fadePercentage > 1.0f ? 1.0f : fadePercentage;

		if (fadeIn)
		{
			fadeColour.a = static_cast<unsigned int>(VInterpolate::Float(255, 0, fadePercentage));
		}
		else
		{
			fadeColour.a = static_cast<unsigned int>(VInterpolate::Float(0, 255, fadePercentage));
		}

		if (t > fadeTime)
		{
			if (OnFadeComplete)
				OnFadeComplete();

			isFading = false;
			isFlashing = false;
		}
	}

	if (isFlashing)
	{
		float t = fadeFlashTimer.getElapsedTime().asSeconds();
		float halfFadeTime = fadeTime / 2.0f;

		if (t < halfFadeTime)
		{
			float fadePercentage = t / halfFadeTime;
			fadePercentage = fadePercentage > 1.0f ? 1.0f : fadePercentage;

			fadeColour.a = static_cast<unsigned int>(VInterpolate::Float(0, 255, fadePercentage));
		}
		else
		{
			float fadePercentage = (t - halfFadeTime) / halfFadeTime;
			fadePercentage = fadePercentage > 1.0f ? 1.0f : fadePercentage;

			fadeColour.a = static_cast<unsigned int>(VInterpolate::Float(255, 0, fadePercentage));
		}

		if (t > fadeTime)
		{
			if (OnFlashComplete)
				OnFlashComplete();

			isFlashing = false;
			isFading = false;
		}
	}

	view.setCenter(position);
	view.setRotation(rotation);
	view.setSize(VCameraList::Default.getSize() * zoom);
}

void VCamera::Render(sf::RenderTarget &RenderTarget)
{
	if (isFlashing || isFading)
	{
		sf::View defaultView = RenderTarget.getDefaultView();
		RenderTarget.setView(defaultView);
		block.setPosition(defaultView.getCenter() - (defaultView.getSize() / 2.0f));
		block.setFillColor(fadeColour);
		RenderTarget.draw(block);
	}
}
