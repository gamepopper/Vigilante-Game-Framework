/**
* @file    VBase.h
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
* The game's specific random number generator for generating random numbers for specific data types.
*/

#pragma once
#include <random>
#include <chrono>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Color.hpp>

///A Random Number Generator class that outputs to multiple types of values, uses the Mersenne Twister algoritm.
class VRandom
{
	std::mt19937 engine;

public:
	VRandom()
	{
		Reset();
	}

	///@param seed The game's initial seed.
	VRandom(unsigned int seed)
	{
		Reset(seed);
	}
	~VRandom() = default;

	///Reset RNG, when no initial seed is passed in, the current system time in milliseconds is used as the initial seed.
	void Reset() 
	{
		unsigned int now = static_cast<unsigned int>(
			std::chrono::duration_cast<std::chrono::milliseconds>
			(std::chrono::system_clock::now().time_since_epoch()).count());
		Reset(now); 
	}

	/**
	* Reset RNG seed.
	* @param seed Initial Seed for RNG.
	*/
	void Reset(unsigned int seed) { engine = std::mt19937(seed); }

	/**
	* Get random float.
	* @param max The highest value the random number can generate.
	* @param min The lowest value the random number can generate (must be less than max).
	* @return The resulting generated value.
	*/
	float GetFloat(float max, float min = 0);

	/**
	* Get random signed integer.
	* @param max The highest value the random number can generate.
	* @param min The lowest value the random number can generate (must be less than max).
	* @return The resulting generated value.
	*/
	int GetInt(int max, int min = 0);

	/**
	* Gets a random bool based on a random float between 0 - 100.
	* @param chance When the generated value is less than this parameter, the value returned will be true.
	* @return The resulting generated value.
	*/
	bool GetBool(float chance = 50.0f);

	/**
	* Get random sf::Color.
	* @param max The highest value the random number can generate.
	* @param min The lowest value the random number can generate (must be less than max).
	* @return The resulting generated value.
	*/
	sf::Color GetColor(sf::Color max, sf::Color min);

	/**
	* Get random two-dimensional float.
	* @param max The highest value the random number can generate.
	* @param min The lowest value the random number can generate (must be less than max).
	* @return The resulting generated value.
	*/
	sf::Vector2f GetVector2f(sf::Vector2f max, sf::Vector2f min = sf::Vector2f());

	/**
	* Get random two-dimensional signed integer.
	* @param max The highest value the random number can generate.
	* @param min The lowest value the random number can generate (must be less than max).
	* @return The resulting generated value.
	*/
	sf::Vector2i GetVector2i(sf::Vector2i max, sf::Vector2i min = sf::Vector2i());

	/**
	* Get random two-dimensional unsigned integer.
	* @param max The highest value the random number can generate.
	* @param min The lowest value the random number can generate (must be less than max).
	* @return The resulting generated value.
	*/
	sf::Vector2u GetVector2u(sf::Vector2u max, sf::Vector2u min = sf::Vector2u());
};

