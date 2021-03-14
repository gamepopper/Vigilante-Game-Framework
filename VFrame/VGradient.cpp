#include "VGradient.h"
#include "VInterpolate.h"
#include <math.h>

void VGradient::updateTransform()
{
	float angle = -Angle * VFRAME_PI / 180.f;
	float cosine = static_cast<float>(cos(angle));
	float sine = static_cast<float>(sin(angle));
	float sxc = Scale.x * cosine;
	float syc = Scale.y * cosine;
	float sxs = Scale.x * sine;
	float sys = Scale.y * sine;
	float tx = -Origin.x * sxc - Origin.y * sys + (Position + Origin).x;
	float ty = Origin.x * sxs - Origin.y * syc + (Position + Origin).y;

	renderState.transform =
		sf::Transform(sxc, sys, tx,
			-sxs, syc, ty,
			0.f, 0.f, 1.f);
}

void VGradient::updateFrame()
{
	unsigned int numPoints = points.size();
	sortedPoints.clear();
	sortedPoints.resize(numPoints);
	for (unsigned int i = 0; i < sortedPoints.size(); i++)
		sortedPoints[i] = points[i].get();

	std::sort(sortedPoints.begin(), sortedPoints.end(),
		[](VColourPoint* first, VColourPoint* second)
	{
		return first->Position < second->Position;
	});

	unsigned int imageWidth = static_cast<unsigned int>(1 * gradResolution);
	if (gradImage.getSize().x != imageWidth)
		gradImage.create(imageWidth, 1);

	unsigned int offset = 0;
	for (unsigned int i = 0; i < sortedPoints.size() - 1; i++)
	{
		unsigned int length = static_cast<unsigned int>((sortedPoints[i + 1]->Position - sortedPoints[i]->Position) * imageWidth);
		for (unsigned int j = 0; j < length; j++)
		{
			gradImage.setPixel(offset + j, 0, VInterpolate::Color(sortedPoints[i]->Tint, sortedPoints[i + 1]->Tint, (float)j, (float)length));
		}

		offset += length;
	}

	if (renderTex.getSize().x != Size.x || renderTex.getSize().y != Size.y)
		renderTex.create(Size.x, Size.y);

	sf::Texture gradTexture;
	gradTexture.loadFromImage(gradImage);
	gradTexture.setSmooth(true);
	renderState.texture = &gradTexture;

	vertexArray.clear();
	switch (gradientType)
	{
	case LINEAR:
	{
		vertexArray.setPrimitiveType(sf::PrimitiveType::Quads);
		vertexArray.append(sf::Vertex(sf::Vector2f(0.0f, Size.y), sf::Vector2f(0.0f, 1.0f)));
		vertexArray.append(sf::Vertex(sf::Vector2f(0.0f, 0.0f), sf::Vector2f(0.0f, 0.0f)));
		vertexArray.append(sf::Vertex(sf::Vector2f(Size.x, 0.0f), sf::Vector2f((float)imageWidth, 0.0f)));
		vertexArray.append(sf::Vertex(sf::Vector2f(Size.x, Size.y), sf::Vector2f((float)imageWidth, 1.0f)));
	}
		break;
	case RADIAL:
	{
		float radius = (Size.x < Size.y ? Size.x : Size.y) / 2.0f;

		vertexArray.setPrimitiveType(sf::PrimitiveType::TriangleFan);
		vertexArray.append(sf::Vertex(sf::Vector2f(Size.x * 0.5f, Size.y * 0.5f), sf::Vector2f(0.0f, 0.0f)));
		for (int i = 0; i < 33; i++)
		{
			float angle = (i / 32.0f) * (VFRAME_PI * 2.0f);
			float sinV = sinf(angle) * radius;
			float cosV = cosf(angle) * radius;

			vertexArray.append(sf::Vertex(sf::Vector2f(Size.x * 0.5f, Size.y * 0.5f) + sf::Vector2f(cosV, sinV), sf::Vector2f((float)imageWidth, 1.0f)));
		}
	}
		break;
	case ANGULAR:
	{
		float radius = sqrtf((Size.x * Size.x) + (Size.y * Size.y)) / 2.0f;

		vertexArray.setPrimitiveType(sf::PrimitiveType::Lines);
		for (int i = 0; i < radius * 10; i++)
		{
			float angle = i / (radius * 10);
			float sinV = sinf(angle * VFRAME_PI * 2.0f) * radius;
			float cosV = cosf(angle * VFRAME_PI * 2.0f) * radius;

			/*vertexArray.append(sf::Vertex(sf::Vector2f(Size.x * angle, 0.0f), sf::Vector2f(angle * imageWidth, 0.0f)));
			vertexArray.append(sf::Vertex(sf::Vector2f(Size.x * angle, Size.y), sf::Vector2f(angle * imageWidth, 1.0f)));
			vertexArray.append(sf::Vertex(sf::Vector2f(Size.x * angle1, Size.y), sf::Vector2f(angle1 * imageWidth, 1.0f)));
			vertexArray.append(sf::Vertex(sf::Vector2f(Size.x * angle1, 0.0f), sf::Vector2f(angle1 * imageWidth, 0.0f)));*/
			vertexArray.append(sf::Vertex(sf::Vector2f(Size.x * 0.5f, Size.y * 0.5f), sf::Vector2f(angle * imageWidth, 0.0f)));
			vertexArray.append(sf::Vertex(sf::Vector2f(Size.x * 0.5f, Size.y * 0.5f) + sf::Vector2f(cosV, sinV), sf::Vector2f(angle * imageWidth, 1.0f)));
		}
	}
		break;
	case REFLECTIVE:
	{
		vertexArray.setPrimitiveType(sf::PrimitiveType::Quads);
		vertexArray.append(sf::Vertex(sf::Vector2f(0.0f, Size.y), sf::Vector2f(0.0f, 1.0f)));
		vertexArray.append(sf::Vertex(sf::Vector2f(0.0f, 0.0f), sf::Vector2f(0.0f, 0.0f)));
		vertexArray.append(sf::Vertex(sf::Vector2f(Size.x * 0.5f, 0.0f), sf::Vector2f((float)imageWidth, 0.0f)));
		vertexArray.append(sf::Vertex(sf::Vector2f(Size.x * 0.5f, Size.y), sf::Vector2f((float)imageWidth, 1.0f)));
		vertexArray.append(sf::Vertex(sf::Vector2f(Size.x * 0.5f, Size.y), sf::Vector2f((float)imageWidth, 1.0f)));
		vertexArray.append(sf::Vertex(sf::Vector2f(Size.x * 0.5f, 0.0f), sf::Vector2f((float)imageWidth, 0.0f)));
		vertexArray.append(sf::Vertex(sf::Vector2f(Size.x, 0.0f), sf::Vector2f(0.0f, 0.0f)));
		vertexArray.append(sf::Vertex(sf::Vector2f(Size.x, Size.y), sf::Vector2f(0.0f, 1.0f)));
	}
		break;
	case DIAMOND:
		float radius = (Size.x < Size.y ? Size.x : Size.y) / 2.0f;

		vertexArray.setPrimitiveType(sf::PrimitiveType::TriangleFan);
		vertexArray.append(sf::Vertex(sf::Vector2f(Size.x * 0.5f, Size.y * 0.5f), sf::Vector2f(0.0f, 0.0f)));
		for (int i = 0; i < 5; i++)
		{
			float angle = (i / 4.0f) * (VFRAME_PI * 2.0f);
			float sinV = sinf(angle) * radius;
			float cosV = cosf(angle) * radius;

			vertexArray.append(sf::Vertex(sf::Vector2f(Size.x * 0.5f, Size.y * 0.5f) + sf::Vector2f(cosV, sinV), sf::Vector2f((float)imageWidth, 1.0f)));
		}
		break;
	}

	renderState.blendMode = sf::BlendNone;
	renderState.transform = sf::Transform::Identity;
	renderTex.clear(sortedPoints[sortedPoints.size() - 1]->Tint);
	renderTex.draw(vertexArray, renderState);
	renderTex.display();

	renderState.blendMode = sf::RenderStates::Default.blendMode;

	vertexArray.clear();
	vertexArray.setPrimitiveType(sf::PrimitiveType::Quads);
	vertexArray.append(sf::Vertex(sf::Vector2f(0.0f, 0.0f), sf::Vector2f(0.0f, 0.0f)));
	vertexArray.append(sf::Vertex(sf::Vector2f(Size.x, 0.0f), sf::Vector2f(Size.x, 0.0f)));
	vertexArray.append(sf::Vertex(sf::Vector2f(Size.x, Size.y), sf::Vector2f(Size.x, Size.y)));
	vertexArray.append(sf::Vertex(sf::Vector2f(0.0f, Size.y), sf::Vector2f(0.0f, Size.y)));

	renderState.texture = &renderTex.getTexture();

	dirty = false;
}

VGradient::VGradient(float x, float y, float width, float height, VGradientType type, float resolution) : VObject(x, y, width, height)
{
	AddPoint(0.0f, sf::Color::Black);
	AddPoint(1.0f, sf::Color::White);
	gradResolution = resolution;
	gradientType = type;
}

VGradient::VGradient(sf::Vector2f position, sf::Vector2f size, VGradientType type, float resolution) : VObject(position, size)
{
	AddPoint(0.0f, sf::Color::Black);
	AddPoint(1.0f, sf::Color::White);
	gradResolution = resolution;
	gradientType = type;
}

void VGradient::AddPoint(float position, const sf::Color& colour)
{
	std::unique_ptr<VColourPoint> p = std::make_unique<VColourPoint>(position, colour);
	points.push_back(std::move(p));
	dirty = true;
}

void VGradient::UpdatePoint(unsigned int index, float position, const sf::Color& colour)
{
	if (index > points.size())
		VBase::VLogError("VGradient Out of Bounds");

	points[index]->Position = position;
	points[index]->Tint = colour;
	dirty = true;
}

void VGradient::UpdatePointColour(unsigned int index, const sf::Color& colour)
{
	if (index > points.size())
		return;

	UpdatePoint(index, points[index]->Position, colour);
}

void VGradient::UpdatePointPosition(unsigned int index, float position)
{
	if (index > points.size())
		return;

	UpdatePoint(index, position, points[index]->Tint);
}

void VGradient::RemovePoint(unsigned int index)
{
	if (index > points.size())
		return;

	points.erase(points.begin() + index);
}

void VGradient::SetType(VGradientType type)
{
	gradientType = type;
	dirty = true;
}

void VGradient::SetResolution(float resolution)
{
	gradResolution = resolution;
	dirty = true;
}

void VGradient::Destroy()
{
	renderTex.clear();
	vertexArray.clear();
	sortedPoints.clear();
	points.clear();

	renderState.texture = 0;
	renderState.shader = 0;
}

void VGradient::Draw(sf::RenderTarget& RenderTarget)
{
	if (dirty)
		updateFrame();

	VSUPERCLASS::Draw(RenderTarget);

	sf::View renderTargetView = RenderTarget.getView();
	sf::View scrollView = RenderTarget.getDefaultView();

	if (TestInView(renderTargetView, &scrollView, this, renderState.transform.transformRect(vertexArray.getBounds())))
	{
		RenderTarget.setView(scrollView);
		RenderTarget.draw(vertexArray, renderState);
		RenderTarget.setView(renderTargetView);
	}
}
