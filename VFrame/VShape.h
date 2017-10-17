#pragma once
#include "VObject.h"
#include <SFML/Graphics/Shape.hpp>
#include <memory>

class VShape : public VObject
{
protected:
	std::unique_ptr<sf::Shape> shape;
	virtual void updateTransform() override;

public:
	sf::Vector2f Origin;
	sf::Vector2f Scale;
	sf::RenderStates RenderState;

	typedef VObject VSUPERCLASS;
	VShape(float x = 0, float y = 0, float width = 0, float height = 0);
	VShape(sf::Vector2f Position, sf::Vector2f Size);

	void SetRectangle(float width, float height);
	void SetCircle(float radius, unsigned int pointCount = 30);
	void SetConvex(std::vector<sf::Vector2f>& points);
	void SetCustom(sf::Shape* shape, float width, float height);
	sf::Shape* GetShape();

	void SetFillTint(const sf::Color& tint);
	void SetOutlineTint(const sf::Color& tint);
	void SetOutlineThickness(float thickness);
	const sf::Color& GetFillTint();
	const sf::Color& GetOutlineTint();
	float GetOutlineThickness();

	virtual void Draw(sf::RenderTarget& RenderTarget) override;
};

