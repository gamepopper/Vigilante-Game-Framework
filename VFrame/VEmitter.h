/**
* @file    VEmitter.h
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
* A special VGroup that is made to handle only VParticle objects, used for Particle Effects.
*/

#pragma once
#include "VGroup.h"
#include "VParticle.h"

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/System/String.hpp>
#include <memory>

///A particle effects system that overrides VGroup.
class VEmitter : public VGroup
{
protected:
	///All particles are rendered from a single set of vertices rendering a Quad.
	sf::VertexArray vertices;
	///If true, texture will be destroyed within VSprite. Important if texture isn't loaded from VContent.
	bool disposible = false;
	///Tint for all particles, particularly used for creating particles with the MakeParticles function.
	sf::Color mainTint;
	///Allows the particles processed while running if true.
	bool running = false;
	///Particles will be killed when the timer exceeds the emitter's timespan.
	bool willKill = false;
	///Timer that increases from start.
	float timer = 0;
	///Counts the amount of particles currently emitted before the emitter shall stop.
	int counter = 0;
	///Amount of particles to emit from start.
	int amount = 0;

	/**
	* Sets up the texture the emitter will use when displaying textures.
	* @param Amount Total amount of particles to set up.
	* @param Animated Use the texel animation system to determine which section of the frames to use, or use the whole texture.
	* @param Width Width of the texture area to use.
	* @param Height Height of the texture area to use.
	* @param Rect Area of the texture to use (default is the full texture size).
	* @param RandomFrames Should the emitter use frames incrementally or random frames.
	*/
	void setSize(int Amount, bool Animated, int Width, int Height, const sf::IntRect& Rect = sf::IntRect(), bool RandomFrames = true);

public:
	///Used to call parent class functions when they are overrided in class.
	typedef VGroup VSUPERCLASS;

	///The delay between particles emitting.
	float Frequency = 0.5f;
	///The amount of particles to activate per emit, in case you want more than one particle to appear at once.
	unsigned int AmountPerEmit = 1;
	///If true, the emitter will emit all the particles at once, instead of at the rate of Frequency.
	bool Explosion = false;
	///If true, particles will be emitted at a normalized velocity (set with SpeedRange) or a random angle. If false, particles will be emitted with a randomized velocity (set with VelocityRange).
	bool Circular = false;
	///If true, the emitter will emit particles continuously. If false, particles will emit until the number of particles emitted is greater than or equal to the amount parameter passed in (or the max set of amount).
	bool Constant = false;
	///Position to emit particles from.
	sf::Vector2f Position;
	///Range relative to the position which to emit particles from (-Size/2 to Size/2)/
	sf::Vector2f Size;
	///If true, particles should move.
	bool Moves = true;
	///If true, when particles have collided, their velocity or position cannot be effected.
	bool Immovable = false;
	///Which sides are allowed to be collided with.
	int AllowCollisions = VObject::TOUCHALL;
	///The amount the object will appear to move when the camera scrolls (1.0f = Scrolls as fast as the camera, 0.5f = Scrolls half as fast as the camera). Good for parallax scrolling.
	sf::Vector2f ScrollFactor;
	///The amount the object will appear to rotate when the camera rotates (1.0f = Rotates as fast as the camera, 0.5f = Rotates half as fast as the camera). Good for parallax scrolling.
	float RotateFactor = 1;
	///The amount the object will appear to scale when the camera zooms (1.0f = Scales as fast as the camera, 0.5f = Scales half as fast as the camera). Good for parallax scrolling.
	float ZoomFactor = 1;

	///The amount of time a particle is active for.
	VRange<float> Lifespan;
	///The range of velocity the particle will be when emitted (used when Circular == false).
	VRange<sf::Vector2f> VelocityRange;
	///The direction the particle will move in when emitted (used when Circular == true).
	VRange<float> EmittingAngle;
	///The speed of the particle when emitted (used when Circular == true).
	VRange<float> SpeedRange;
	///The velocity of rotation of the particle.
	VRange<float> AngleVelocityRange;
	///The acceleration of rotation of the particle.
	VRange<float> AngleAccelerationRange;
	///The drag of rotation of the particle.
	VRange<float> AngleDragRange;
	///The angle the particle will initially emit at.
	VRange<float> AngleRange;
	///The amount of drag movement of the particle.
	VRange<sf::Vector2f> DragRange;
	///The amount of acceleration of the particle.
	VRange<sf::Vector2f> AccelerationRange;
	///The bounciness of a particle.
	VRange<float> ElasticityRange;
	///The scale of the particle, interpolates using LifeSpan as the total length of time.
	VRangeBounds<sf::Vector2f> ScaleRange;
	///The colour tint of the particle, interpolates using LifeSpan as the total length of time.
	VRangeBounds<sf::Color> ColourRange;
	///The alpha transparency of the particle, interpolates using LifeSpan as the total length of time.
	VRangeBounds<float> AlphaRange;

	///The main instance of all particles in the emitter, can be overwritten to a class that inherits from VParticle, but will only be used when particles are loaded or created.
	VParticle* ParticleInstance = new VParticle();

	///The RenderState that applies to all particles in the emitter.
	sf::RenderStates RenderState = sf::RenderStates::Default;

	/**
	* @param x The X Position of the emitter.
	* @param y The Y Position of the emitter.
	* @param MaxSize The total size of the group that holds the particle data.
	*/
	VEmitter(float x, float y, unsigned int MaxSize = 0) :	VGroup(MaxSize),
															Position(x, y),
															EmittingAngle(-180, 180),
															Lifespan(3),
															VelocityRange(sf::Vector2f(-100, -100), sf::Vector2f(100, 100)),
															SpeedRange(0, 100),
															AngleVelocityRange(0, 0),
															AngleAccelerationRange(0, 0),
															AngleDragRange(0, 0),
															AngleRange(0, 0),
															DragRange(sf::Vector2f(), sf::Vector2f()),
															AccelerationRange(sf::Vector2f(), sf::Vector2f()),
															ElasticityRange(0, 0),
															ScaleRange(sf::Vector2f(1, 1), sf::Vector2f(1, 1)),
															ColourRange(sf::Color::White, sf::Color::White),
															AlphaRange(1, 1)
	{
		exists = false;
		vertices.setPrimitiveType(sf::PrimitiveType::Quads);
		mainTint = sf::Color::White;
	}

	/**
	* @param position The Position of the emitter.
	* @param MaxSize The total size of the group that holds the particle data.
	*/
	VEmitter(sf::Vector2f position, unsigned int MaxSize = 0) : 	VGroup(MaxSize),
																	Position(position),
																	EmittingAngle(-180, 180),
																	Lifespan(3),
																	VelocityRange(sf::Vector2f(-100, -100), sf::Vector2f(100, 100)),
																	SpeedRange(0, 100),
																	AngleVelocityRange(0, 0),
																	AngleAccelerationRange(0, 0),
																	AngleDragRange(0, 0),
																	AngleRange(0, 0),
																	DragRange(sf::Vector2f(), sf::Vector2f()),
																	AccelerationRange(sf::Vector2f(), sf::Vector2f()),
																	ElasticityRange(0, 0),
																	ScaleRange(sf::Vector2f(1, 1), sf::Vector2f(1, 1)),
																	ColourRange(sf::Color::White, sf::Color::White),
																	AlphaRange(1, 1)
	{
		vertices.setPrimitiveType(sf::PrimitiveType::Quads);
		exists = false;
		mainTint = sf::Color::White;
	}

	/**
	* Load set amount of particles from a file.
	* @param Amount The amount of particles will be loaded using this function. This can be less than or equal to the MaxSize if specified in the constructor.
	* @param Filename The filename and path of the texture used to load the particle. It will be loaded from VContent in VGlobal so it will also store the texture in this content manager.
	* @param Animated Whether the emitter should use the entire texture as one particle, or each particle should be a frame (texel) of that texture.
	* @param Width The frame width and width of the particle.
	* @param Height The frame height and height of the particle.
	* @param Rect The rectangle area of the texture for the emitter to use. By default this is the entire texture.
	* @param RandomFrames Whether the emitter should set frames in order from first to last, or random frames.
	* @return The VEmitter that is being loaded, typically the one that the function is being called from.
	*/
	VEmitter* LoadParticlesFromFile(int Amount, sf::String Filename, bool Animated = false, int Width = 1, int Height = 1, const sf::IntRect& Rect = sf::IntRect(), bool RandomFrames = true);
	
	/**
	* Load set amount of particles from an sf::Texture object.
	* @param Amount The amount of particles will be loaded using this function. This can be less than or equal to the MaxSize if specified in the constructor.
	* @param Texture The sf::Texture object for the texture.
	* @param Animated Whether the emitter should use the entire texture as one particle, or each particle should be a frame (texel) of that texture.
	* @param Width The frame width and width of the particle.
	* @param Height The frame height and height of the particle.
	* @param Rect The rectangle area of the texture for the emitter to use. By default this is the entire texture.
	* @param RandomFrames Whether the emitter should set frames in order from first to last, or random frames.
	* @return The VEmitter that is being loaded, typically the one that the function is being called from.
	*/
	VEmitter* LoadParticles(int Amount, sf::Texture& Texture, bool Animated = false, int Width = 1, int Height = 1, const sf::IntRect& Rect = sf::IntRect(), bool RandomFrames = true);

	/**
	* Makes a set of particles as a series of solid rectangles.
	* @param Amount The amount of particles will be loaded using this function. This can be less than or equal to the MaxSize if specified in the constructor.
	* @param Width The width of the particle.
	* @param Height The height of the particle.
	* @param Color The colour of the rectangle of the particle. Note that this colour will be mutliplied with the tint set in ColourRange.
	* @return The VEmitter that is being loaded, typically the one that the function is being called from.
	*/
	VEmitter* MakeParticles(int Amount, int Width, int Height, sf::Color Color = sf::Color::White);
	
	///Kills the emitter and all of its particles.
	virtual void Kill() override;
	
	///Deletes and removes all particles before destroying any data stored in the emitter.
	virtual void Destroy() override;

	/**
	* Updates all the particles in the emitter.
	* @param dt Delta Time between the current and previous frames of the game.
	*/
	virtual void Update(float dt) override;

	/**
	* Draws all active particles in the scene.
	* @param RenderTarget The game's sf::RenderTarget object to render data onto.
	*/
	virtual void Draw(sf::RenderTarget& RenderTarget) override;

	/**
	* Start emitting particles.
	* @param Amount The amount of particles to emit. Default is all particles available in the emitter.
	*/
	VEmitter* Start(int Amount = 0);

	///Emits a single particle. This can be called directly if you don't want to emit a bunch of particles in sequence or as a special pattern.
	virtual void EmitParticle();
};

