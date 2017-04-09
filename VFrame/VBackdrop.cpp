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
	if (sprite.getColor() != Tint)
		sprite.setColor(Tint);
}

VSprite* VBackdrop::LoadGraphic(sf::String filename, bool animated, int width, int height, int offsetX, int offsetY, int texWidth, int texHeight)
{
	VSUPERCLASS::LoadGraphic(filename);
	texture.setRepeated(true);
	Size.x = 0;
	Size.y = 0;
	return this;
}

VSprite* VBackdrop::LoadGraphicFromTexture(sf::Texture texture, bool animated, int width, int height, int offsetX, int offsetY, int texWidth, int texHeight)
{
	VSUPERCLASS::LoadGraphicFromTexture(texture);
	texture.setRepeated(true);
	Size.x = 0;
	Size.y = 0;
	return this;
}

void VBackdrop::Draw(sf::RenderTarget& RenderTarget)
{
	VObject::Draw(RenderTarget);

	//Todo: Move this transform code to VObject, along with the line to restore the original transform.
	sf::View renderTargetView = RenderTarget.getView();
	sf::View scrollView = RenderTarget.getDefaultView();

	if (CameraScroll)
		Scroll = renderTargetView.getCenter() - scrollView.getCenter();

	Scroll.x *= RepeatX ? ScrollFactor.x : 0;
	Scroll.y *= RepeatY ? ScrollFactor.y : 0;

	sf::Vector2f texSize = scrollView.getSize();
	texSize.x /= Scale.x;
	texSize.y /= Scale.y;

	sf::IntRect Rect = sf::IntRect(sf::FloatRect(Scroll, texSize));
	sprite.setTextureRect(Rect);

	sf::Vector2f scroll = renderTargetView.getCenter() - scrollView.getCenter();
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
}
