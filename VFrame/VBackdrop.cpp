#include "VBackdrop.h"
#include <cmath>

void VBackdrop::updateTransform()
{
	if (sprite.getPosition() != Position)
		sprite.setPosition(Position);
	if (sprite.getRotation() != Angle)
		sprite.setRotation(Angle);
	if (sprite.getScale() != Scale)
		sprite.setScale(Scale);
	if (sprite.getColor() != Tint)
		sprite.setColor(Tint);
}

void VBackdrop::updateFrame()
{
	
}

VSprite* VBackdrop::LoadGraphic(sf::String filename, bool animated, int width, int height, const sf::IntRect& area)
{
	VSUPERCLASS::LoadGraphic(filename);

	Size.x = 0;
	Size.y = 0;
	return this;
}

VSprite* VBackdrop::LoadGraphicFromTexture(sf::Texture& texture, bool animated, int width, int height, const sf::IntRect& area)
{
	VSUPERCLASS::LoadGraphicFromTexture(texture);

	Size.x = 0;
	Size.y = 0;
	return this;
}

void VBackdrop::Draw(sf::RenderTarget& RenderTarget)
{
	updateTransform();

	texture->setRepeated(true);

	//Todo: Move this transform code to VObject, along with the line to restore the original transform.
	sf::View renderTargetView = RenderTarget.getView();
	sf::View scrollView = RenderTarget.getDefaultView();

	sf::Vector2f scroll = CameraScroll ? (renderTargetView.getCenter() - scrollView.getCenter()) : sf::Vector2f();
	scroll += Scroll;

	scroll.x *= ScrollFactor.x;
	scroll.y *= ScrollFactor.y;

	sf::Vector2f texSize = sf::Vector2f(GetTexture()->getSize());

	if (RepeatX)
	{
		texSize.x = renderTargetView.getSize().x;
		texSize.x /= Scale.x;
	}

	if (RepeatY)
	{
		texSize.y = renderTargetView.getSize().y;
		texSize.y /= Scale.y;
	}

	sf::IntRect Rect;
	Rect.left = static_cast<int>((scroll.x * (RepeatX ? 1 : 0)) - (texSize.x / 2.0f));
	Rect.top = static_cast<int>((scroll.y * (RepeatY ? 1 : 0)) - (texSize.y / 2.0f));
	Rect.width = static_cast<int>(texSize.x);
	Rect.height = static_cast<int>(texSize.y);

	if (!CameraScroll)
	{
		Rect.left += static_cast<int>(texSize.x / 2.0f);
		Rect.top += static_cast<int>(texSize.y / 2.0f);
	}

	sprite.setTextureRect(Rect);

	sf::Vector2f oldPosition = sprite.getPosition();
	sf::Vector2f newPosition = oldPosition + (scrollView.getSize() / 2.0f - renderTargetView.getSize() / 2.0f);
	newPosition.x -= RepeatX ? 0 : scroll.x;
	newPosition.y -= RepeatY ? 0 : scroll.y;
	sprite.setPosition(newPosition);

	scroll = renderTargetView.getCenter() - scrollView.getCenter();
	scroll.x *= RepeatX ? 0 : ScrollFactor.x;
	scroll.y *= RepeatY ? 0 : ScrollFactor.y;

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
		renderBox.top + renderBox.height > scrollBox.top)
	{
		RenderTarget.setView(scrollView);
		RenderTarget.draw(sprite, RenderState);
		RenderTarget.setView(renderTargetView);
	}

	sprite.setPosition(oldPosition);
	texture->setRepeated(false);
}
