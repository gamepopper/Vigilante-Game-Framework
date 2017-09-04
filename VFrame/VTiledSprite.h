#pragma once
#include "VSprite.h"

//Tiled Sprites display a regular texture in a tiled format, so instead of scaling it repeats.
//This is good for backgrounds and also works with sprite animation!
class VTiledSprite : public VSprite
{
private:
	std::unique_ptr<sf::Image> image;

protected:
	virtual void updateTransform() override;
	virtual void updateFrame() override;

public:
	typedef VSprite VSUPERCLASS;

	VTiledSprite(sf::Vector2f Position, const sf::String& Filename = "") : VSprite(Position, "") 
	{
		if (Filename != "")
		{
			LoadGraphic(Filename);
		}

		Animation.GetCurrentFrame();
	}
	VTiledSprite(float x = 0, float y = 0, const sf::String& Filename = "") : VSprite(x, y, "") 
	{ 
		if (Filename != "")
		{
			LoadGraphic(Filename);
		}

		Animation.GetCurrentFrame();
	}

	virtual VSprite* MakeGraphic(int width, int height, sf::Color color, float outline = 0, sf::Color outlineColor = sf::Color::Transparent) { return this; }
	virtual VSprite* MakeGraphicCircle(int radius, sf::Color color, float outline = 0, sf::Color outlineColor = sf::Color::Transparent)  { return this; }
	virtual VSprite* MakeGraphicSided(int radius, int sides, sf::Color color, float outline = 0, sf::Color outlineColor = sf::Color::Transparent)  { return this; }
	virtual VSprite* MakeGraphicConvex(const std::vector<sf::Vector2f>& points, sf::Color color, float outline = 0, sf::Color outlineColor = sf::Color::Transparent)  { return this; }

	virtual VSprite* LoadGraphic(sf::String filename, bool animated = false, int width = 0, int height = 0, const sf::IntRect& area = sf::IntRect()) override;
	virtual VSprite* LoadGraphicFromTexture(sf::Texture& texture, bool animated = false, int width = 0, int height = 0, const sf::IntRect& area = sf::IntRect()) override;

	virtual void Destroy() override;
	virtual void Update(float dt) override;
	virtual void Draw(sf::RenderTarget& RenderTarget) override;
};

