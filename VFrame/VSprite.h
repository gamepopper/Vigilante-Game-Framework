#pragma once
#include "VObject.h"
#include "VAnimationManager.h"

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Color.hpp>
#include <memory>

class VSprite :	public VObject
{
protected:
	sf::Texture* texture = nullptr;
	bool disposible = false;
	sf::Sprite sprite;

	void setSize(unsigned int texWidth, unsigned int texHeight, bool animated = false, int width = 0, int height = 0, int offsetX = 0, int offsetY = 0);
	virtual void updateTransform() override;
	virtual void updateFrame();

public:
	typedef VObject VSUPERCLASS;
	sf::RenderStates RenderState = sf::RenderStates::Default;
	sf::Color Tint = sf::Color::White;
	sf::Vector2f Origin;
	sf::Vector2f Offset;
	sf::Vector2f Scale;
	sf::Vector2u FrameSize;
	bool FlipX = false, FlipY = false;
	VAnimationManager Animation;

	VSprite(sf::Vector2f Position, const sf::String& Filename = "") : VObject(Position)
	{
		if (Filename != "")
		{
			LoadGraphic(Filename);
		}

		RenderState = sf::RenderStates::Default;
		Tint = sf::Color::White;
		Scale = sf::Vector2f(1, 1);
		Offset = sf::Vector2f();

#if _DEBUG
		DebugColor = sf::Color(0, 255, 0, 128);
#endif
	}

	VSprite(float x = 0, float y = 0, const sf::String& filename = "") : VObject(x,y)
	{
		if (filename != "")
		{
			LoadGraphic(filename);
		}

		RenderState = sf::RenderStates::Default;
		Tint = sf::Color::White;
		Scale = sf::Vector2f(1, 1);
		Offset = sf::Vector2f();

#if _DEBUG
		DebugColor = sf::Color(0, 255, 0, 128);
#endif
	}

	/*
	Load Graphic from filepath.
	filename: Path of texture file.
	animated: Sets if the graphic is part of an animation or not.
	width: Width of sprite, ignored if animated is false.
	height: Height of sprite, ignored if animated is false.
	offsetX: X coordinate to start animation frames (if you don't want to use the entire texture), ignored if animated is false.
	offsetY: Y coordinate to start animation frames (if you don't want to use the entire texture), ignored if animated is false.
	texWidth: Width of Texture Area (if you don't want to use the entire texture), ignored if animated is false, defaults to Texture's full width.
	texWidth: Height of Texture Area (if you don't want to use the entire texture), ignored if animated is false, defaults to Texture's full height.
	*/
	virtual VSprite* LoadGraphic(sf::String filename, bool animated = false, int width = 0, int height = 0, int offsetX = 0, int offsetY = 0, int texWidth = 0, int texHeight = 0);
	/*
	Load Graphic from sf::Texture object.
	texture: Texture object.
	animated: Sets if the graphic is part of an animation or not.
	width: Width of sprite, ignored if animated is false.
	height: Height of sprite, ignored if animated is false.
	offsetX: X coordinate to start animation frames (if you don't want to use the entire texture), ignored if animated is false.
	offsetY: Y coordinate to start animation frames (if you don't want to use the entire texture), ignored if animated is false.
	texWidth: Width of Texture Area (if you don't want to use the entire texture), ignored if animated is false, defaults to Texture's full width.
	texWidth: Height of Texture Area (if you don't want to use the entire texture), ignored if animated is false, defaults to Texture's full height.
	*/
	virtual VSprite* LoadGraphicFromTexture(sf::Texture& texture, bool animated = false, int width = 0, int height = 0, int offsetX = 0, int offsetY = 0, int texWidth = 0, int texHeight = 0);
	/*
	Make sprite texture.
	width: Width of sprite.
	height: Height of sprite.
	color: Fill Colour of sprite.
	outline: Outline thickness.
	outlineColor: colour of sprite outline.
	*/
	virtual VSprite* MakeGraphic(int width, int height, sf::Color color, float outline = 0, sf::Color outlineColor = sf::Color::Transparent);
	/*
	Make sprite texture a circle.
	radius: Radius of circle (width and height will be diameter of circle or radius * 2).
	color: Fill Colour of sprite.
	outline: Outline thickness.
	outlineColor: colour of sprite outline.
	*/
	virtual VSprite* MakeGraphicCircle(int radius, sf::Color color, float outline = 0, sf::Color outlineColor = sf::Color::Transparent);
	/*
	Make sprite texture a shape of a specific number of sides.
	radius: Radius of circle (width and height will be diameter of circle or radius * 2).
	color: Fill Colour of sprite.
	outline: Outline thickness.
	outlineColor: colour of sprite outline.
	*/
	virtual VSprite* MakeGraphicSided(int radius, int sides, sf::Color color, float outline = 0, sf::Color outlineColor = sf::Color::Transparent);
	/*
	Make sprite texture a custom convex shape.
	points: Set of points that make the shape.
	color: Fill Colour of sprite.
	outline: Outline thickness.
	outlineColor: colour of sprite outline.
	*/
	virtual VSprite* MakeGraphicConvex(const std::vector<sf::Vector2f>& points, sf::Color color, float outline = 0, sf::Color outlineColor = sf::Color::Transparent);

	//Return texture of sprite.
	sf::Texture* GetTexture()
	{
		return texture;
	}

	virtual void Destroy() override;
	virtual void Update(float dt) override;
	virtual void Draw(sf::RenderTarget& RenderTarget) override;
};

