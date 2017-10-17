#pragma once
#include "VObject.h"
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/System/String.hpp>

class VBackdrop : public VObject
{
protected:
	sf::Texture* texture = nullptr;
	sf::VertexArray vertices;
	sf::Transform transform;

	virtual void updateTransform() override;

	bool RepeatX;
	bool RepeatY;

public:
	typedef VObject VSUPERCLASS;

	//Scrolling Coordinates
	sf::Vector2f Scroll;
	sf::Vector2f ScrollVelocity;
	sf::Vector2f ScrollAcceleration;
	sf::Vector2f ScrollDrag;
	sf::Vector2f ScrollMaxVelocity;
	sf::Vector2f Scale;
	sf::Vector2f FrameSize;

	sf::RenderStates RenderState;

	//Overrides the scrolling coordinates to use the game's camera.
	bool CameraScroll = true;

	VBackdrop(float scrollX, float scrollY, bool repeatX, bool repeatY, const sf::String& filename) : VObject()
	{
		if (filename != "")
			LoadGraphic(filename);

		vertices.setPrimitiveType(sf::Quads);
		vertices.resize(4);

		Size.x = 0;
		Size.y = 0;

		Scale.x = 1;
		Scale.y = 1;

		RepeatX = repeatX;
		RepeatY = repeatY;

		ScrollFactor.x = scrollX;
		ScrollFactor.y = scrollY;

		ScrollMaxVelocity.x = 10000.0f;
		ScrollMaxVelocity.y = 10000.0f;
	}

	VBackdrop* LoadGraphic(sf::String filename);
	VBackdrop* LoadGraphicFromTexture(sf::Texture& texture);

	void SetTint(const sf::Color& tint);
	sf::Color& GetTint();

	virtual void Update(float dt) override;
	virtual void Draw(sf::RenderTarget& RenderTarget) override;
};

