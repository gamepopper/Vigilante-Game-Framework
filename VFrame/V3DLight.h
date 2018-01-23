#pragma once
#include <SFML/System/Vector3.hpp>
#include <SFML/Graphics/Color.hpp>

enum class LightType { DIRECTION, POINT, SPOT, COUNT };

#define LIGHT_COUNT 8

struct V3DLight
{
	V3DLight(LightType type, const sf::Vector3f pos = sf::Vector3f(), const sf::Vector3f dir = sf::Vector3f(), const sf::Vector3f color = sf::Vector3f(), float cutOff = 0.0f, float attentuation = 1.0f, float ambientCoefficient = 1.0f)
		: Type(type), Position(pos), Dir(dir), Tint(color), CutOff(cutOff), Attenuation(attentuation), Coefficient(ambientCoefficient)
	{
		Enabled = true;
	}

	sf::Vector3f Position;
	sf::Vector3f Dir;
	sf::Vector3f Tint;
	float CutOff;
	float Attenuation;
	float Coefficient;
	LightType Type;
	bool Enabled;
};

struct V3DMaterial
{
	V3DMaterial(const sf::Vector3f specular = sf::Vector3f(), float shininess = 0.0f) : Specular(specular), Shininess(shininess) {}
	sf::Vector3f Specular;
	float Shininess;
};