#include "VRandom.h"

using std::uniform_int_distribution;
using std::uniform_real_distribution;

float VRandom::GetFloat(float max, float min)
{
	return uniform_real_distribution<float>(min, max)(engine);
}

int VRandom::GetInt(int max, int min)
{
	return uniform_int_distribution<int>(min, max)(engine);
}

bool VRandom::GetBool(float chance)
{
	chance = chance > 100 ? 100 : chance < 0 ? 0 : chance;

	return GetFloat(100.0f) < chance;
}

sf::Color VRandom::GetColor(sf::Color max, sf::Color min)
{
	sf::Color color;
	color.r = GetInt(max.r, min.r);
	color.g = GetInt(max.g, min.g);
	color.b = GetInt(max.b, min.b);
	color.a = 255;
	return color;
}

sf::Vector2f VRandom::GetVector2f(sf::Vector2f max, sf::Vector2f min)
{
	return sf::Vector2f(GetFloat(max.x, min.x), GetFloat(max.y, min.y));
}

sf::Vector2u VRandom::GetVector2u(sf::Vector2u max, sf::Vector2u min)
{
	return sf::Vector2u(GetInt(max.x, min.x), GetInt(max.y, min.y));
}
