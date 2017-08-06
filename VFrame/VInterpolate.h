#pragma once
#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/System/Vector3.hpp>

class VInterpolate
{
private:
	static float backIn(float t = 0.0f, float b = 0.0f, float c = 1.0f, float d = 1.0f);
	static float backOut(float t = 0.0f, float b = 0.0f, float c = 1.0f, float d = 1.0f);
	static float backInOut(float t = 0.0f, float b = 0.0f, float c = 1.0f, float d = 1.0f);
	static float bounceIn(float t = 0.0f, float b = 0.0f, float c = 1.0f, float d = 1.0f);
	static float bounceOut(float t = 0.0f, float b = 0.0f, float c = 1.0f, float d = 1.0f);
	static float bounceInOut(float t = 0.0f, float b = 0.0f, float c = 1.0f, float d = 1.0f);
	static float circIn(float t = 0.0f, float b = 0.0f, float c = 1.0f, float d = 1.0f);
	static float circOut(float t = 0.0f, float b = 0.0f, float c = 1.0f, float d = 1.0f);
	static float circInOut(float t = 0.0f, float b = 0.0f, float c = 1.0f, float d = 1.0f);
	static float cubicIn(float t = 0.0f, float b = 0.0f, float c = 1.0f, float d = 1.0f);
	static float cubicOut(float t = 0.0f, float b = 0.0f, float c = 1.0f, float d = 1.0f);
	static float cubicInOut(float t = 0.0f, float b = 0.0f, float c = 1.0f, float d = 1.0f);
	static float elasticIn(float t = 0.0f, float b = 0.0f, float c = 1.0f, float d = 1.0f);
	static float elasticOut(float t = 0.0f, float b = 0.0f, float c = 1.0f, float d = 1.0f);
	static float elasticInOut(float t = 0.0f, float b = 0.0f, float c = 1.0f, float d = 1.0f);
	static float expoIn(float t = 0.0f, float b = 0.0f, float c = 1.0f, float d = 1.0f);
	static float expoOut(float t = 0.0f, float b = 0.0f, float c = 1.0f, float d = 1.0f);
	static float expoInOut(float t = 0.0f, float b = 0.0f, float c = 1.0f, float d = 1.0f);
	static float linear(float t = 0.0f, float b = 0.0f, float c = 1.0f, float d = 1.0f);
	static float quadIn(float t = 0.0f, float b = 0.0f, float c = 1.0f, float d = 1.0f);
	static float quadOut(float t = 0.0f, float b = 0.0f, float c = 1.0f, float d = 1.0f);
	static float quadInOut(float t = 0.0f, float b = 0.0f, float c = 1.0f, float d = 1.0f);
	static float quartIn(float t = 0.0f, float b = 0.0f, float c = 1.0f, float d = 1.0f);
	static float quartOut(float t = 0.0f, float b = 0.0f, float c = 1.0f, float d = 1.0f);
	static float quartInOut(float t = 0.0f, float b = 0.0f, float c = 1.0f, float d = 1.0f);
	static float quintIn(float t = 0.0f, float b = 0.0f, float c = 1.0f, float d = 1.0f);
	static float quintOut(float t = 0.0f, float b = 0.0f, float c = 1.0f, float d = 1.0f);
	static float quintInOut(float t = 0.0f, float b = 0.0f, float c = 1.0f, float d = 1.0f);
	static float sineIn(float t = 0.0f, float b = 0.0f, float c = 1.0f, float d = 1.0f);
	static float sineOut(float t = 0.0f, float b = 0.0f, float c = 1.0f, float d = 1.0f);
	static float sineInOut(float t = 0.0f, float b = 0.0f, float c = 1.0f, float d = 1.0f);

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

	/*
	Interpolates based on angle in degrees (0 - 360)
	a: Start point.
	b: Finish point.
	t: Current point in time.
	d: Total length of time.
	type: Easing function.
	*/
	static float Angle(float a, float b, float t, float d = 1.0f, VInterpolateType type = Linear);
	/*
	Interpolates standard float variables.
	a: Start point.
	b: Finish point.
	t: Current point in time.
	d: Total length of time.
	type: Easing function.
	*/
	static float Float(float a, float b, float t, float d = 1.0f, VInterpolateType type = Linear);
	/*
	Interpolates colour values.
	a: Start point.
	b: Finish point.
	t: Current point in time.
	d: Total length of time.
	type: Easing function.
	*/
	static sf::Color Color(sf::Color a, sf::Color b, float t, float d = 1.0f, VInterpolateType type = Linear);
	/*
	Interpolates two-dimensional vectors.
	a: Start point.
	b: Finish point.
	t: Current point in time.
	d: Total length of time.
	type: Easing function.
	*/
	static sf::Vector2f Vector2f(sf::Vector2f a, sf::Vector2f b, float t, float d = 1.0f, VInterpolateType type = Linear);
	/*
	Interpolates three-dimensional vectors.
	a: Start point.
	b: Finish point.
	t: Current point in time.
	d: Total length of time.
	type: Easing function.
	*/
	static sf::Vector3f Vector3f(sf::Vector3f a, sf::Vector3f b, float t, float d = 1.0f, VInterpolateType type = Linear);
};
