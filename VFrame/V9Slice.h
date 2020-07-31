#pragma once
#include "VObject.h"

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/System/String.hpp>

class V9Slice :	public VObject
{
protected:
	///Render infomation.
	sf::VertexArray vertexArray;
	///The height and width of each corner.
	sf::Vector2f cornerSize;
	///The render states object.
	sf::RenderStates renderState;
	///The texture area rectangle to sample.
	sf::IntRect textureRect;

	///Update the tranform.
	virtual void updateTransform() override;
	///Update the frame.
	virtual void updateFrame();

public:
	///Used to call parent class functions when they are overrided in class.
	typedef VObject VSUPERCLASS;
	/**
	* @param Position Position of the 9-Slice object.
	* @param Size Size of the 9-Slice object.
	* @param Filename File path and name of the texture of this 9-Slice object.
	* @param Corner The width and height of the unscalable corners.
	*/
	V9Slice(sf::Vector2f Position, sf::Vector2f Size, const sf::String& Filename = "", const sf::Vector2f& Corner = sf::Vector2f());
	
	/**
	* @param x X Position of the 9-Slice object.
	* @param y Y Position of the 9-Slice object.
	* @param width Width of the 9-Slice object.
	* @param height Height of the 9-Slice object.
	* @param Filename File path and name of the texture of this 9-Slice object.
	* @param Corner The width and height of the unscalable corners.
	*/
	V9Slice(float x, float y, float width, float height, const sf::String& Filename = "", const sf::Vector2f& Corner = sf::Vector2f());

	///Origin of the 9-slice object in percentage (0.0f - 1.0f) for rotation.
	sf::Vector2f Origin;
	///Colour Tint of the Sprite.
	sf::Color Tint = sf::Color::White;
	///Scale transform.
	sf::Vector2f Scale = sf::Vector2f(1.0f, 1.0f);

	/**
	* Load Graphic from filepath.
	* @param filename Path of texture file.
	* @param corner The width and height of the corner areas.
	* @param rect Only use a certain area of the texture, whether you are animated or not. Default value assumes whole texture is used. (Ignored if VTiledSprite or VBackdrop)
	*/
	virtual V9Slice* LoadGraphic(sf::String filename, const sf::Vector2f& corner, const sf::IntRect& rect = sf::IntRect());

	/**
	* Load Graphic from an sf::Texture object.
	* @param texture sf::Texture object.
	* @param corner The width and height of the corner areas.
	* @param rect Only use a certain area of the texture, whether you are animated or not. Default value assumes whole texture is used. (Ignored if VTiledSprite or VBackdrop)
	*/
	virtual V9Slice* LoadGraphicFromTexture(sf::Texture& texture, const sf::Vector2f& corner, const sf::IntRect& rect = sf::IntRect());

	///Destroys the 9-slice rendering data (and texture if disposable is set for true).
	virtual void Destroy() override;

	/**
	* Updates the 9-slice.
	* @param dt Delta Time between the previous and current frame.
	*/
	virtual void Update(float dt) override;

	/**
	* Renders the sprite.
	* @param RenderTarget The game's sf::RenderTarget object to render data onto.
	*/
	virtual void Draw(sf::RenderTarget& RenderTarget) override;
};

