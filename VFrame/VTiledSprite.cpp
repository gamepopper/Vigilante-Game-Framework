#include "VTiledSprite.h"
#include "VGlobal.h"

void VTiledSprite::updateTransform()
{
	if (sprite.getPosition() != Position) 
		sprite.setPosition(Position);
	if (sprite.getRotation() != Angle) 
		sprite.setRotation(Angle);
	if (sprite.getColor() != Tint) 
		sprite.setColor(Tint);
	if (sprite.getTextureRect() != sf::IntRect(0, 0, static_cast<int>(Size.x), static_cast<int>(Size.y))) 
		sprite.setTextureRect(sf::IntRect(0, 0, static_cast<int>(Size.x), static_cast<int>(Size.y)));
}

void VTiledSprite::updateFrame()
{
	sf::IntRect rect = sprite.getTextureRect();
	rect.left = FlipX ? Animation.GetU() + FrameSize.x : Animation.GetU();
	rect.top = FlipY ? Animation.GetV() + FrameSize.y : Animation.GetV();
	rect.width = FlipX ? -(int)FrameSize.x : FrameSize.x;
	rect.height = FlipY ? -(int)FrameSize.y : FrameSize.y;

	if (image.getSize().x > 0 && image.getSize().y > 0)
		texture.loadFromImage(image, rect);
}

VSprite* VTiledSprite::LoadGraphic(sf::String filename, bool animated, int width, int height, int offsetX, int offsetY, int texWidth, int texHeight)
{
	if (!VGlobal::p()->Content->FindImage(filename))
	{
		if (!VGlobal::p()->Content->LoadTexture(filename, texture))
		{
			return this;
		}
		else
		{
			image = texture.copyToImage();
		}
	}
	else
	{
		if (!VGlobal::p()->Content->LoadImage(filename, image))
		{
			return this;
		}
	}

	if (texWidth == 0)
		texWidth = image.getSize().x;

	if (texHeight == 0)
		texHeight = image.getSize().y;

	setSize(texWidth, texHeight, animated, width, height, offsetX, offsetY);

	width = width == 0 ? texWidth : width;
	height = height == 0 ? texHeight : height;

	sprite = sf::Sprite(texture);
	texture.setRepeated(true);
	updateFrame();

	return this;
}

VSprite* VTiledSprite::LoadGraphicFromTexture(sf::Texture tex, bool animated, int width, int height, int offsetX, int offsetY, int texWidth, int texHeight)
{
	image = tex.copyToImage();

	if (texWidth == 0)
		texWidth = image.getSize().x;

	if (texHeight == 0)
		texHeight = image.getSize().y;

	setSize(texWidth, texHeight, animated, width, height, offsetX, offsetY);

	width = width == 0 ? texWidth : width;
	height = height == 0 ? texHeight : height;

	sprite = sf::Sprite(this->texture);
	texture.setRepeated(true);
	updateFrame();

	return this;
}