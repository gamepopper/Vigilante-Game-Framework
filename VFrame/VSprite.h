#pragma once
#include "VObject.h"
#include "VAnimationManager.h"

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Color.hpp>

class VSprite :	public VObject
{
protected:
	sf::Texture texture;
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

	virtual VSprite* LoadGraphic(sf::String filename, bool animated = false, int width = 0, int height = 0, int offsetX = 0, int offsetY = 0, int texWidth = 0, int texHeight = 0);
	virtual VSprite* LoadGraphicFromTexture(sf::Texture texture, bool animated = false, int width = 0, int height = 0, int offsetX = 0, int offsetY = 0, int texWidth = 0, int texHeight = 0);
	virtual VSprite* MakeGraphic(int width, int height, sf::Color color, float outline = 0, sf::Color outlineColor = sf::Color::Transparent);
	virtual VSprite* MakeGraphicCircle(int radius, sf::Color color, float outline = 0, sf::Color outlineColor = sf::Color::Transparent);
	virtual VSprite* MakeGraphicSided(int radius, int sides, sf::Color color, float outline = 0, sf::Color outlineColor = sf::Color::Transparent);
	virtual VSprite* MakeGraphicConvex(std::vector<sf::Vector2f>& points, sf::Color color, float outline = 0, sf::Color outlineColor = sf::Color::Transparent);

	sf::Texture& GetTexture()
	{
		return texture;
	}

	virtual void Destroy() override;
	virtual void Update(float dt) override;
	virtual void Draw(sf::RenderTarget& RenderTarget) override;
};

