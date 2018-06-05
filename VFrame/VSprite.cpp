#include "VSprite.h"
#include "VGlobal.h"

void VSprite::setSize(unsigned int texWidth, unsigned int texHeight, bool animated, int width, int height, int offsetX, int offsetY)
{
	Size = sf::Vector2f(sf::Vector2u(width, height));

	if (Size.x <= 0)
	{
		Size.x = static_cast<float>(animated ? texHeight : texWidth);
		Size.x = static_cast<float>(Size.x > texWidth ? texWidth : Size.x);
	}
	if (Size.y <= 0)
	{
		Size.y = static_cast<float>(animated ? Size.x : texHeight);
		Size.y = static_cast<float>(Size.y > texHeight ? texHeight : Size.y);
	}

	FrameSize = sf::Vector2u(Size);
	Animation = VAnimationManager(texWidth, texHeight, FrameSize.x, FrameSize.y, offsetX, offsetY);
	Origin = sf::Vector2f(FrameSize) / 2.0f;

	Radius = Size.x < Size.y ? Size.x / 2 : Size.y / 2;
}

void VSprite::updateTransform()
{
	if (sprite.getPosition() != Position + Origin - Offset) 
		sprite.setPosition(Position + Origin - Offset);
	if (sprite.getRotation() != Angle) 
		sprite.setRotation(Angle);
	if (sprite.getScale() != Scale)	
		sprite.setScale(Scale);
	if (sprite.getColor() != Tint) 
		sprite.setColor(Tint);
	if (sprite.getOrigin() != Origin) 
		sprite.setOrigin(Origin);
}

void VSprite::updateFrame()
{
	sf::IntRect rect = sprite.getTextureRect();
	rect.left		= FlipX ? Animation.GetU() + FrameSize.x : Animation.GetU();
	rect.top		= FlipY ? Animation.GetV() + FrameSize.y : Animation.GetV();
	rect.width		= FlipX ? -(int)FrameSize.x : FrameSize.x;
	rect.height		= FlipY ? -(int)FrameSize.y : FrameSize.y;
	if (sprite.getTextureRect() != rect) sprite.setTextureRect(rect);
}

VSprite* VSprite::LoadGraphic(sf::String filename, bool animated, int width, int height, const sf::IntRect& area)
{
	texture = &VGlobal::p()->Content->LoadTexture(filename);
	sprite.setTexture(*texture);

	setSize(
		area.width == 0 ? GetTexture()->getSize().x : area.width, 
		area.height == 0 ? GetTexture()->getSize().y : area.height,
		animated, width, height, area.left, area.top
		);

	updateFrame();

	return this;
}

VSprite* VSprite::LoadGraphicFromTexture(sf::Texture& texture, bool animated, int width, int height, const sf::IntRect& area)
{
	this->texture = &texture;
	sprite = sf::Sprite(*this->texture);

	setSize(
		area.width == 0 ? GetTexture()->getSize().x : area.width,
		area.height == 0 ? GetTexture()->getSize().y : area.height,
		animated, width, height, area.left, area.top
		);

	updateFrame();
	return this;
}

VSprite* VSprite::MakeGraphic(int width, int height, sf::Color color, float outline, sf::Color outlineColor)
{
	if (disposible && texture)
	{
		delete texture;
		texture = nullptr;
		disposible = false;
	}

	sf::Image image;
	image.create(width, height, color);

	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			if (x < outline || y < outline || x > width - outline || y > height - outline)
			{
				image.setPixel(x, y, outlineColor);
			}
		}
	}

	texture = new sf::Texture();
	texture->loadFromImage(image);
	disposible = true;

	sprite = sf::Sprite(*texture);
	setSize(width, height, false, width, height);

	return this;
}

VSprite* VSprite::MakeGraphicCircle(int radius, sf::Color color, float outline, sf::Color outlineColor)
{
	if (disposible && texture)
	{
		delete texture;
		texture = nullptr;
		disposible = false;
	}

	unsigned int diameter = radius * 2;

	sf::Image image;
	image.create(diameter, diameter, sf::Color::Transparent);

	for (unsigned int y = 0; y < diameter; y++)
	{
		for (unsigned int x = 0; x < diameter; x++)
		{
			int dx = radius - x;
			int dy = radius - y;
			int length = (dx * dx) + (dy * dy);

			if (length <= (radius * radius))
			{
				if (outline <= 0 || length <= ((radius - outline) * (radius - outline)))
					image.setPixel(x, y, color);
				else
					image.setPixel(x, y, outlineColor);
			}
		}
	}

	texture = new sf::Texture();
	texture->loadFromImage(image);
	disposible = true;

	sprite = sf::Sprite(*texture);
	setSize(radius * 2, radius * 2, false, radius * 2, radius * 2);

	return this;
}

void VSprite::Destroy()
{
	VSUPERCLASS::Destroy();

	if (disposible && texture)
	{
		delete texture;
		texture = nullptr;
	}
}

void VSprite::Update(float dt)
{
	VSUPERCLASS::Update(dt);

	Animation.Update(dt);
	
	bool updateTexture = false;
	if (Animation.GetLastFrame() != Animation.GetCurrentFrame())
		updateTexture = true;

	if ((FlipX && sprite.getTextureRect().width > 0) || (!FlipX && sprite.getTextureRect().width < 0))
		updateTexture = true;

	if ((FlipY && sprite.getTextureRect().height > 0) || (!FlipY && sprite.getTextureRect().height < 0))
		updateTexture = true;

	if (updateTexture)
	{
		updateFrame();
	}
}

void VSprite::Draw(sf::RenderTarget& RenderTarget)
{
	VSUPERCLASS::Draw(RenderTarget);

	sf::View renderTargetView = RenderTarget.getView();
	sf::View scrollView = RenderTarget.getDefaultView();

	if (TestInView(renderTargetView, scrollView, this, sprite.getGlobalBounds()))
	{
		RenderTarget.setView(scrollView);
		RenderTarget.draw(sprite, RenderState);
		RenderTarget.setView(renderTargetView);
	}
}