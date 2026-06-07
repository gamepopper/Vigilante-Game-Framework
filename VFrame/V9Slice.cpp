#include "V9Slice.h"
#include "VGlobal.h"

void V9Slice::updateTransform()
{
	sf::Vector2f origin = sf::Vector2f(Origin.x * Size.x, Origin.y * Size.y);

	float angle = -Angle.asRadians();
	float cosine = static_cast<float>(std::cos(angle));
	float sine = static_cast<float>(std::sin(angle));
	float sxc = Scale.x * cosine;
	float syc = Scale.y * cosine;
	float sxs = Scale.x * sine;
	float sys = Scale.y * sine;
	float tx = -Origin.x * sxc - Origin.y * sys + (Position + origin).x;
	float ty = Origin.x * sxs - Origin.y * syc + (Position + origin).y;

	renderState.transform =
		sf::Transform(sxc, sys, tx,
			-sxs, syc, ty,
			0.f, 0.f, 1.f);

	if (vertexArray[0].color != Tint)
	{
		for (unsigned int i = 0; i < vertexArray.getVertexCount(); ++i)
			vertexArray[i].color = Tint;
	}
}

void V9Slice::updateFrame()
{
	Size.x = Size.x < cornerSize.x * 2 ? cornerSize.x * 2 : Size.x;
	Size.y = Size.y < cornerSize.y * 2 ? cornerSize.y * 2 : Size.y;

	sf::Vector2f barSize = Size - (cornerSize * 2.0f);

	//1 2
	//0 3

	//Top Left Corner
	vertexArray[(0 * 6) + 0].position = sf::Vector2f();
	vertexArray[(0 * 6) + 1].position = sf::Vector2f(cornerSize.x, 0.0f);
	vertexArray[(0 * 6) + 2].position = sf::Vector2f(cornerSize.x, cornerSize.y);
	vertexArray[(0 * 6) + 3].position = sf::Vector2f(cornerSize.x, cornerSize.y);
	vertexArray[(0 * 6) + 4].position = sf::Vector2f(0.0f, cornerSize.y);
	vertexArray[(0 * 6) + 5].position = sf::Vector2f();
	vertexArray[(0 * 6) + 0].texCoords = sf::Vector2f((float)textureRect.position.x, (float)textureRect.position.y);
	vertexArray[(0 * 6) + 1].texCoords = sf::Vector2f((float)textureRect.position.x + cornerSize.x, (float)textureRect.position.y);
	vertexArray[(0 * 6) + 2].texCoords = sf::Vector2f((float)textureRect.position.x + cornerSize.x, (float)textureRect.position.y + cornerSize.y);
	vertexArray[(0 * 6) + 3].texCoords = sf::Vector2f((float)textureRect.position.x + cornerSize.x, (float)textureRect.position.y + cornerSize.y);
	vertexArray[(0 * 6) + 4].texCoords = sf::Vector2f((float)textureRect.position.x, (float)textureRect.position.y + cornerSize.y);
	vertexArray[(0 * 6) + 5].texCoords = sf::Vector2f((float)textureRect.position.x, (float)textureRect.position.y);

	//Top Bar
	vertexArray[(1 * 6) + 0].position = sf::Vector2f(cornerSize.x, 0.0f);
	vertexArray[(1 * 6) + 1].position = sf::Vector2f(cornerSize.x + barSize.x, 0.0f);
	vertexArray[(1 * 6) + 2].position = sf::Vector2f(cornerSize.x + barSize.x, cornerSize.y);
	vertexArray[(1 * 6) + 3].position = sf::Vector2f(cornerSize.x + barSize.x, cornerSize.y);
	vertexArray[(1 * 6) + 4].position = sf::Vector2f(cornerSize.x, cornerSize.y);
	vertexArray[(1 * 6) + 5].position = sf::Vector2f(cornerSize.x, 0.0f);
	vertexArray[(1 * 6) + 0].texCoords = sf::Vector2f((float)textureRect.position.x + cornerSize.x, (float)textureRect.position.y);
	vertexArray[(1 * 6) + 1].texCoords = sf::Vector2f((float)textureRect.position.x + textureRect.size.x - cornerSize.x, (float)textureRect.position.y);
	vertexArray[(1 * 6) + 2].texCoords = sf::Vector2f((float)textureRect.position.x + textureRect.size.x - cornerSize.x, (float)textureRect.position.y + cornerSize.y);
	vertexArray[(1 * 6) + 3].texCoords = sf::Vector2f((float)textureRect.position.x + textureRect.size.x - cornerSize.x, (float)textureRect.position.y + cornerSize.y);
	vertexArray[(1 * 6) + 4].texCoords = sf::Vector2f((float)textureRect.position.x + cornerSize.x, (float)textureRect.position.y + cornerSize.y);
	vertexArray[(1 * 6) + 5].texCoords = sf::Vector2f((float)textureRect.position.x + cornerSize.x, (float)textureRect.position.y);

	//Top Right Corner
	vertexArray[(2 * 6) + 0].position = sf::Vector2f(Size.x - cornerSize.x, 0.0f);
	vertexArray[(2 * 6) + 1].position = sf::Vector2f(Size.x, 0.0f);
	vertexArray[(2 * 6) + 2].position = sf::Vector2f(Size.x, cornerSize.y);
	vertexArray[(2 * 6) + 3].position = sf::Vector2f(Size.x, cornerSize.y);
	vertexArray[(2 * 6) + 4].position = sf::Vector2f(Size.x - cornerSize.x, cornerSize.y);
	vertexArray[(2 * 6) + 5].position = sf::Vector2f(Size.x - cornerSize.x, 0.0f);
	vertexArray[(2 * 6) + 0].texCoords = sf::Vector2f((float)textureRect.position.x + textureRect.size.x - cornerSize.x, (float)textureRect.position.y);
	vertexArray[(2 * 6) + 1].texCoords = sf::Vector2f((float)textureRect.position.x + textureRect.size.x, (float)textureRect.position.y);
	vertexArray[(2 * 6) + 2].texCoords = sf::Vector2f((float)textureRect.position.x + textureRect.size.x, (float)textureRect.position.y + cornerSize.y);
	vertexArray[(2 * 6) + 3].texCoords = sf::Vector2f((float)textureRect.position.x + textureRect.size.x, (float)textureRect.position.y + cornerSize.y);
	vertexArray[(2 * 6) + 4].texCoords = sf::Vector2f((float)textureRect.position.x + textureRect.size.x - cornerSize.x, (float)textureRect.position.y + cornerSize.y);
	vertexArray[(2 * 6) + 5].texCoords = sf::Vector2f((float)textureRect.position.x + textureRect.size.x - cornerSize.x, (float)textureRect.position.y);

	//Left Bar
	vertexArray[(3 * 6) + 0].position = sf::Vector2f(0.0f, cornerSize.y);
	vertexArray[(3 * 6) + 1].position = sf::Vector2f(cornerSize.x, cornerSize.y);
	vertexArray[(3 * 6) + 2].position = sf::Vector2f(cornerSize.x, cornerSize.y + barSize.y);
	vertexArray[(3 * 6) + 3].position = sf::Vector2f(cornerSize.x, cornerSize.y + barSize.y);
	vertexArray[(3 * 6) + 4].position = sf::Vector2f(0.0f, cornerSize.y + barSize.y);
	vertexArray[(3 * 6) + 5].position = sf::Vector2f(0.0f, cornerSize.y);
	vertexArray[(3 * 6) + 0].texCoords = sf::Vector2f((float)textureRect.position.x, (float)textureRect.position.y + cornerSize.y);
	vertexArray[(3 * 6) + 1].texCoords = sf::Vector2f((float)textureRect.position.x + cornerSize.x, (float)textureRect.position.y + cornerSize.y);
	vertexArray[(3 * 6) + 2].texCoords = sf::Vector2f((float)textureRect.position.x + cornerSize.x, (float)textureRect.position.y + textureRect.size.y - cornerSize.y);
	vertexArray[(3 * 6) + 3].texCoords = sf::Vector2f((float)textureRect.position.x + cornerSize.x, (float)textureRect.position.y + textureRect.size.y - cornerSize.y);
	vertexArray[(3 * 6) + 4].texCoords = sf::Vector2f((float)textureRect.position.x, (float)textureRect.position.y + textureRect.size.y - cornerSize.y);
	vertexArray[(3 * 6) + 5].texCoords = sf::Vector2f((float)textureRect.position.x, (float)textureRect.position.y + cornerSize.y);

	//Centre
	vertexArray[(4 * 6) + 0].position = sf::Vector2f(cornerSize.x, cornerSize.y);
	vertexArray[(4 * 6) + 1].position = sf::Vector2f(cornerSize.x + barSize.x, cornerSize.y);
	vertexArray[(4 * 6) + 2].position = sf::Vector2f(cornerSize.x + barSize.x, cornerSize.y + barSize.y);
	vertexArray[(4 * 6) + 3].position = sf::Vector2f(cornerSize.x + barSize.x, cornerSize.y + barSize.y);
	vertexArray[(4 * 6) + 4].position = sf::Vector2f(cornerSize.x, cornerSize.y + barSize.y);
	vertexArray[(4 * 6) + 5].position = sf::Vector2f(cornerSize.x, cornerSize.y);
	vertexArray[(4 * 6) + 0].texCoords = sf::Vector2f((float)textureRect.position.x + cornerSize.x, (float)textureRect.position.y + cornerSize.y);
	vertexArray[(4 * 6) + 1].texCoords = sf::Vector2f((float)textureRect.position.x + textureRect.size.x - cornerSize.x, (float)textureRect.position.y + cornerSize.y);
	vertexArray[(4 * 6) + 2].texCoords = sf::Vector2f((float)textureRect.position.x + textureRect.size.x - cornerSize.x, (float)textureRect.position.y + textureRect.size.y - cornerSize.y);
	vertexArray[(4 * 6) + 3].texCoords = sf::Vector2f((float)textureRect.position.x + textureRect.size.x - cornerSize.x, (float)textureRect.position.y + textureRect.size.y - cornerSize.y);
	vertexArray[(4 * 6) + 4].texCoords = sf::Vector2f((float)textureRect.position.x + cornerSize.x, (float)textureRect.position.y + textureRect.size.y - cornerSize.y);
	vertexArray[(4 * 6) + 5].texCoords = sf::Vector2f((float)textureRect.position.x + cornerSize.x, (float)textureRect.position.y + cornerSize.y);

	//Right Bar
	vertexArray[(5 * 6) + 0].position = sf::Vector2f(Size.x - cornerSize.x, cornerSize.y);
	vertexArray[(5 * 6) + 1].position = sf::Vector2f(Size.x, cornerSize.y);
	vertexArray[(5 * 6) + 2].position = sf::Vector2f(Size.x, cornerSize.y + barSize.y);
	vertexArray[(5 * 6) + 3].position = sf::Vector2f(Size.x, cornerSize.y + barSize.y);
	vertexArray[(5 * 6) + 4].position = sf::Vector2f(Size.x - cornerSize.x, cornerSize.y + barSize.y);
	vertexArray[(5 * 6) + 5].position = sf::Vector2f(Size.x - cornerSize.x, cornerSize.y);
	vertexArray[(5 * 6) + 0].texCoords = sf::Vector2f((float)textureRect.position.x + textureRect.size.x - cornerSize.x, (float)textureRect.position.y + cornerSize.y);
	vertexArray[(5 * 6) + 1].texCoords = sf::Vector2f((float)textureRect.position.x + textureRect.size.x, (float)textureRect.position.y + cornerSize.y);
	vertexArray[(5 * 6) + 2].texCoords = sf::Vector2f((float)textureRect.position.x + textureRect.size.x, (float)textureRect.position.y + textureRect.size.y - cornerSize.y);
	vertexArray[(5 * 6) + 3].texCoords = sf::Vector2f((float)textureRect.position.x + textureRect.size.x, (float)textureRect.position.y + textureRect.size.y - cornerSize.y);
	vertexArray[(5 * 6) + 4].texCoords = sf::Vector2f((float)textureRect.position.x + textureRect.size.x - cornerSize.x, (float)textureRect.position.y + textureRect.size.y - cornerSize.y);
	vertexArray[(5 * 6) + 5].texCoords = sf::Vector2f((float)textureRect.position.x + textureRect.size.x - cornerSize.x, (float)textureRect.position.y + cornerSize.y);

	//Bottom Left Corner
	vertexArray[(6 * 6) + 0].position = sf::Vector2f(0.0f, Size.y - cornerSize.y);
	vertexArray[(6 * 6) + 1].position = sf::Vector2f(cornerSize.x, Size.y - cornerSize.y);
	vertexArray[(6 * 6) + 2].position = sf::Vector2f(cornerSize.x, Size.y);
	vertexArray[(6 * 6) + 3].position = sf::Vector2f(cornerSize.x, Size.y);
	vertexArray[(6 * 6) + 4].position = sf::Vector2f(0.0f, Size.y);
	vertexArray[(6 * 6) + 5].position = sf::Vector2f(0.0f, Size.y - cornerSize.y);
	vertexArray[(6 * 6) + 0].texCoords = sf::Vector2f((float)textureRect.position.x, (float)textureRect.position.y + textureRect.size.y - cornerSize.y);
	vertexArray[(6 * 6) + 1].texCoords = sf::Vector2f((float)textureRect.position.x + cornerSize.x, (float)textureRect.position.y + textureRect.size.y - cornerSize.y);
	vertexArray[(6 * 6) + 2].texCoords = sf::Vector2f((float)textureRect.position.x + cornerSize.x, (float)textureRect.position.y + textureRect.size.y);
	vertexArray[(6 * 6) + 3].texCoords = sf::Vector2f((float)textureRect.position.x + cornerSize.x, (float)textureRect.position.y + textureRect.size.y);
	vertexArray[(6 * 6) + 4].texCoords = sf::Vector2f((float)textureRect.position.x, (float)textureRect.position.y + textureRect.size.y);
	vertexArray[(6 * 6) + 5].texCoords = sf::Vector2f((float)textureRect.position.x, (float)textureRect.position.y + textureRect.size.y - cornerSize.y);

	//Bottom Bar
	vertexArray[(7 * 6) + 0].position = sf::Vector2f(cornerSize.x, Size.y - cornerSize.y);
	vertexArray[(7 * 6) + 1].position = sf::Vector2f(cornerSize.x + barSize.x, Size.y - cornerSize.y);
	vertexArray[(7 * 6) + 2].position = sf::Vector2f(cornerSize.x + barSize.x, Size.y);
	vertexArray[(7 * 6) + 3].position = sf::Vector2f(cornerSize.x + barSize.x, Size.y);
	vertexArray[(7 * 6) + 4].position = sf::Vector2f(cornerSize.x, Size.y);
	vertexArray[(7 * 6) + 5].position = sf::Vector2f(cornerSize.x, Size.y - cornerSize.y);
	vertexArray[(7 * 6) + 0].texCoords = sf::Vector2f((float)textureRect.position.x + cornerSize.x, (float)textureRect.position.y + textureRect.size.y - cornerSize.y);
	vertexArray[(7 * 6) + 1].texCoords = sf::Vector2f((float)textureRect.position.x + textureRect.size.x - cornerSize.x, (float)textureRect.position.y + textureRect.size.y - cornerSize.y);
	vertexArray[(7 * 6) + 2].texCoords = sf::Vector2f((float)textureRect.position.x + textureRect.size.x - cornerSize.x, (float)textureRect.position.y + textureRect.size.y);
	vertexArray[(7 * 6) + 3].texCoords = sf::Vector2f((float)textureRect.position.x + textureRect.size.x - cornerSize.x, (float)textureRect.position.y + textureRect.size.y);
	vertexArray[(7 * 6) + 4].texCoords = sf::Vector2f((float)textureRect.position.x + cornerSize.x, (float)textureRect.position.y + textureRect.size.y);
	vertexArray[(7 * 6) + 5].texCoords = sf::Vector2f((float)textureRect.position.x + cornerSize.x, (float)textureRect.position.y + textureRect.size.y - cornerSize.y);

	//Bottom Right Corner
	vertexArray[(8 * 6) + 0].position = sf::Vector2f(Size.x - cornerSize.x, Size.y - cornerSize.y);
	vertexArray[(8 * 6) + 1].position = sf::Vector2f(Size.x, Size.y - cornerSize.y);
	vertexArray[(8 * 6) + 2].position = sf::Vector2f(Size.x, Size.y);
	vertexArray[(8 * 6) + 3].position = sf::Vector2f(Size.x, Size.y);
	vertexArray[(8 * 6) + 4].position = sf::Vector2f(Size.x - cornerSize.x, Size.y);
	vertexArray[(8 * 6) + 5].position = sf::Vector2f(Size.x - cornerSize.x, Size.y - cornerSize.y);
	vertexArray[(8 * 6) + 0].texCoords = sf::Vector2f((float)textureRect.position.x + textureRect.size.x - cornerSize.x, (float)textureRect.position.y + textureRect.size.y - cornerSize.y);
	vertexArray[(8 * 6) + 1].texCoords = sf::Vector2f((float)textureRect.position.x + textureRect.size.x, (float)textureRect.position.y + textureRect.size.y - cornerSize.y);
	vertexArray[(8 * 6) + 2].texCoords = sf::Vector2f((float)textureRect.position.x + textureRect.size.x, (float)textureRect.position.y + textureRect.size.y);
	vertexArray[(8 * 6) + 3].texCoords = sf::Vector2f((float)textureRect.position.x + textureRect.size.x, (float)textureRect.position.y + textureRect.size.y);
	vertexArray[(8 * 6) + 4].texCoords = sf::Vector2f((float)textureRect.position.x + textureRect.size.x - cornerSize.x, (float)textureRect.position.y + textureRect.size.y);
	vertexArray[(8 * 6) + 5].texCoords = sf::Vector2f((float)textureRect.position.x + textureRect.size.x - cornerSize.x, (float)textureRect.position.y + textureRect.size.y - cornerSize.y);
}

V9Slice::V9Slice(sf::Vector2f Position, sf::Vector2f Size, const sf::String& Filename, const sf::Vector2f& Corner) : VObject(Position, Size)
{
	vertexArray.setPrimitiveType(sf::PrimitiveType::Triangles);
	vertexArray.resize(6 * 9);

	if (Filename != "")
		LoadGraphic(Filename, Corner);

#ifdef VFRAME_DEBUGDRAW
	DebugColor = sf::Color(0, 255, 0, 128);
#endif
}

V9Slice::V9Slice(float x, float y, float width, float height, const sf::String& Filename, const sf::Vector2f& Corner) : VObject(x, y, width, height)
{
	vertexArray.setPrimitiveType(sf::PrimitiveType::Triangles);
	vertexArray.resize(6 * 9);

	if (Filename != "")
		LoadGraphic(Filename, Corner);

#ifdef VFRAME_DEBUGDRAW
	DebugColor = sf::Color(0, 255, 0, 128);
#endif
}

V9Slice* V9Slice::LoadGraphic(const sf::String& filename, const sf::Vector2f& corner, const sf::IntRect& rect)
{
	renderState.texture = &VGlobal::p()->Content->LoadTexture(filename);
	cornerSize = corner;
	textureRect = rect == sf::IntRect() ? sf::IntRect(sf::Vector2i(), sf::Vector2i(renderState.texture->getSize())) : rect;

	return this;
}

V9Slice* V9Slice::LoadGraphicFromTexture(sf::Texture& texture, const sf::Vector2f& corner, const sf::IntRect& rect)
{
	renderState.texture = &texture;
	cornerSize = corner;
	textureRect = rect == sf::IntRect() ? sf::IntRect(sf::Vector2i(), sf::Vector2i(renderState.texture->getSize())) : rect;

	return this;
}

void V9Slice::Draw(sf::RenderTarget& RenderTarget)
{
	VSUPERCLASS::Draw(RenderTarget);

	updateFrame();

	sf::View renderTargetView = RenderTarget.getView();
	sf::View scrollView = RenderTarget.getDefaultView();

	if (TestInView(renderTargetView, &scrollView, this, renderState.transform.transformRect(vertexArray.getBounds())))
	{
		RenderTarget.setView(scrollView);
		RenderTarget.draw(vertexArray, renderState);
		RenderTarget.setView(renderTargetView);
	}
}
