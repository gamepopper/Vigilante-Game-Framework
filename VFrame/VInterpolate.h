#pragma once
#include <SFML\Graphics\Color.hpp>
#include <SFML\System\Vector2.hpp>

class VInterpolate
{
private:
	static float VInterpolate::backIn(float t, float b, float c, float d);
	static float VInterpolate::backOut(float t, float b, float c, float d);
	static float VInterpolate::backInOut(float t, float b, float c, float d);
	static float VInterpolate::bounceIn(float t, float b, float c, float d);
	static float VInterpolate::bounceOut(float t, float b, float c, float d);
	static float VInterpolate::bounceInOut(float t, float b, float c, float d);
	static float VInterpolate::circIn(float t, float b, float c, float d);
	static float VInterpolate::circOut(float t, float b, float c, float d);
	static float VInterpolate::circInOut(float t, float b, float c, float d);
	static float VInterpolate::cubicIn(float t, float b, float c, float d);
	static float VInterpolate::cubicOut(float t, float b, float c, float d);
	static float VInterpolate::cubicInOut(float t, float b, float c, float d);
	static float VInterpolate::elasticIn(float t, float b, float c, float d);
	static float VInterpolate::elasticOut(float t, float b, float c, float d);
	static float VInterpolate::elasticInOut(float t, float b, float c, float d);
	static float VInterpolate::expoIn(float t, float b, float c, float d);
	static float VInterpolate::expoOut(float t, float b, float c, float d);
	static float VInterpolate::expoInOut(float t, float b, float c, float d);
	static float VInterpolate::linear(float t, float b, float c, float d);
	static float VInterpolate::quadIn(float t, float b, float c, float d);
	static float VInterpolate::quadOut(float t, float b, float c, float d);
	static float VInterpolate::quadInOut(float t, float b, float c, float d);
	static float VInterpolate::quartIn(float t, float b, float c, float d);
	static float VInterpolate::quartOut(float t, float b, float c, float d);
	static float VInterpolate::quartInOut(float t, float b, float c, float d);
	static float VInterpolate::quintIn(float t, float b, float c, float d);
	static float VInterpolate::quintOut(float t, float b, float c, float d);
	static float VInterpolate::quintInOut(float t, float b, float c, float d);
	static float VInterpolate::sineIn(float t, float b, float c, float d);
	static float VInterpolate::sineOut(float t, float b, float c, float d);
	static float VInterpolate::sineInOut(float t, float b, float c, float d);

public:
	enum VInterpolateType
	{
		InBack,
		OutBack,
		InOutBack,
		InBounce,
		OutBounce,
		InOutBounce,
		InCirc,
		OutCirc,
		InOutCirc,
		InCubic,
		OutCubic,
		InOutCubic,
		InElastic,
		OutElastic,
		InOutElastic,
		InExpo,
		OutExpo,
		InOutExpo,
		Linear,
		InQuad,
		OutQuad,
		InOutQuad,
		InQuart,
		OutQuart,
		InOutQuart,
		InQuint,
		OutQuint,
		InOutQuint,
		InSine,
		OutSine,
		InOutSine,
		NumInterpolationTypes,
	};

	static float Angle(float a, float b, float t, float d = 1.0f, VInterpolateType type = Linear);
	static float Float(float a, float b, float t, float d = 1.0f, VInterpolateType type = Linear);
	static sf::Color Color(sf::Color a, sf::Color b, float t, float d = 1.0f, VInterpolateType type = Linear);
	static sf::Vector2f Vector2f(sf::Vector2f a, sf::Vector2f b, float t, float d = 1.0f, VInterpolateType type = Linear);
};