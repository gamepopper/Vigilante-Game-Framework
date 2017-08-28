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

VSprite* VSprite::LoadGraphic(sf::String filename, bool animated, int width, int height, int offsetX, int offsetY, int texWidth, int texHeight)
{
	if (!VGlobal::p()->Content->LoadTexture(filename, texture))
	{
		return this;
	}

	if (texWidth == 0)
		texWidth = texture.getSize().x;

	if (texHeight == 0)
		texHeight = texture.getSize().y;

	sprite = sf::Sprite(texture);
	setSize(texWidth, texHeight, animated, width, height, offsetX, offsetY);
	updateFrame();

	return this;
}

VSprite* VSprite::LoadGraphicFromTexture(sf::Texture texture, bool animated, int width, int height, int offsetX, int offsetY, int texWidth, int texHeight)
{
	this->texture = texture;
	sprite = sf::Sprite(this->texture);

	if (texWidth == 0)
		texWidth = texture.getSize().x;

	if (texHeight == 0)
		texHeight = texture.getSize().y;

	Size = sf::Vector2f(sf::Vector2u(width, height));
	setSize(texWidth, texHeight, animated, width, height, offsetX, offsetY);
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
	texture = tex.getTexture();
	sprite = sf::Sprite(texture);
	setSize(texture.getSize().x, texture.getSize().y, false, width, height);

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
	texture = tex.getTexture();
	sprite = sf::Sprite(texture);
	setSize(texture.getSize().x, texture.getSize().y, false, radius * 2, radius * 2);

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
	texture = tex.getTexture();
	sprite = sf::Sprite(texture);
	setSize(texture.getSize().x, texture.getSize().y, false, radius * 2, radius * 2);

	return this;
}

VSprite* VSprite::MakeGraphicConvex(std::vector<sf::Vector2f>& points, sf::Color color, float outline, sf::Color outlineColor)
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

	for (unsigned int i = 0; i < points.size(); i++)
	{
		//Contract shape by outline amount
		if (outline > 0)
		{
			sf::Vector2f diff = sf::Vector2f(width / 2.0f, height / 2.0f) - points[i];
			float length = sqrtf((diff.x * diff.x) + (diff.y * diff.y));
			diff /= length;

			points[i] += diff * outline;
		}

		shape.setPoint(i, points[i]);
	}

	shape.setFillColor(color);
	shape.setOutlineThickness(outline);
	shape.setOutlineColor(outlineColor);
	shape.setOrigin(0, 0);

	sf::RenderTexture tex;
	tex.create(width, height);
	tex.draw(shape);
	tex.display();
	texture = tex.getTexture();
	sprite = sf::Sprite(texture);
	setSize(texture.getSize().x, texture.getSize().y, false, width, height);

	return this;
}

void VSprite::Destroy()
{
	VSUPERCLASS::Destroy();
	Animation.Clear();
}

void VSprite::Update(float dt)
{
	VSUPERCLASS::Update(dt);

	Animation.Update(dt);
	
	bool updateTexture = false;
	if (Animation.GetLastFrame() != Animation.GetCurrentFrame())
		updateTexture = true;

	if ((FlipX && sprite.getTextureRect().width < 0) || (!FlipX && sprite.getTextureRect().width > 0))
		updateTexture = true;

	if ((FlipY && sprite.getTextureRect().height < 0) || (!FlipY && sprite.getTextureRect().height > 0))
		updateTexture = true;

	if (updateTexture)
	{
		updateFrame();
	}
}

void VSprite::Draw(sf::RenderTarget& RenderTarget)
{
	VSUPERCLASS::Draw(RenderTarget);

	//Todo: Move this transform code to VObject, along with the line to restore the original transform.
	sf::View renderTargetView = RenderTarget.getView();
	sf::View scrollView = RenderTarget.getDefaultView();

	sf::Vector2f scroll = renderTargetView.getCenter() - scrollView.getCenter();
	scroll.x *= ScrollFactor.x;
	scroll.y *= ScrollFactor.y;

	float rotate = renderTargetView.getRotation() - scrollView.getRotation();
	rotate *= RotateFactor;

	float zoom = renderTargetView.getSize().x / scrollView.getSize().x;
	zoom--;
	zoom *= ZoomFactor;
	zoom++;

	scrollView.move(scroll);
	scrollView.rotate(rotate);
	scrollView.zoom(zoom);
	scrollView.setViewport(renderTargetView.getViewport());

	sf::FloatRect renderBox = sprite.getGlobalBounds();
	float maxSize = fmaxf(scrollView.getSize().x, scrollView.getSize().y);
	sf::FloatRect scrollBox = sf::FloatRect(scrollView.getCenter() - sf::Vector2f(maxSize, maxSize) / 2.0f, sf::Vector2f(maxSize, maxSize));

	if (renderBox.left < scrollBox.left + scrollBox.width &&
		renderBox.left + renderBox.width > scrollBox.left &&
		renderBox.top <	 scrollBox.top + scrollBox.height &&
		renderBox.top +  renderBox.height > scrollBox.top)
	{
		RenderTarget.setView(scrollView);
		RenderTarget.draw(sprite, RenderState);
		RenderTarget.setView(renderTargetView);
	}
}