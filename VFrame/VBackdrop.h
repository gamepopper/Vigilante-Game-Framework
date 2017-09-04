#pragma once
#include "VSprite.h"
class VBackdrop : public VSprite
{
protected:
	virtual void updateTransform() override;
	virtual void updateFrame() override;

	bool RepeatX;
	bool RepeatY;

public:
	typedef VSprite VSUPERCLASS;

	//Scrolling Coordinates
	sf::Vector2f Scroll;

	//Overrides the scrolling coordinates to use the game's camera.
	bool CameraScroll = true;

	VBackdrop(float scrollX, float scrollY, bool repeatX, bool repeatY, const sf::String& filename) : VSprite(0, 0, filename)
	{
		sf::Texture* tex = const_cast<sf::Texture*>(GetTexture());
			tex->setRepeated(true);

		Size.x = 0;
		Size.y = 0;

		RepeatX = repeatX;
		RepeatY = repeatY;

		ScrollFactor.x = scrollX;
		ScrollFactor.y = scrollY;
	}

	virtual VSprite* MakeGraphic(int width, int height, sf::Color color, float outline = 0, sf::Color outlineColor = sf::Color::Transparent)  { return this; }
	virtual VSprite* MakeGraphicCircle(int radius, sf::Color color, float outline = 0, sf::Color outlineColor = sf::Color::Transparent)  { return this; }
	virtual VSprite* MakeGraphicSided(int radius, int sides, sf::Color color, float outline = 0, sf::Color outlineColor = sf::Color::Transparent)  { return this; }
	virtual VSprite* MakeGraphicConvex(const std::vector<sf::Vector2f>& points, sf::Color color, float outline = 0, sf::Color outlineColor = sf::Color::Transparent)  { return this; }

	virtual VSprite* LoadGraphic(sf::String filename, bool animated = false, int width = 0, int height = 0, const sf::IntRect& area = sf::IntRect()) override;
	virtual VSprite* LoadGraphicFromTexture(sf::Texture& texture, bool animated = false, int width = 0, int height = 0, const sf::IntRect& area = sf::IntRect()) override;

	virtual void Draw(sf::RenderTarget& RenderTarget) override;
};

