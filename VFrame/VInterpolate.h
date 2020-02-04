/**
* @file    VInterpolate.h
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
* Functions for calculating interpolation and easings using Robert Penner's Easing Functions.
* Implementation based on C++ port by Jesus Gollonet: https://github.com/jesusgollonet/ofpennereasing
*/

#pragma once
#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/System/Vector3.hpp>

///A helper class for calculating interpolation using Robert Penner's Easing functions.
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

	///The many types of easing functions, both intro, outtro and inouttros.
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

	/**
	* Interpolates based on angle in degrees (0 - 360)
	* @param a Start point.
	* @param b Finish point.
	* @param t Current point in time.
	* @param d Total length of time.
	* @param type Easing function.
	*/
	static float Angle(float a, float b, float t, float d = 1.0f, VInterpolateType type = Linear);

	/**
	* Interpolates standard float variables.
	* @param a Start point.
	* @param b Finish point.
	* @param t Current point in time.
	* @param d Total length of time.
	* @param type Easing function.
	*/
	static float Float(float a, float b, float t, float d = 1.0f, VInterpolateType type = Linear);

	/**
	* Interpolates color values using the sf::Color variable.
	* @param a Start point.
	* @param b Finish point.
	* @param t Current point in time.
	* @param d Total length of time.
	* @param type Easing function.
	*/
	static sf::Color Color(sf::Color a, sf::Color b, float t, float d = 1.0f, VInterpolateType type = Linear);

	/**
	* Interpolates two dimensional vector floats using sf::Vector2f.
	* @param a Start point.
	* @param b Finish point.
	* @param t Current point in time.
	* @param d Total length of time.
	* @param type Easing function.
	*/
	static sf::Vector2f Vector2f(sf::Vector2f a, sf::Vector2f b, float t, float d = 1.0f, VInterpolateType type = Linear);

	/**
	* Interpolates three dimensional vector floats variables.
	* @param a Start point.
	* @param b Finish point.
	* @param t Current point in time.
	* @param d Total length of time.
	* @param type Easing function.
	*/
	static sf::Vector3f Vector3f(sf::Vector3f a, sf::Vector3f b, float t, float d = 1.0f, VInterpolateType type = Linear);
};
