/**
* @file    VParticle.h
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
* The base particle class used by the VEmitter for particle systems. This is not a VSprite as rendering is done in the VEmitter.
*/

#pragma once
#include "VObject.h"
#include "VRange.h"

///A base particle class for use with VEmitter. This does not override VSprite as it does not handle rendering for particle systems.
class VParticle : public VObject
{
public:
	///Used to call parent class functions when they are overrided in class.
	typedef VObject VSUPERCLASS;
	///The length of time the particle is active.
	float Lifespan;
	///The current amount of time the particle is active for.
	float Age;
	///The current tint of the particle.
	sf::Color Tint;
	///The current scale of the particle.
	sf::Vector2f Scale;
	///The scale range that is set from VEmitter.
	VRange<sf::Vector2f> ScaleRange;
	///The colour tint range that is set from VEmitter.
	VRange<sf::Color> ColourRange;
	///The alpha range that is set from VEmitter.
	VRange<float> AlphaRange;

	VParticle() : VObject()
	{
		Lifespan = 0;
		Age = 0;
		exists = false;

#if _DEBUG
		DebugColor = sf::Color::Yellow;
#endif
	}

	///@param src The source particle to copy.
	VParticle(const VParticle& src)
	{
		*this = src;
		VBase();
	}

	/**
	* Updates the particle, such as the scale, colour tint and alpha.
	* @param dt Delta Time between the previous and current frame.
	*/
	virtual void Update(float dt) override;

	/**
	* Resets the particle to a specific position.
	* @param x The X Position.
	* @param y The Y Position.
	*/
	virtual void Reset(float x, float y) override;
	
	///Particle specific behaviour when emitted. (Nothing by default).
	virtual void OnEmit() {}
};

