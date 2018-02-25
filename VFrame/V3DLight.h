/**
* @file    V3DLight.h
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
* 3D Light Properties.
*/

#pragma once

#ifndef VFRAME_NO_3D
#include <SFML/System/Vector3.hpp>
#include <SFML/Graphics/Color.hpp>

///Types of lights to use.
enum class LightType { DIRECTION, POINT, SPOT, COUNT };
///Maximum amount of lights in the scene. (Modify to allow more lights)
#define LIGHT_COUNT 8

///Light Structure
struct V3DLight
{
	/**
	* @param type Type of Light.
	* @param pos Light Position.
	* @param dir Light Direction.
	* @param color Light Colour.
	* @param cutOff Light Angle Cutoff.
	* @param attentuation Light Attenuation.
	* @param ambientCoefficient Light Coefficient.
	*/
	V3DLight(LightType type, const sf::Vector3f pos = sf::Vector3f(), const sf::Vector3f dir = sf::Vector3f(), const sf::Vector3f color = sf::Vector3f(), float cutOff = 0.0f, float attentuation = 1.0f, float ambientCoefficient = 1.0f)
		: Type(type), Position(pos), Dir(dir), Tint(color), CutOff(cutOff), Attenuation(attentuation), Coefficient(ambientCoefficient)
	{
		Enabled = true;
	}

	///Light Position
	sf::Vector3f Position;
	///Light Direction
	sf::Vector3f Dir;
	///Light Colour
	sf::Vector3f Tint;
	///Light Angle Cutoff (Useful for Spot Lights)
	float CutOff;
	///Light Attenuation.
	float Attenuation;
	///Light Coefficient.
	float Coefficient;
	///Type of Light.
	LightType Type;
	///Light is enabled.
	bool Enabled;
};

///Material Structure.
struct V3DMaterial
{
	/**
	* @param specular Material specular colour.
	* @param shininess Material Shininess.
	*/
	V3DMaterial(const sf::Vector3f specular = sf::Vector3f(), float shininess = 0.0f) : Specular(specular), Shininess(shininess) {}
	///Material Specular colour.
	sf::Vector3f Specular;
	///Material Shininess coefficient.
	float Shininess;
};
#endif