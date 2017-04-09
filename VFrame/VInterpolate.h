#pragma once
#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Vector2.hpp>

class VInterpolate
{
private:
	static float backIn(float t, float b, float c, float d);
	static float backOut(float t, float b, float c, float d);
	static float backInOut(float t, float b, float c, float d);
	static float bounceIn(float t, float b, float c, float d);
	static float bounceOut(float t, float b, float c, float d);
	static float bounceInOut(float t, float b, float c, float d);
	static float circIn(float t, float b, float c, float d);
	static float circOut(float t, float b, float c, float d);
	static float circInOut(float t, float b, float c, float d);
	static float cubicIn(float t, float b, float c, float d);
	static float cubicOut(float t, float b, float c, float d);
	static float cubicInOut(float t, float b, float c, float d);
	static float elasticIn(float t, float b, float c, float d);
	static float elasticOut(float t, float b, float c, float d);
	static float elasticInOut(float t, float b, float c, float d);
	static float expoIn(float t, float b, float c, float d);
	static float expoOut(float t, float b, float c, float d);
	static float expoInOut(float t, float b, float c, float d);
	static float linear(float t, float b, float c, float d);
	static float quadIn(float t, float b, float c, float d);
	static float quadOut(float t, float b, float c, float d);
	static float quadInOut(float t, float b, float c, float d);
	static float quartIn(float t, float b, float c, float d);
	static float quartOut(float t, float b, float c, float d);
	static float quartInOut(float t, float b, float c, float d);
	static float quintIn(float t, float b, float c, float d);
	static float quintOut(float t, float b, float c, float d);
	static float quintInOut(float t, float b, float c, float d);
	static float sineIn(float t, float b, float c, float d);
	static float sineOut(float t, float b, float c, float d);
	static float sineInOut(float t, float b, float c, float d);

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