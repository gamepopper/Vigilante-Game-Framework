/**
* @file    VShape.h
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
* Class for rendering primitive shapes.
*/

#pragma once
#include "VObject.h"
#include <SFML/Graphics/Shape.hpp>
#include <SFML/System/String.hpp>
#include <memory>

///An object class that renders primitive shapes. Arguably more optimal than creating a VSprite and calling any of the Make graphic functions.
class VShape : public VObject
{
protected:
	///The shape object that holds the rendering and transform infomation of the object.
	std::unique_ptr<sf::Shape> shape;
	sf::Texture* tex;
	bool disposable = false;
	virtual void updateTransform() override;

public:
	///Origin point for rotation.
	sf::Vector2f Origin;
	///Scale transform.
	sf::Vector2f Scale;
	///RenderState for the shape.
	sf::RenderStates RenderState;

	///Used to call parent class functions when they are overrided in class.
	typedef VObject VSUPERCLASS;

	/**
	* Constructor creates a rectangle shape by default.
	* @param x X position of the shape.
	* @param y Y position of the shape.
	* @param width Width of the shape.
	* @param height Height of the shape.
	*/
	VShape(float x = 0, float y = 0, float width = 0, float height = 0);

	/**
	* Constructor creates a rectangle shape by default.
	* @param Position Position of the shape.
	* @param Size Size of the shape.
	*/
	VShape(sf::Vector2f Position, sf::Vector2f Size);

	virtual void Destroy() override;

	/**
	* Set the shape to a rectangle.
	* @param width Width of the new rectangle.
	* @param height Height of the new rectangle.
	*/
	void SetRectangle(float width, float height);

	/**
	* Set the shape to a circle.
	* @param radius Radius of the new circle.
	* @param pointCount The amount of points in the shape (because sf::Shape cannot render perfect circles, only a series of points that can make up a circle).
	*/
	void SetCircle(float radius, unsigned int pointCount = 30);

	/**
	* Set the shape to a convex shape.
	* @param points A list of points that will make up the shape.
	*/
	void SetConvex(std::vector<sf::Vector2f>& points);

	/**
	* Set the shape to a custom object.
	* @param shape Custom object that overrides sf::Shape.
	* @param width Width of the new rectangle.
	* @param height Height of the new rectangle.
	*/
	void SetCustom(sf::Shape* shape, float width, float height);

	///@return Return the pointer to the shape object.
	sf::Shape* GetShape();

	///@param tint The colour that will fill the shape.
	void SetFillTint(const sf::Color& tint);
	///@param tint The colour that will outline the shape.
	void SetOutlineTint(const sf::Color& tint);
	///@param thickness The thickness of the outline in pixels.
	void SetOutlineThickness(float thickness);
	///@return The colour of the shape's fill colour.
	const sf::Color& GetFillTint();
	///@return The colour of the shape's outline colour.
	const sf::Color& GetOutlineTint();
	///@return The thickness of the outline in thickness.
	float GetOutlineThickness();
	///@param texture The texture you want to set to, gets destroyed once removed. NULL by default.
	void SetTexture(sf::Texture* texture);
	///@param filename The file of the texture you want to set to.
	void SetTextureFromFile(const sf::String& filename);
	///@param rect The area of the texture you want to be displayed on the shape, by default it's the full area.
	void SetTextureRect(const sf::IntRect& rect);

	/**
	* Renders the shape.
	* @param RenderTarget The game's sf::RenderTarget object to render data onto.
	*/
	virtual void Draw(sf::RenderTarget& RenderTarget) override;
};

