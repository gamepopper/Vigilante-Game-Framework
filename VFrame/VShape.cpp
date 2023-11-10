#include "VShape.h"
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/ConvexShape.hpp>

#include "VGlobal.h"

VShape::VShape(float x, float y, float width, float height) : VObject(x, y, width, height)
{
	SetRectangle(width, height);
	SetFillTint(sf::Color::White);

	Scale = sf::Vector2f(1, 1);

#ifdef _DEBUG
	DebugColor = sf::Color::Green;
#endif
}

VShape::VShape(sf::Vector2f Position, sf::Vector2f Size) : VObject(Position, Size)
{
	SetRectangle(Size.x, Size.y);
	SetFillTint(sf::Color::White);

	Scale = sf::Vector2f(1, 1);

#ifdef _DEBUG
	DebugColor = sf::Color::Green;
#endif
}

void VShape::Destroy()
{
	if (disposable && tex != nullptr)
	{
		delete tex;
		disposable = false;
	}
}

void VShape::updateTransform()
{
	if (Origin != shape->getOrigin())
		shape->setOrigin(Origin);
	if (Position + Origin != shape->getPosition())
		shape->setPosition(Position + Origin);
	if (Angle != shape->getRotation())
		shape->setRotation(Angle);
	if (Scale != shape->getScale())
		shape->setScale(Scale);
}

void VShape::SetRectangle(float width, float height)
{
	Size = sf::Vector2f(width, height);
	shape.reset(new sf::RectangleShape(Size));

	Origin = Size / 2.0f;

	if (width > height)
		Radius = height / 2.0f;
	else
		Radius = width / 2.0f;
}

void VShape::SetCircle(float radius, unsigned int pointCount)
{
	Size = sf::Vector2f(radius * 2.0f, radius * 2.0f);
	shape.reset(new sf::CircleShape(radius, pointCount));

	Origin = Size / 2.0f;
	Radius = radius;
}

void VShape::SetConvex(std::vector<sf::Vector2f>& points)
{
	Size = sf::Vector2f();
	sf::Vector2f offset = sf::Vector2f();
	for (sf::Vector2f& point : points)
	{
		offset.x = std::min(offset.x, point.x);
		offset.y = std::min(offset.y, point.y);
		Size.x = std::max(Size.x, point.x);
		Size.y = std::max(Size.y, point.y);
	}

	sf::ConvexShape* newShape = new sf::ConvexShape(points.size());
	for (unsigned int i = 0; i < points.size(); ++i)
	{
		points[i].x -= offset.x;
		points[i].y -= offset.y;
		newShape->setPoint(i, points[i]);
	}

	Size -= offset;
	Origin = Size / 2.0f;

	Radius = Origin.y * (Origin.x > Origin.y) + Origin.x * (Origin.x <= Origin.y);

	shape.reset(newShape);
}

void VShape::SetCustom(sf::Shape* newShape, float width, float height)
{
	Size = sf::Vector2f(width, height);
	shape.reset(newShape);

	Origin = Size / 2.0f;

	if (Origin.x > Origin.y)
		Radius = Origin.y;
	else
		Radius = Origin.x;
}

sf::Shape* VShape::GetShape()
{
	return shape.get();
}

void VShape::SetFillTint(const sf::Color& tint)
{
	shape->setFillColor(tint);
}

void VShape::SetOutlineTint(const sf::Color& tint)
{
	shape->setOutlineColor(tint);
}

void VShape::SetOutlineThickness(float thickness)
{
	shape->setOutlineThickness(thickness);
}

const sf::Color& VShape::GetFillTint()
{
	return shape->getFillColor();
}

const sf::Color& VShape::GetOutlineTint()
{
	return shape->getOutlineColor();
}

float VShape::GetOutlineThickness()
{
	return shape->getOutlineThickness();
}

void VShape::SetTexture(sf::Texture* texture)
{
	if (disposable && tex)
		delete tex;

	tex = texture;
	disposable = true;
	shape->setTexture(tex, true);
}

void VShape::SetTextureFromFile(const sf::String& filename)
{
	if (disposable && tex)
	{
		delete tex;
		disposable = false;
	}

	tex = &VGlobal::p()->Content->LoadTexture(filename);
	shape->setTexture(tex, true);
}

void VShape::SetTextureRect(const sf::IntRect& rect)
{
	shape->setTextureRect(rect);
}

void VShape::Draw(sf::RenderTarget& RenderTarget)
{
	VSUPERCLASS::Draw(RenderTarget);

	sf::View renderTargetView = RenderTarget.getView();
	sf::View scrollView = RenderTarget.getDefaultView();

	if (TestInView(renderTargetView, &scrollView, this, shape->getGlobalBounds()))
	{
		RenderTarget.setView(scrollView);
		RenderTarget.draw(*shape);
		RenderTarget.setView(renderTargetView);
	}
}
