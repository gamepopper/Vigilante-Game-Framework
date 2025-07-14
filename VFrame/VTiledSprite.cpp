#include "VTiledSprite.h"
#include "VGlobal.h"

void VTiledSprite::updateTransform()
{
	float angle = -Angle.asRadians();
	float cosine = static_cast<float>(std::cos(angle));
	float sine = static_cast<float>(std::sin(angle));
	float sxc = Scale.x * cosine;
	float syc = Scale.y * cosine;
	float sxs = Scale.x * sine;
	float sys = Scale.y * sine;
	float tx = -Origin.x * sxc - Origin.y * sys + (Position + Origin).x;
	float ty = Origin.x * sxs - Origin.y * syc + (Position + Origin).y;

	RenderState.transform =
		sf::Transform(sxc, sys, tx,
			-sxs, syc, ty,
			0.f, 0.f, 1.f);

	if (vertexArray[0].color != Tint)
	{
		vertexArray[0].color = Tint;
		vertexArray[1].color = Tint;
		vertexArray[2].color = Tint;
		vertexArray[3].color = Tint;
	}
	
	vertexArray[0].position = sf::Vector2f();
	vertexArray[2].position = sf::Vector2f(Size.x, 0.0f);
	vertexArray[3].position = sf::Vector2f(Size.x, Size.y);
	vertexArray[1].position = sf::Vector2f(0.0f, Size.y);
}

void VTiledSprite::updateFrame()
{
	sf::IntRect rect;
	rect.position.x = FlipX ? Animation.GetU() + FrameSize.x : Animation.GetU();
	rect.position.y = FlipY ? Animation.GetV() + FrameSize.y : Animation.GetV();
	rect.size.x = FlipX ? -(int)FrameSize.x : FrameSize.x;
	rect.size.y = FlipY ? -(int)FrameSize.y : FrameSize.y;

	if (image != nullptr)
	{
		if (!const_cast<sf::Texture*>(RenderState.texture)->loadFromImage(*image, false, rect))
		{
			VBase::VLogError("Unable to load image to texture for TiledSprite");
		}
	}
}

VSprite* VTiledSprite::LoadGraphic(const sf::String& filename, bool animated, unsigned int width, unsigned int height, const sf::IntRect& area)
{
	if (RenderState.texture && disposible)
	{
		delete RenderState.texture;
		RenderState.texture = nullptr;
		disposible = false;
	}

	RenderState.texture = new sf::Texture();
	const_cast<sf::Texture*>(RenderState.texture)->setRepeated(true);
	disposible = true;

	sf::Texture* tex = &VGlobal::p()->Content->LoadTexture(filename);
	image = std::unique_ptr<sf::Image>(new sf::Image(tex->copyToImage()));

	setSize(area.size.x == 0 ? image->getSize().x : area.size.x, area.size.y == 0 ? image->getSize().y : area.size.y, animated, width, height, area.position.x, area.position.y);
	Origin = sf::Vector2f();

	width = width == 0 ? image->getSize().x : width;
	height = height == 0 ? image->getSize().y : height;

	updateFrame();

	return this;
}

VSprite* VTiledSprite::LoadGraphicFromTexture(sf::Texture& tex, bool animated, unsigned int width, unsigned int height, const sf::IntRect& area)
{
	if (RenderState.texture && disposible)
	{
		delete RenderState.texture;
		RenderState.texture = nullptr;
		disposible = false;
	}

	RenderState.texture = new sf::Texture();
	const_cast<sf::Texture*>(RenderState.texture)->setRepeated(true);
	disposible = true;
	image = std::unique_ptr<sf::Image>(new sf::Image(tex.copyToImage()));

	setSize(area.size.x == 0 ? image->getSize().x : area.size.x, area.size.y == 0 ? image->getSize().y : area.size.y, animated, width, height, area.position.x, area.position.y);
	Origin = sf::Vector2f();

	width = width == 0 ? image->getSize().x : width;
	height = height == 0 ? image->getSize().y : height;

	updateFrame();

	return this;
}

void VTiledSprite::Destroy()
{
	VSUPERCLASS::Destroy();
	image.reset();
}

void VTiledSprite::Draw(sf::RenderTarget& RenderTarget)
{
	vertexArray[0].position = sf::Vector2f(0,		0);
	vertexArray[2].position = sf::Vector2f(Size.x,	0);
	vertexArray[3].position = sf::Vector2f(Size.x,	Size.y);
	vertexArray[1].position = sf::Vector2f(0,		Size.y);

	vertexArray[0].texCoords = sf::Vector2f(0,		0);
	vertexArray[2].texCoords = sf::Vector2f(Size.x,	0);
	vertexArray[3].texCoords = sf::Vector2f(Size.x,	Size.y);
	vertexArray[1].texCoords = sf::Vector2f(0,		Size.y);

	VSUPERCLASS::Draw(RenderTarget);
}
