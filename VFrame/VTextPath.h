/**
* @file    VTextPath.h
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
* Text rendering object that displays the text to follow a specified path.
*/

#pragma once
#include "VText.h"

///Special text object that renders text along a specific curve.
class VTextPath : public VText
{
protected:
	virtual void updateTextRender(sf::String text) override;

public:
	///Used to set how the path should curve like.
	enum PathCurveType : unsigned char
	{
		PATH_NONE, ///No path curve, straight lines between points.
		PATH_ANCHOR, ///Path will curve by anchoring to an average of all points.
		PATH_BEZIER, ///Path will curve as a bezier curve. The kind of bezier curve will depend on the amount of points set (if >= 4 then a 4 point, else a 3 point or linear interpolation.
		NUM_PATH_CURVES,
	};

	///Used to call parent class functions when they are overrided in class.
	typedef VText VSUPERCLASS;
	
	/**
	* @param x X position coordinate.
	* @param y Y position coordinate.
	* @param width The width of the text area that will bound the text.
	* @param text The string that the text object will display.
	* @param charSize The font size the text will be displayed at.
	*/
	VTextPath(float x = 0, float y = 0, float width = 0, const sf::String& text = "", int charSize = 8);
	
	/**
	* Updates text transform and rendering info.
	* @param dt Delta Time between the previous and current frame.
	*/
	virtual void Update(float dt) override;

	///@param point Point to add to the path.
	void AddPoint(const sf::Vector2f& point);
	///@param points A list of points to the path.
	void SetPoints(const std::vector<sf::Vector2f>& points);

	/**
	* Update an existing point.
	* @param index The index in the path array for the point you wish to update.
	* @param x The X Coordinate of the new point.
	* @param y The Y Coordinate of the new point.
	*/
	void UpdatePoint(unsigned int index, float x, float y);

	///@param curve The type of curve that the points to follow.
	void SetCurveType(PathCurveType curve) { curveType = curve; dirty = true; }
	///@return The current curve type set.
	PathCurveType GetCurveType() { return curveType; }

	///@param offset The offset that the text should start rendering on the curve (wrapped to 1.0f). Text wraps along path.
	void SetPathOffset(float offset) { pathOffset = offset; dirty = true; }
	///@return Current path offset.
	float GetPathOffset() { return pathOffset; }

private:
	///The path curve type.
	PathCurveType curveType = PATH_NONE;
	///The text path offset.
	float pathOffset = 0.0f;
	///Point lost
	std::vector<sf::Vector2f> pointList;
	///Get a point on the bezier curve using the point list.
	sf::Vector2f GetBezierPoint(float t);
};

