#pragma once
#include <random>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Color.hpp>

class VRandom
{
	std::default_random_engine engine;
	std::random_device device;

public:
	VRandom()
	{
		engine.seed(device());
	}
	VRandom(unsigned int seed)
	{
		engine.seed(seed);
	}
	~VRandom() = default;

	//Reset RNG.
	void Reset() { Reset(device()); }
	//Reset RNG seed.
	void Reset(unsigned int seed) { engine.seed(seed); }
	//Reset RNG device.
	void Reset(std::random_device device) { engine.seed(device()); }

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

