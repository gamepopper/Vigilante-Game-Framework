/**
* @file    VCamera.h
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
* The 2D Camera used to render a specific area of a VState.
*/

#pragma once
#include "VObject.h"
#include <SFML/Graphics/View.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/System/Clock.hpp>
#include <vector>
#include <functional>

///The method of how the camera shall follow a specific VObject.
enum CameraFollowMethod : unsigned char
{
	///Locks on directly to the VObject with no lerp or lead.
	LOCKON,
	///Locks onto the VObject with lerp and lead, best if a game has a top-down perspective.
	TOPDOWN,
	///Locks onto the VObject with lerp and lead, but with no dead zone restrictions for moving up. Best if a game is a side-scrolling platformer.
	PLATFORMER,
	///Fixes to positions like a grid. best used for grid based levels where the screen size is evenly divisible to the world size.
	ROOM,
	///Same as Room, however instead of instantly changing positions, the camera will lerp to the next position.
	ROOMLERP,
};

///The 2D camera that renders the scene.
class VCamera
{
protected:
	///The current view the Camera applies to the render target.
	sf::View view;
	///Used to render a fade in or out.
	sf::RectangleShape block;
	///If true, the camera will follow a specific VObject.
	bool isFollowing = false;
	///The VObject that the camera will follow. Must not be nullptr if set to follow.
	VObject* followObject = nullptr;
	///The camera's current follow method.
	CameraFollowMethod followMethod = LOCKON;
	///The amount the camera should lead in front of the followed VObject.
	float followLead = 0;
	///The amount the camera should lerp towards the followed object. Can cause the camera to fall behind the VObject.
	float followLerp = 0;
	///The deadzone area as a decimal percentage (range 0.0 - 1.0) of what area the camera cannot surpass relative to the followed VObject.
	float deadZone = 0.5f;
	///If true, the camera is currently shaking.
	bool isShaking = false;
	///The maximum offset in the X axis from the current position the camera should shake from.
	float shakeX = 0;
	///The maximum offset in the Y axis from the current position the camera should shake from.
	float shakeY = 0;
	///The amount of time the camera should shake for.
	float shakeLength = 0;
	///The timer for camera shaking.
	sf::Clock shakeTimer;
	///The callback function used when the camera finishes shaking.
	std::function<void()> OnShakeComplete = nullptr;
	///The timer for fading and flashing.
	sf::Clock fadeFlashTimer;
	///If true, the camera is currently fading.
	bool isFading = false;
	///If the camera is fading, this bool determines whether it's a fade in (fades from a colour) or a fade out (fades to a colour).
	bool fadeIn = false; 
	///Time to fade.
	float fadeTime = 0.0f;
	///Colour to fade to/from.
	sf::Color fadeColour = sf::Color::Black;
	///The callback function used when the camera finishes fading.
	std::function<void()> OnFadeComplete = nullptr;
	///If true, the camera is currently flashing.
	bool isFlashing = false;
	///The callback function used when the camera finishes flashing.
	std::function<void()> OnFlashComplete = nullptr; 
	///If true, the camera shall be bound to the WorldBounds set in VGlobal.
	bool boundToWorld = true;
	///The Position in the last frame.
	sf::Vector2f LastPosition; 

public:
	///The current position of the camera.
	sf::Vector2f Position;
	///The current angle of the camera.
	float Rotation = 0.0f;
	///The current amount of zoom (default 1.0f) for the camera.
	float Zoom = 1.0f;
	///The camera is only used for rendering if Active is set to true.
	bool Active = true;

	VCamera()
	{
		Reset();
	}

	/**
	* @param View the sf::View that the Camera should use instead of the default one.
	*/
	VCamera(sf::View& View) : view(View)
	{
		
	}

	virtual ~VCamera()
	{
		followObject = nullptr;
		OnShakeComplete = nullptr;
		OnFadeComplete = nullptr;
		OnFlashComplete = nullptr;
	}

	/**
	* Set the camera to follow a specific VObject.
	* @param target The object the camera must follow.
	* @param deadzone The dead zone area the camera should be restricted to.
	* @param followmethod The follow method that the camera should use.
	* @param lead The amount the camera should lead in front of the target.
	* @param lerp The amount the camera should lerp towards the target.
	*/
	void Follow(VObject* target, float deadzone = 0.5f, CameraFollowMethod followmethod = LOCKON, float lead = 0.0f, float lerp = 0.0f);

	///Stop following a game object.
	void StopFollow();

	/**
	* Shake the camera.
	* @param intensity The amount as a decimal percentage (multiplied by the camera's width and height) to shake by.
	* @param time The length of time for the camera to shake for.
	* @param OnComplete A callback to call when the camera finishes shaking.
	*/
	bool Shake(float intensity, float time = 1.0f, std::function<void()> OnComplete = nullptr);

	/**
	* Shake the camera.
	* @param intensity The amount as a decimal percentage (multiplied by the camera's width and height) to shake by.
	* @param time The length of time for the camera to shake for.
	* @param OnComplete A callback to call when the camera finishes shaking.
	*/
	bool Shake(sf::Vector2f intensity, float time = 1.0f, std::function<void()> OnComplete = nullptr);

	/**
	* Fade the screen the camera renders.
	* @param colour The colour the camera should fade from/to.
	* @param time The length of time for the camera to fade.
	* @param fadein If true, the camera will fade from the colour to the screen. If false it will do the reverse, as if to fade out from the screen.
	* @param OnComplete A callback to call when the camera finishes fading.
	*/
	bool Fade(sf::Color colour = sf::Color::Black, float time = 1.0f, bool fadein = false, std::function<void()> OnComplete = nullptr);

	/**
	* Flash the screen the camera renders.
	* @param colour The colour the camera should flash to and from.
	* @param time The total length of time for the camera to flash.
	* @param OnComplete A callback to call when the camera finishes flashing.
	*/
	bool Flash(sf::Color colour = sf::Color::Black, float time = 1.0f, std::function<void()> OnComplete = nullptr);
	
	/**
	* @param bound If true, the game will bound to the WorldBounds set in VGlobal.
	*/
	void SetToBounds(bool bound)
	{
		boundToWorld = bound;
	}

	/**
	* @param viewport sets the camera's view to display from a specific section of the viewport (default is FloatRect(0, 0, 1, 1)).
	*/
	void SetViewport(sf::FloatRect &viewport)
	{
		view.setViewport(viewport);
	}

	/**
	* Updates the camera's position, rotation and zoom.
	* @param dt Delta Time between the current and previous frame in the game.
	*/
	void Update(float dt);

	/**
	* Draws Camera for Flashing and Fading.
	* @param RenderTarget the game's sf::RenderTarget object to render data onto.
	*/
	void Render(sf::RenderTarget &RenderTarget);

	/**
	* Resets camera properties to default.
	*/
	void Reset();
 
	/// @return True if Fading.
	bool IsFading()		{ return isFading;		}
	/// @return True if Flashing.
	bool IsFlashing()	{ return isFlashing;	}
	/// @return True if Shaking.
	bool IsShaking()	{ return isShaking;		}

	/// @return The camera's current sf::View.
	sf::View const& GetView() { return view; }

	/// @return The camera's current sf::View with default parameters.
	sf::View GetDefaultView();
};

///A basic class to access the default view.
class VCameraList
{
public:
	///Default view, do not modify as this can be used to reset the Camera list to the default single camera.
	static sf::View Default;
};

