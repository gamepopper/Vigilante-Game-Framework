#include "VBackdrop.h"
#include "VGlobal.h"
#include <cmath>

VBackdrop::VBackdrop(float scrollX, float scrollY, bool repeatX, bool repeatY, const sf::String& filename) : VObject()
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

void VBackdrop::updateTransform()
{
	transform = sf::Transform::Identity;
	transform.translate(Position);
	transform.scale(Scale);
}

VBackdrop* VBackdrop::LoadGraphic(sf::String filename, const sf::IntRect& area)
{
	if (disposable)
	{
		delete texture;
		texture = nullptr;
		disposable = false;
	}

	if (area == sf::IntRect())
	{
		texture = &VGlobal::p()->Content->LoadTexture(filename);
	}
	else
	{
		texture = new sf::Texture();
		sf::Texture* tex = &VGlobal::p()->Content->LoadTexture(filename);
		texture->loadFromImage(tex->copyToImage(), area);
		disposable = true;
	}

	return this;
}

VBackdrop* VBackdrop::LoadGraphicFromTexture(sf::Texture& texture)
{
	if (disposable)
	{
		delete this->texture;
		this->texture = nullptr;
		disposable = false;
	}

	this->texture = &texture;
	return this;
}

void VBackdrop::SetTint(const sf::Color& tint)
{
	vertices[0].color = tint;
	vertices[1].color = tint;
	vertices[2].color = tint;
	vertices[3].color = tint;
}

void VBackdrop::Destroy()
{
	VSUPERCLASS::Destroy();

	if (disposable)
	{
		delete this->texture;
		this->texture = nullptr;
		disposable = false;
	}
}

void VBackdrop::Update(float dt)
{
	VSUPERCLASS::Update(dt);

	float deltaV;
	float delta;
	deltaV = 0.5f * (computeVelocity(ScrollVelocity.x, ScrollAcceleration.x, ScrollDrag.x, ScrollMaxVelocity.x, dt) - ScrollVelocity.x);
	ScrollVelocity.x += deltaV;
	delta = ScrollVelocity.x * dt;
	ScrollVelocity.x += deltaV;
	Scroll.x += delta;

	deltaV = 0.5f * (computeVelocity(ScrollVelocity.y, ScrollAcceleration.y, ScrollDrag.y, ScrollMaxVelocity.y, dt) - ScrollVelocity.y);
	ScrollVelocity.y += deltaV;
	delta = ScrollVelocity.y * dt;
	ScrollVelocity.y += deltaV;
	Scroll.y += delta;
}

sf::Color& VBackdrop::GetTint()
{
	return vertices[0].color;
}

void VBackdrop::Draw(sf::RenderTarget& RenderTarget)
{
	texture->setRepeated(true);

	//Todo: Move this transform code to VObject, along with the line to restore the original transform.
	sf::View renderTargetView = RenderTarget.getView();
	sf::View scrollView = RenderTarget.getDefaultView();

	sf::Vector2f scroll = renderTargetView.getCenter() - scrollView.getCenter();
	sf::Vector2f bScroll = CameraScroll ? scroll : sf::Vector2f();
	bScroll += Scroll;

	bScroll.x *= ScrollFactor.x;
	bScroll.y *= ScrollFactor.y;

	float rotate = renderTargetView.getRotation() - scrollView.getRotation();
	rotate *= RotateFactor;

	float zoom = renderTargetView.getSize().x / scrollView.getSize().x;
	zoom--;
	zoom *= ZoomFactor;
	zoom++;

	sf::Vector2f texPos;
	sf::Vector2f texSize = sf::Vector2f(texture->getSize());
	Size = texSize;

	if (RepeatX)
	{
		texPos.x = bScroll.x;
		texSize.x = (float)VGlobal::p()->Width;
		texSize.x /= Scale.x;
		texPos.x += (texSize.x / 2.0f) * (zoom - 1.0f);
		texSize.x /= zoom;
		Size.x = VGlobal::p()->Width / Scale.x;
	}
	else
	{
		Size.x *= zoom;
	}

	if (RepeatY)
	{
		texPos.y = bScroll.y;
		texSize.y = (float)VGlobal::p()->Height;
		texSize.y /= Scale.y;
		texPos.y += (texSize.y / 2.0f) * (zoom - 1.0f);
		texSize.y /= zoom;
		Size.y = VGlobal::p()->Height / Scale.x;
	}
	else
	{
		Size.y *= zoom;
	}

	vertices[0].position = sf::Vector2f(0, 0);
	vertices[0].texCoords = texPos + sf::Vector2f(0, 0);

	vertices[1].position = sf::Vector2f(Size.x, 0);
	vertices[1].texCoords = texPos + sf::Vector2f(texSize.x, 0);

	vertices[2].position = sf::Vector2f(Size.x, Size.y);
	vertices[2].texCoords = texPos + sf::Vector2f(texSize.x, texSize.y);

	vertices[3].position = sf::Vector2f(0, Size.y);
	vertices[3].texCoords = texPos + sf::Vector2f(0, texSize.y);

	scrollView.setRotation(rotate);

	sf::FloatRect renderBox(Position, Size);
	float maxSize = fmaxf(scrollView.getSize().x, scrollView.getSize().y);
	sf::FloatRect scrollBox = sf::FloatRect(scrollView.getCenter() - sf::Vector2f(maxSize, maxSize) / 2.0f, sf::Vector2f(maxSize, maxSize));

	if (renderBox.left < scrollBox.left + scrollBox.width &&
		renderBox.left + renderBox.width > scrollBox.left &&
		renderBox.top <	 scrollBox.top + scrollBox.height &&
		renderBox.top + renderBox.height > scrollBox.top)
	{
		updateTransform();

		RenderState.texture = texture;
		RenderState.transform = transform;

		RenderTarget.setView(scrollView);
		RenderTarget.draw(vertices, RenderState);
		RenderTarget.setView(renderTargetView);
	}

	Size = sf::Vector2f();

	texture->setRepeated(false);
}
