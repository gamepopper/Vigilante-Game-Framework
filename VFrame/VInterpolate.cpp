#include "VInterpolate.h"
#include <math.h>

// Easing functions based on equations by Robert Penner
// Ported to C++ by Jesus Gollonet: https://github.com/jesusgollonet/ofpennereasing

//t - current time
//b - beginning
//c - change
//d - destination time

const float PI = 3.14159265f;

float VInterpolate::backIn(float t, float b, float c, float d)
{
	float s = 1.70158f;
	float postFix = t /= d;
	return c*(postFix)*t*((s + 1)*t - s) + b;
}
float VInterpolate::backOut(float t, float b, float c, float d)
{
	float s = 1.70158f;
	return c*((t = t / d - 1)*t*((s + 1)*t + s) + 1) + b;
}

float VInterpolate::backInOut(float t, float b, float c, float d)
{
	float s = 1.70158f;
	if ((t /= d / 2) < 1) return c / 2 * (t*t*(((s *= (1.525f)) + 1)*t - s)) + b;
	float postFix = t -= 2;
	return c / 2 * ((postFix)*t*(((s *= (1.525f)) + 1)*t + s) + 2) + b;
}

float VInterpolate::bounceIn(float t, float b, float c, float d)
{
	return c - bounceOut(d - t, 0, c, d) + b;
}

float VInterpolate::bounceOut(float t, float b, float c, float d)
{
	if ((t /= d) < (1 / 2.75f)) {
		return c*(7.5625f*t*t) + b;
	}
	else if (t < (2 / 2.75f)) {
		float postFix = t -= (1.5f / 2.75f);
		return c*(7.5625f*(postFix)*t + .75f) + b;
	}
	else if (t < (2.5 / 2.75)) {
		float postFix = t -= (2.25f / 2.75f);
		return c*(7.5625f*(postFix)*t + .9375f) + b;
	}
	else {
		float postFix = t -= (2.625f / 2.75f);
		return c*(7.5625f*(postFix)*t + .984375f) + b;
	}
}

float VInterpolate::bounceInOut(float t, float b, float c, float d)
{
	if (t < d / 2) return bounceIn(t * 2, 0, c, d) * .5f + b;
	else return bounceOut(t * 2 - d, 0, c, d) * .5f + c*.5f + b;
}

float VInterpolate::circIn(float t, float b, float c, float d)
{
	return -c * (sqrt(1 - (t /= d)*t) - 1) + b;
}

float VInterpolate::circOut(float t, float b, float c, float d)
{
	return c * sqrt(1 - (t = t / d - 1)*t) + b;
}

float VInterpolate::circInOut(float t, float b, float c, float d)
{
	if ((t /= d / 2) < 1) return -c / 2 * (sqrt(1 - t*t) - 1) + b;
	return c / 2 * (sqrt(1 - t*(t -= 2)) + 1) + b;
}

float VInterpolate::cubicIn(float t, float b, float c, float d)
{
	return c*(t /= d)*t*t + b;
}

float VInterpolate::cubicOut(float t, float b, float c, float d)
{
	return c*((t = t / d - 1)*t*t + 1) + b;
}

float VInterpolate::cubicInOut(float t, float b, float c, float d)
{
	if ((t /= d / 2) < 1) return c / 2 * t*t*t + b;
	return c / 2 * ((t -= 2)*t*t + 2) + b;
}

float VInterpolate::elasticIn(float t, float b, float c, float d)
{
	if (t == 0) return b;  if ((t /= d) == 1) return b + c;
	float p = d*.3f;
	float a = c;
	float s = p / 4;
	float postFix = a*pow(2, 10 * (t -= 1)); // this is a fix, again, with post-increment operators
	return -(postFix * sin((t*d - s)*(2 * PI) / p)) + b;
}

float VInterpolate::elasticOut(float t, float b, float c, float d)
{
	if (t == 0) return b;  if ((t /= d) == 1) return b + c;
	float p = d*.3f;
	float a = c;
	float s = p / 4;
	return (a*pow(2, -10 * t) * sin((t*d - s)*(2 * PI) / p) + c + b);
}

float VInterpolate::elasticInOut(float t, float b, float c, float d)
{
	if (t == 0) return b;  if ((t /= d / 2) == 2) return b + c;
	float p = d*(.3f*1.5f);
	float a = c;
	float s = p / 4;

	if (t < 1) {
		float postFix = a*pow(2, 10 * (t -= 1)); // postIncrement is evil
		return -.5f*(postFix* sin((t*d - s)*(2 * PI) / p)) + b;
	}
	float postFix = a*pow(2, -10 * (t -= 1)); // postIncrement is evil
	return postFix * sin((t*d - s)*(2 * PI) / p)*.5f + c + b;
}

float VInterpolate::expoIn(float t, float b, float c, float d)
{
	return (t == 0) ? b : c * pow(2, 10 * (t / d - 1)) + b;
}

float VInterpolate::expoOut(float t, float b, float c, float d)
{
	return (t == d) ? b + c : c * (-pow(2, -10 * t / d) + 1) + b;
}

float VInterpolate::expoInOut(float t, float b, float c, float d)
{
	if (t == 0) return b;
	if (t == d) return b + c;
	if ((t /= d / 2) < 1) return c / 2 * pow(2, 10 * (t - 1)) + b;
	return c / 2 * (-pow(2, -10 * --t) + 2) + b;
}

float VInterpolate::linear(float t, float b, float c, float d)
{
	return c*t / d + b;
}

float VInterpolate::quadIn(float t, float b, float c, float d)
{
	return c*(t /= d)*t + b;
}

float VInterpolate::quadOut(float t, float b, float c, float d)
{
	return -c *(t /= d)*(t - 2) + b;
}

float VInterpolate::quadInOut(float t, float b, float c, float d)
{
	if ((t /= d / 2) < 1) return ((c / 2)*(t*t)) + b;
	return -c / 2 * (((t - 2)*(--t)) - 1) + b;
	/*
	originally return -c/2 * (((t-2)*(--t)) - 1) + b;

	I've had to swap (--t)*(t-2) due to diffence in behaviour in
	pre-increment operators between java and c++, after hours
	of joy
	*/

}

float VInterpolate::quartIn(float t, float b, float c, float d)
{
	return c*(t /= d)*t*t*t + b;
}

float VInterpolate::quartOut(float t, float b, float c, float d)
{
	return -c * ((t = t / d - 1)*t*t*t - 1) + b;
}

float VInterpolate::quartInOut(float t, float b, float c, float d)
{
	if ((t /= d / 2) < 1) return c / 2 * t*t*t*t + b;
	return -c / 2 * ((t -= 2)*t*t*t - 2) + b;
}

float VInterpolate::quintIn(float t, float b, float c, float d)
{
	return c*(t /= d)*t*t*t*t + b;
}

float VInterpolate::quintOut(float t, float b, float c, float d)
{
	return c*((t = t / d - 1)*t*t*t*t + 1) + b;
}

float VInterpolate::quintInOut(float t, float b, float c, float d)
{
	if ((t /= d / 2) < 1) return c / 2 * t*t*t*t*t + b;
	return c / 2 * ((t -= 2)*t*t*t*t + 2) + b;
}

float VInterpolate::sineIn(float t, float b, float c, float d)
{
	return -c * cos(t / d * (PI / 2)) + c + b;
}

float VInterpolate::sineOut(float t, float b, float c, float d)
{
	return c * sin(t / d * (PI / 2)) + b;
}

float VInterpolate::sineInOut(float t, float b, float c, float d)
{
	return -c / 2 * (cos(PI*t / d) - 1) + b;
}

float VInterpolate::Angle(float a, float b, float t, float d, VInterpolateType type)
{
	float diff = b - a;

	a -= (int)(a / 360.0f);
	b -= (int)(b / 360.0f);

	if (diff > 180)
	{
		a += 360.0f;
	}
	else if (diff < -180)
	{
		a -= 360.0f;
	}

	return Float(a, b, t, d, type);
}

float VInterpolate::Float(float a, float b, float t, float d, VInterpolateType type)
{
	t = t < 0 ? 0 : t;
	t = t > d ? d : t;

	switch (type)
	{
	case InBack:		return backIn(t, a, (b - a), d);
	case OutBack:		return backOut(t, a, (b - a), d);
	case InOutBack:		return backInOut(t, a, (b - a), d);
	case InBounce:		return bounceIn(t, a, (b - a), d);
	case OutBounce:		return bounceOut(t, a, (b - a), d);
	case InOutBounce:	return bounceInOut(t, a, (b - a), d);
	case InCirc:		return circIn(t, a, (b - a), d);
	case OutCirc:		return circOut(t, a, (b - a), d);
	case InOutCirc:		return circInOut(t, a, (b - a), d);
	case InCubic:		return cubicIn(t, a, (b - a), d);
	case OutCubic:		return cubicOut(t, a, (b - a), d);
	case InOutCubic:	return cubicInOut(t, a, (b - a), d);
	case InElastic:		return elasticIn(t, a, (b - a), d);
	case OutElastic:	return elasticOut(t, a, (b - a), d);
	case InOutElastic:	return elasticInOut(t, a, (b - a), d);
	case InExpo:		return expoIn(t, a, (b - a), d);
	case OutExpo:		return expoOut(t, a, (b - a), d);
	case InOutExpo:		return expoInOut(t, a, (b - a), d);
	case Linear:		return linear(t, a, (b - a), d);
	case InQuad:		return quadIn(t, a, (b - a), d);
	case OutQuad:		return quadOut(t, a, (b - a), d);
	case InOutQuad:		return quadInOut(t, a, (b - a), d);
	case InQuart:		return quartIn(t, a, (b - a), d);
	case OutQuart:		return quartOut(t, a, (b - a), d);
	case InOutQuart:	return quartInOut(t, a, (b - a), d);
	case InQuint:		return quintIn(t, a, (b - a), d);
	case OutQuint:		return quintOut(t, a, (b - a), d);
	case InOutQuint:	return quintInOut(t, a, (b - a), d);
	case InSine:		return sineIn(t, a, (b - a), d);
	case OutSine:		return sineOut(t, a, (b - a), d);
	case InOutSine:		return sineInOut(t, a, (b - a), d);
	default:			return linear(t, a, (b - a), d);
	}
}

sf::Color VInterpolate::Color(sf::Color a, sf::Color b, float t, float d, VInterpolateType type)
{
	sf::Color color;
	color.r = sf::Uint8(Float((float)a.r, (float)b.r, t, d, type));
	color.g = sf::Uint8(Float((float)a.g, (float)b.g, t, d, type));
	color.b = sf::Uint8(Float((float)a.b, (float)b.b, t, d, type));
	color.a = sf::Uint8(Float((float)a.a, (float)b.a, t, d, type));

	return color;
}

sf::Vector2f VInterpolate::Vector2f(sf::Vector2f a, sf::Vector2f b, float t, float d, VInterpolateType type)
{
	return sf::Vector2f(Float(a.x, b.x, t, d, type), Float(a.y, b.y, t, d, type));
}

sf::Vector3f VInterpolate::Vector3f(sf::Vector3f a, sf::Vector3f b, float t, float d, VInterpolateType type)
{
	return sf::Vector3f(Float(a.x, b.x, t, d, type), Float(a.y, b.y, t, d, type), Float(a.z, b.z, t, d, type));
}
