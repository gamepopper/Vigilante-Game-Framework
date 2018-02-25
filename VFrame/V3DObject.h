/**
* @file    V3DObject.h
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
* Similar to VObject except for 3D objects.
*/

#pragma once
#ifndef VFRAME_NO_3D
#include "VBase.h"

#include <SFML/System/Vector3.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

class V3DShader;
class V3DCamera;
///Base 3D Object.
class V3DObject : public VBase
{
protected:
	/**
	* Updates the position and rotation of object.
	* By default all velocity values will be set using the computeVelocity function + velocity * delta time.
	* @param dt Delta Time between the previous and current frame.
	*/
	virtual void updateMotion(float dt);
	///Update the tranform.
	virtual void updateTransform() {};

public:
	///Used to call parent class functions when they are overrided in class.
	typedef VBase VSUPERCLASS;
	///Position Transform.
	sf::Vector3f Position;
	///Rotation Transform.
	sf::Vector3f Rotation;
	///Last Position.
	sf::Vector3f Last;
	///Velocity of Rotation.
	sf::Vector3f AngleVelocity;
	///Acceleration of Rotation.
	sf::Vector3f AngleAcceleration;
	///Drag of Rotation.
	sf::Vector3f AngleDrag;
	///Max Velocity of Rotation.
	sf::Vector3f AngleMax;
	///If true, position and rotation transforms will be updated.
	bool Moves = true;
	///Velocity of Movement.
	sf::Vector3f Velocity;
	///Acceleration of Movement.
	sf::Vector3f Acceleration;
	///Drag of Movement.
	sf::Vector3f Drag;
	///Max Velocity of Movement.
	sf::Vector3f MaxVelocity;
	///Standard health value (object dies if health is <= 0)
	float Health = 100;
	
	/**
	* @param position Position of the object.
	* @param rotation Angle of the object.
	*/
	V3DObject(sf::Vector3f position, sf::Vector3f rotation) : VBase()
	{
		Position = position;
		Rotation = rotation;
		MaxVelocity = sf::Vector3f(10000, 10000, 10000);
		AngleMax = sf::Vector3f(10000, 10000, 10000);
		type = VType::OBJECT;
	}

	/**
	* @param posX X position of the object.
	* @param posY Y position of the object.
	* @param posZ Z position of the object.
	* @param rotX X angle of the object.
	* @param rotY Y angle of the object.
	* @param rotZ Z angle of the object.
	*/
	V3DObject(float posX = 0, float posY = 0, float posZ = 0, 
			float rotX = 0, float rotY = 0, float rotZ = 0) : VBase()
	{
		Position = sf::Vector3f(posX, posY, posZ);
		Rotation = sf::Vector3f(rotX, rotY, rotZ);
		MaxVelocity = sf::Vector3f(10000, 10000, 10000);
		AngleMax = sf::Vector3f(10000, 10000, 10000);
		type = VType::OBJECT;
	}

	/**
	* Resets object
	* @param newPos Sets Position with a new sf::Vector3f.
	*/
	virtual void Reset(sf::Vector3f newPos);

	/**
	* Resets object
	* @param x Sets Position.x.
	* @param y Sets Position.y.
	* @param z Sets Position.z.
	*/
	virtual void Reset(float x, float y, float z);

	/**
	* Reduces Health and Kills Object if Health is <= 0.
	* @param damage The value to subtract from Healh.
	*/
	virtual void Hurt(float damage);

	/**
	* Updates the shader with the current camera and object specific values.
	* @param shader The scene shader.
	* @param camera The scene camera.
	*/
	virtual void UpdateShader(V3DShader* shader, V3DCamera* camera);
	
	/**
	* Updates 3D object and any data it contains.
	* @param dt Delta Time between the previous and current frame.
	*/
	virtual void Update(float dt) override;

	/**
	* Draws 3D object.
	* @param RenderTarget The game's sf::RenderTarget object to render data onto.
	*/
	virtual void Draw(sf::RenderTarget &RenderTarget) override;
};
#endif