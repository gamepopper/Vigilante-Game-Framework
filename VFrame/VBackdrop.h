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

	sf::Vector2f Scroll;
	bool CameraScroll = true;

	VBackdrop(float scrollX, float scrollY, bool repeatX, bool repeatY, const sf::String& filename) : VSprite(0, 0, filename)
	{
		texture.setRepeated(true);
		Size.x = 0;
		Size.y = 0;

		RepeatX = repeatX;
		RepeatY = repeatY;

		ScrollFactor.x = scrollX;
		ScrollFactor.y = scrollY;
	}

	virtual VSprite* LoadGraphic(sf::String filename, bool animated = false, int width = 0, int height = 0, int offsetX = 0, int offsetY = 0, int texWidth = 0, int texHeight = 0) override;
	virtual VSprite* LoadGraphicFromTexture(sf::Texture texture, bool animated = false, int width = 0, int height = 0, int offsetX = 0, int offsetY = 0, int texWidth = 0, int texHeight = 0) override;

	virtual void Draw(sf::RenderTarget& RenderTarget) override;
};

