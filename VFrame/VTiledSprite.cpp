#include "VTiledSprite.h"
#include "VGlobal.h"

void VTiledSprite::updateTransform()
{
	float angle = -Angle * VFRAME_PI / 180.f;
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
	vertexArray[1].position = sf::Vector2f(Size.x, 0.0f);
	vertexArray[2].position = sf::Vector2f(Size.x, Size.y);
	vertexArray[3].position = sf::Vector2f(0.0f, Size.y);
}

void VTiledSprite::updateFrame()
{
	sf::IntRect rect;
	rect.left = FlipX ? Animation.GetU() + FrameSize.x : Animation.GetU();
	rect.top = FlipY ? Animation.GetV() + FrameSize.y : Animation.GetV();
	rect.width = FlipX ? -(int)FrameSize.x : FrameSize.x;
	rect.height = FlipY ? -(int)FrameSize.y : FrameSize.y;

	if (image != nullptr)
		const_cast<sf::Texture*>(RenderState.texture)->loadFromImage(*image, rect);
}

VSprite* VTiledSprite::LoadGraphic(const sf::String& filename, bool animated, int width, int height, const sf::IntRect& area)
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

	setSize(area.width == 0 ? image->getSize().x : area.width, area.height == 0 ? image->getSize().y : area.height, animated, width, height, area.left, area.top);
	Origin = sf::Vector2f();

	width = width == 0 ? image->getSize().x : width;
	height = height == 0 ? image->getSize().y : height;

	updateFrame();

	return this;
}

VSprite* VTiledSprite::LoadGraphicFromTexture(sf::Texture& tex, bool animated, int width, int height, const sf::IntRect& area)
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

	setSize(area.width == 0 ? image->getSize().x : area.width, area.height == 0 ? image->getSize().y : area.height, animated, width, height, area.left, area.top);
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
	vertexArray[1].position = sf::Vector2f(Size.x,	0);
	vertexArray[2].position = sf::Vector2f(Size.x,	Size.y);
	vertexArray[3].position = sf::Vector2f(0,		Size.y);

	vertexArray[0].texCoords = sf::Vector2f(0,		0);
	vertexArray[1].texCoords = sf::Vector2f(Size.x,	0);
	vertexArray[2].texCoords = sf::Vector2f(Size.x,	Size.y);
	vertexArray[3].texCoords = sf::Vector2f(0,		Size.y);

	VSUPERCLASS::Draw(RenderTarget);
}
