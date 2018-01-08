#pragma once
#include <random>
#include <chrono>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Color.hpp>

class VRandom
{
	std::mt19937 engine;

public:
	VRandom()
	{
		Reset();
	}
	VRandom(unsigned int seed)
	{
		Reset(seed);
	}
	~VRandom() = default;

	//Reset RNG.
	void Reset() 
	{
		unsigned int now = static_cast<unsigned int>(
			std::chrono::duration_cast<std::chrono::milliseconds>
			(std::chrono::system_clock::now().time_since_epoch()).count());
		Reset(now); 
	}
	//Reset RNG seed.
	void Reset(unsigned int seed) { engine = std::mt19937(seed); }

	//Get random float.
	float GetFloat(float max, float min = 0);
	//Get random integer.
	int GetInt(int max, int min = 0);
	//Get random boolean with chance.
	bool GetBool(float chance = 50.0f);
	//Get random colour.
	sf::Color GetColor(sf::Color max, sf::Color min);
	//Get Random two-dimensional float.
	sf::Vector2f GetVector2f(sf::Vector2f max, sf::Vector2f min = sf::Vector2f());
	//Get Random two-dimensional integer.
	sf::Vector2i GetVector2i(sf::Vector2i max, sf::Vector2i min = sf::Vector2i());
	//Get Random two-dimensional unsigned integer.
	sf::Vector2u GetVector2u(sf::Vector2u max, sf::Vector2u min = sf::Vector2u());
};

