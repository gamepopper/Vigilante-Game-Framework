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

#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/Shape.hpp>

VSprite* VSprite::MakeGraphic(int width, int height, sf::Color color, float outline, sf::Color outlineColor)
{
	sf::RectangleShape shape(sf::Vector2f(width - (outline * 2), height - (outline * 2)));
	shape.setFillColor(color);
	shape.setOutlineThickness(outline);
	shape.setOutlineColor(outlineColor);
	shape.setOrigin(shape.getSize() / 2.0f);
	shape.setPosition(width / 2.0f, height / 2.0f);

	sf::RenderTexture tex;
	tex.create(width, height);
	tex.draw(shape);
	tex.display();

	if (disposible && texture)
	{
		delete texture;
		texture = nullptr;
		disposible = false;
	}

	texture = new sf::Texture(tex.getTexture());
	disposible = true;
	sprite = sf::Sprite(*texture);
	setSize(width, height, false, width, height);

	return this;
}

VSprite* VSprite::MakeGraphicCircle(int radius, sf::Color color, float outline, sf::Color outlineColor)
{
	sf::CircleShape shape(radius - outline);
	shape.setFillColor(color);
	shape.setOutlineThickness(outline);
	shape.setOutlineColor(outlineColor);
	shape.setPosition(outline, outline);

	sf::RenderTexture tex;
	tex.create(radius * 2, radius * 2);
	tex.draw(shape);
	tex.display();

	if (disposible && texture)
	{
		delete texture;
		texture = nullptr;
		disposible = false;
	}

	texture = new sf::Texture(tex.getTexture());
	disposible = true;
	sprite = sf::Sprite(*texture);
	setSize(radius * 2, radius * 2, false, radius * 2, radius * 2);

	return this;
}

VSprite* VSprite::MakeGraphicSided(int radius, int sides, sf::Color color, float outline, sf::Color outlineColor)
{
	sf::CircleShape shape(radius - outline, sides);
	shape.setFillColor(color);
	shape.setOutlineThickness(outline);
	shape.setOutlineColor(outlineColor);
	shape.setPosition(outline, outline);

	sf::RenderTexture tex;
	tex.create(radius * 2, radius * 2);
	tex.draw(shape);
	tex.display();

	if (disposible && texture)
	{
		delete texture;
		texture = nullptr;
		disposible = false;
	}

	texture = new sf::Texture(tex.getTexture());
	disposible = true;
	sprite = sf::Sprite(*texture);
	setSize(radius * 2, radius * 2, false, radius * 2, radius * 2);

	return this;
}

VSprite* VSprite::MakeGraphicConvex(const std::vector<sf::Vector2f>& points, sf::Color color, float outline, sf::Color outlineColor)
{
	sf::ConvexShape shape;
	shape.setPointCount(points.size());

	float top = 0xFFFFFF, bottom = -0xFFFFFF, left = 0xFFFFFF, right = -0xFFFFFF;
	for (unsigned int i = 0; i < points.size(); i++)
	{
		top		= top		> points[i].y ? points[i].y : top;
		left	= left		> points[i].y ? points[i].y : left;
		bottom	= bottom	< points[i].y ? points[i].y : bottom;
		right	= right		< points[i].y ? points[i].y : right;
	}

	int width	= static_cast<int>(ceil(right - left));
	int height	= static_cast<int>(ceil(bottom - top));

	std::vector<sf::Vector2f> outlinePoints(points.size());
	for (unsigned int i = 0; i < points.size(); i++)
	{
		//Contract shape by outline amount
		if (outline > 0)
		{
			sf::Vector2f diff = sf::Vector2f(width / 2.0f, height / 2.0f) - points[i];
			float length = sqrtf((diff.x * diff.x) + (diff.y * diff.y));
			diff /= length;

			outlinePoints[i] = points[i] + (diff * outline);
			shape.setPoint(i, outlinePoints[i]);
		}
		else
		{
			shape.setPoint(i, points[i]);
		}
	}

	shape.setFillColor(color);
	shape.setOutlineThickness(outline);
	shape.setOutlineColor(outlineColor);
	shape.setOrigin(0, 0);

	sf::RenderTexture tex;
	tex.create(width, height);
	tex.draw(shape);
	tex.display();

	if (disposible && texture)
	{
		delete texture;
		texture = nullptr;
		disposible = false;
	}

	texture = new sf::Texture(tex.getTexture());
	disposible = true;
	sprite = sf::Sprite(*texture);
	setSize(width, height, false, width, height);

	return this;
}

void VSprite::Destroy()
{
	VSUPERCLASS::Destroy();
	Animation.Clear();

	if (disposible && texture)
	{
		delete texture;
		texture = nullptr;
		disposible = false;
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