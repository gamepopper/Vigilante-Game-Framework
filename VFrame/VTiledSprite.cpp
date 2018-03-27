#include "VTiledSprite.h"
#include "VGlobal.h"

void VTiledSprite::updateTransform()
{
	if (sprite.getPosition() != Position + Origin)
		sprite.setPosition(Position + Origin);
	if (sprite.getRotation() != Angle)
		sprite.setRotation(Angle);
	if (sprite.getColor() != Tint)
		sprite.setColor(Tint);
	if (sprite.getTextureRect() != sf::IntRect(0, 0, static_cast<int>(Size.x), static_cast<int>(Size.y)))
		sprite.setTextureRect(sf::IntRect(0, 0, static_cast<int>(Size.x), static_cast<int>(Size.y)));
	if (sprite.getOrigin() != Origin)
		sprite.setOrigin(Origin);
}

void VTiledSprite::updateFrame()
{
	sf::IntRect rect = sprite.getTextureRect();
	rect.left = FlipX ? Animation.GetU() + FrameSize.x : Animation.GetU();
	rect.top = FlipY ? Animation.GetV() + FrameSize.y : Animation.GetV();
	rect.width = FlipX ? -(int)FrameSize.x : FrameSize.x;
	rect.height = FlipY ? -(int)FrameSize.y : FrameSize.y;

	if (image != nullptr)
		GetTexture()->loadFromImage(*image, rect);
}

VSprite* VTiledSprite::LoadGraphic(sf::String filename, bool animated, int width, int height, const sf::IntRect& area)
{
	if (texture && disposible)
	{
		delete texture;
		texture = nullptr;
		disposible = false;
	}

	texture = new sf::Texture();
	disposible = true;

	sf::Texture* tex = &VGlobal::p()->Content->LoadTexture(filename);
	image = std::unique_ptr<sf::Image>(new sf::Image(tex->copyToImage()));

	setSize(area.width == 0 ? image->getSize().x : area.width, area.height == 0 ? image->getSize().y : area.height, animated, width, height, area.left, area.top);
	Origin = sf::Vector2f();

	width = width == 0 ? image->getSize().x : width;
	height = height == 0 ? image->getSize().y : height;

	sprite = sf::Sprite(*texture);
	updateFrame();

	return this;
}

VSprite* VTiledSprite::LoadGraphicFromTexture(sf::Texture& tex, bool animated, int width, int height, const sf::IntRect& area)
{
	if (texture && disposible)
	{
		delete texture;
		texture = nullptr;
		disposible = false;
	}

	this->texture = new sf::Texture();
	disposible = true;
	image = std::unique_ptr<sf::Image>(new sf::Image(tex.copyToImage()));

	setSize(area.width == 0 ? image->getSize().x : area.width, area.height == 0 ? image->getSize().y : area.height, animated, width, height, area.left, area.top);
	Origin = sf::Vector2f();

	width = width == 0 ? image->getSize().x : width;
	height = height == 0 ? image->getSize().y : height;

	sprite = sf::Sprite(*texture);
	updateFrame();

	return this;
}

void VTiledSprite::Destroy()
{
	VSUPERCLASS::Destroy();
	image.reset();
}

void VTiledSprite::Update(float dt)
{
	VSUPERCLASS::Update(dt);

	sf::IntRect rect = sprite.getTextureRect();
	rect.left = FlipX ? Animation.GetU() + FrameSize.x : Animation.GetU();
	rect.top = FlipY ? Animation.GetV() + FrameSize.y : Animation.GetV();
	rect.width = FlipX ? -(int)FrameSize.x : FrameSize.x;
	rect.height = FlipY ? -(int)FrameSize.y : FrameSize.y;
	sprite.setTextureRect(rect);
}

void VTiledSprite::Draw(sf::RenderTarget& RenderTarget)
{
	texture->setRepeated(true);
	VSUPERCLASS::Draw(RenderTarget);
	texture->setRepeated(false);
}
