#include "VDebugRenderer.h"
#include "VGlobal.h"
#include "VRenderGroup.h"
#include "VEmitter.h"

void VDebugRenderer::forEachExistingObject(VBase* b)
{
	VObject* object = dynamic_cast<VObject*>(b);

	if (object == nullptr && object->active)
		return;

	if (object->type == VType::RENDERGROUP)
	{
		VRenderGroup* renderGroup = dynamic_cast<VRenderGroup*>(object);
		object = renderGroup->Sprite.get();
	}

	unsigned int vertexCount = debuggingVertices.getVertexCount();
	if (vertexSet >= vertexCount)
	{
		vertexCount += 8;
		debuggingVertices.resize(vertexCount);
	}

	const sf::Color& debugColor = object->DebugColor;
	debuggingVertices[0 + vertexSet].position = object->Position;													debuggingVertices[0 + vertexSet].color = debugColor;
	debuggingVertices[1 + vertexSet].position = object->Position + sf::Vector2f(object->Size.x, 0);					debuggingVertices[1 + vertexSet].color = debugColor;
	debuggingVertices[2 + vertexSet].position = object->Position + sf::Vector2f(object->Size.x, 0);					debuggingVertices[2 + vertexSet].color = debugColor;
	debuggingVertices[3 + vertexSet].position = object->Position + sf::Vector2f(object->Size.x, object->Size.y);	debuggingVertices[3 + vertexSet].color = debugColor;
	debuggingVertices[4 + vertexSet].position = object->Position + sf::Vector2f(object->Size.x, object->Size.y);	debuggingVertices[4 + vertexSet].color = debugColor;
	debuggingVertices[5 + vertexSet].position = object->Position + sf::Vector2f(0, object->Size.y);					debuggingVertices[5 + vertexSet].color = debugColor;
	debuggingVertices[6 + vertexSet].position = object->Position + sf::Vector2f(0, object->Size.y);					debuggingVertices[6 + vertexSet].color = debugColor;
	debuggingVertices[7 + vertexSet].position = object->Position;													debuggingVertices[7 + vertexSet].color = debugColor;

	vertexSet += 8;
}

void VDebugRenderer::hideExcessVertices()
{
	unsigned int vertexCount = debuggingVertices.getVertexCount();
	for (vertexSet; vertexSet < vertexCount; vertexSet += 8)
	{
		debuggingVertices[0 + vertexSet].color = sf::Color::Transparent;
		debuggingVertices[1 + vertexSet].color = sf::Color::Transparent;
		debuggingVertices[2 + vertexSet].color = sf::Color::Transparent;
		debuggingVertices[3 + vertexSet].color = sf::Color::Transparent;
		debuggingVertices[4 + vertexSet].color = sf::Color::Transparent;
		debuggingVertices[5 + vertexSet].color = sf::Color::Transparent;
		debuggingVertices[6 + vertexSet].color = sf::Color::Transparent;
		debuggingVertices[7 + vertexSet].color = sf::Color::Transparent;
	}
}

VDebugRenderer::VDebugRenderer()
{
	debuggingVertices.setPrimitiveType(sf::Lines);
}

void VDebugRenderer::Update(float dt)
{
	VSUPERCLASS::Update(dt);

	if (visible = VGlobal::p()->DrawDebug)
	{
		vertexSet = 0;

		VState* currentState = VGlobal::p()->CurrentState();

		if (currentState->active)
			currentState->ForEachExists(std::bind(&VDebugRenderer::forEachExistingObject, this, std::placeholders::_1), true);

		VSubState* subState = currentState->SubState();
		if (subState && subState->active)
			subState->ForEachExists(std::bind(&VDebugRenderer::forEachExistingObject, this, std::placeholders::_1), true);

		hideExcessVertices();
	}
}

void VDebugRenderer::Draw(sf::RenderTarget& RenderTarget)
{
	VSUPERCLASS::Draw(RenderTarget);
	if (visible)
		RenderTarget.draw(debuggingVertices);
}
