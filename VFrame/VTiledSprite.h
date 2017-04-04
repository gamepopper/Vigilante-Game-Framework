#pragma once
#include "VSprite.h"
class VTiledSprite : public VSprite
{
private:
	sf::Image image;
	sf::Image buffer;

protected:
	virtual void updateTransform() override;
	virtual void updateFrame() override;

public:
	VTiledSprite(sf::Vector2f Position, const sf::String& Filename = "") : VSprite(Position, Filename) 
	{ 
		texture.setRepeated(true);
		Animation.GetCurrentFrame();
	}
	VTiledSprite(float x = 0, float y = 0, const sf::String& filename = "") : VSprite(x, y, filename) 
	{ 
		texture.setRepeated(true);
		Animation.GetCurrentFrame();
	}
	virtual ~VTiledSprite()
	{
		Destroy();
	}

	virtual VSprite* LoadGraphic(sf::String filename, bool animated = false, int width = 0, int height = 0, int offsetX = 0, int offsetY = 0, int texWidth = 0, int texHeight = 0) override;
	virtual VSprite* LoadGraphicFromTexture(sf::Texture texture, bool animated = false, int width = 0, int height = 0, int offsetX = 0, int offsetY = 0, int texWidth = 0, int texHeight = 0) override;
};

