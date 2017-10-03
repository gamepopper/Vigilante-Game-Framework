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
	if (texture)
	{
		delete texture;
		texture = nullptr;
	}

	texture = new sf::Texture();

	sf::Texture* tex = &VGlobal::p()->Content->LoadTexture(filename);
	image = std::unique_ptr<sf::Image>(new sf::Image(tex->copyToImage()));

	setSize(image->getSize().x, image->getSize().y, animated, width, height);
	Origin = sf::Vector2f();

	width = width == 0 ? image->getSize().x : width;
	height = height == 0 ? image->getSize().y : height;

	sprite = sf::Sprite(*texture);
	updateFrame();

	return this;
}

VSprite* VTiledSprite::LoadGraphicFromTexture(sf::Texture& texture, bool animated, int width, int height, const sf::IntRect& area)
{
	if (this->texture)
	{
		delete this->texture;
		this->texture = nullptr;
	}

	this->texture = new sf::Texture();
	image = std::unique_ptr<sf::Image>(new sf::Image(texture.copyToImage()));

	setSize(image->getSize().x, image->getSize().y, animated, width, height);
	Origin = sf::Vector2f();

	width = width == 0 ? image->getSize().x : width;
	height = height == 0 ? image->getSize().y : height;

	sprite = sf::Sprite(*this->texture);
	updateFrame();

	return this;
}

void VTiledSprite::Destroy()
{
	VSUPERCLASS::Destroy();
	image.reset();

	if (texture)
	{
		delete texture;
		texture = nullptr;
	}
}

void VTiledSprite::Update(float dt)
{
	sf::IntRect rect = sprite.getTextureRect();
	rect.left = FlipX ? Animation.GetU() + FrameSize.x : Animation.GetU();
	rect.top = FlipY ? Animation.GetV() + FrameSize.y : Animation.GetV();
	rect.width = FlipX ? -(int)FrameSize.x : FrameSize.x;
	rect.height = FlipY ? -(int)FrameSize.y : FrameSize.y;
	sprite.setTextureRect(rect);

	VSUPERCLASS::Update(dt);
}

void VTiledSprite::Draw(sf::RenderTarget& RenderTarget)
{
	texture->setRepeated(true);
	VSUPERCLASS::Draw(RenderTarget);
	texture->setRepeated(false);
}
