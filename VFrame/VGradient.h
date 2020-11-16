/**
* @file    VGradient.h
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
* VObject that generates and displays a gradient texture.
*/

#pragma once
#include <VFrame/VObject.h>
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/Color.hpp>

#include <memory>

///VObject that generates and displays a gradient texture. This is done by generating a pixel-high texture as a gradient line, rendering it to the correct shape and applying it to the texture.
class VGradient : public VObject
{
public:
	///Types of gradients that could be used (uses the types specified in Adobe Photoshop)
	enum VGradientType
	{
		LINEAR,
		RADIAL,
		ANGULAR,
		REFLECTIVE,
		DIAMOND
	};

private:
	struct VColourPoint
	{
		float Position;
		sf::Color Tint;
		VColourPoint(float pos, sf::Color tint)
		{
			Position = pos;
			Tint = tint;
		}
	};

	bool dirty = false;

protected:
	///Render infomation.
	sf::VertexArray vertexArray;
	///The render states object.
	sf::RenderStates renderState;
	///The colours of the gradient.
	std::vector<std::unique_ptr<VColourPoint>> points;
	///The colours of the gradient, sorted for rendering purposes.
	std::vector<VColourPoint*> sortedPoints;
	///The type of gradiant to use.
	VGradientType gradientType;
	///Gradient strip
	sf::Image gradImage;
	///Gradient resolution;
	float gradResolution;
	///Render texture
	sf::RenderTexture renderTex;

	///Update the tranform.
	virtual void updateTransform() override;
	///Update the frame.
	virtual void updateFrame();

public:
	
	///Used to call parent class functions when they are overrided in class.
	typedef VObject VSUPERCLASS;

	///Origin of the 9-slice object in percentage (0.0f - 1.0f) for rotation.
	sf::Vector2f Origin;
	///Colour Tint of the Sprite.
	sf::Color Tint = sf::Color::White;
	///Scale transform.
	sf::Vector2f Scale = sf::Vector2f(1.0f, 1.0f);

	/**
	* @param x X position of the sprite.
	* @param y Y position of the sprite
	* @param width Width of the sprite (also sets the width of the render area).
	* @param height Height of the sprite (also sets the height of the render area).
	* @param type Type of gradient to use.
	* @param resolution Resolution of the generated gradient.
	*/
	VGradient(float x, float y, float width, float height, VGradientType type = VGradientType::LINEAR, float resolution = 1024.0f);

	/**
	* @param position Position of the sprite.
	* @param size Size of the sprite (also sets the size of the render area).
	* @param type Type of gradient to use.
	* @param resolution Resolution of the generated gradient.
	*/
	VGradient(sf::Vector2f position, sf::Vector2f size = sf::Vector2f(), VGradientType type = VGradientType::LINEAR, float resolution = 1024.0f);

	/**
	* Adds a new gradient point to the gradient strip. Points can be added in any order, and the class will sort them in position order on generation.
	* @param position A float between 0.0 and 1.0 to represent where on the initial gradient line should the renderer interpolate to the specified colour.
	* @param colour Tint of the gradient at the specified position on the gradient line.
	*/
	void AddPoint(float position, const sf::Color& colour);

	/**
	* Update an existing gradient point to the gradient strip.
	* @param index The position in the array of colour indexes.
	* @param position A float between 0.0 and 1.0 to represent where on the initial gradient line should the renderer interpolate to the specified colour.
	* @param colour Tint of the gradient at the specified position on the gradient line.
	*/
	void UpdatePoint(unsigned int index, float position, const sf::Color& colour);

	/**
	* Update the tint of an existing gradient point to the gradient strip.
	* @param index The position in the array of colour indexes.
	* @param colour Tint of the gradient at the specified position on the gradient line.
	*/
	void UpdatePointColour(unsigned int index, const sf::Color& colour);

	/**
	* Update the position of an existing gradient point to the gradient strip.
	* @param index The position in the array of colour indexes.
	* @param position A float between 0.0 and 1.0 to represent where on the initial gradient line should the renderer interpolate to the specified colour.
	*/
	void UpdatePointPosition(unsigned int index, float position);

	/**
	* Remove an existing gradient point to the gradient strip.
	* @param index The position in the array of colour indexes.
	*/
	void RemovePoint(unsigned int index);

	///@param type The types of gradients that could be used (uses the types specified in Adobe Photoshop)
	void SetType(VGradientType type);

	///@param resolution Resolution of the generated gradient.
	void SetResolution(float resolution);

	///Destroys the gradient rendering data (and texture if disposable is set for true).
	virtual void Destroy() override;

	/**
	* Renders the gradient object.
	* @param RenderTarget The game's sf::RenderTarget object to render data onto.
	*/
	virtual void Draw(sf::RenderTarget& RenderTarget) override;
};

