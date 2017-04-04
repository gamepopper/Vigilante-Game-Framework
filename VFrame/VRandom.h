#pragma once
#include <random>
#include <SFML\System\Vector2.hpp>
#include <SFML\Graphics\Color.hpp>

class VRandom
{
	std::default_random_engine engine;

public:
	VRandom()
	{
		std::random_device device;
		engine.seed(device());
	}
	VRandom(unsigned int seed)
	{
		engine.seed(seed);
	}
	~VRandom() = default;

	void Reset() { std::random_device device; Reset(device()); }
	void Reset(unsigned int seed) { engine.seed(seed); }
	void Reset(std::random_device device) { engine.seed(device()); }

	float GetFloat(float max, float min = 0);
	int GetInt(int max, int min = 0);
	bool GetBool(float chance = 50.0f);
	sf::Color GetColor(sf::Color max, sf::Color min);
	sf::Vector2f GetVector2f(sf::Vector2f max, sf::Vector2f min = sf::Vector2f());
	sf::Vector2u GetVector2u(sf::Vector2u max, sf::Vector2u min = sf::Vector2u());
};

