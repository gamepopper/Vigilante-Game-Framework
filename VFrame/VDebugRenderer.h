#pragma once
#include "VBase.h"
#include <SFML/Graphics/VertexArray.hpp>

class VDebugRenderer : public VBase
{
private:
	///Render vertices for drawing collision bounderies of all the objects. Used when in Debug mode with DebugDraw = true.
	sf::VertexArray debuggingVertices;
	unsigned int vertexSet;

	void forEachExistingObject(VBase* b);
	void hideExcessVertices();

public:
	///Used to call parent class functions when they are overrided in class.
	typedef VBase VSUPERCLASS;

	VDebugRenderer();

	void Update(float dt) override;
	void Draw(sf::RenderTarget& RenderTarget) override;
};

