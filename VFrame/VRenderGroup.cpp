#include "VRenderGroup.h"
#include "VGlobal.h"

class VRenderSprite : public VSprite
{
public:
	VRenderSprite(float x, float y) : VSprite(x, y) {}
	VRenderSprite(sf::Vector2f position) : VSprite(position) {}

	void SetTexture(const sf::Texture& texture)
	{
		sprite.setTexture(texture, true);
	}
};

VRenderGroup::VRenderGroup(float x, float y, unsigned int width, unsigned int height, unsigned int maxSize) : VGroup(maxSize)
{
	Sprite = new VRenderSprite(x, y);
	renderTex.create(width, height);
	postProcessTex.create(width, height);
	Sprite->Size = sf::Vector2f(sf::Vector2u(width, height));

	Sprite->Radius = Sprite->Size.x < Sprite->Size.y ? Sprite->Size.x / 2 : Sprite->Size.y / 2;

	type = RENDERGROUP;
}

VRenderGroup::VRenderGroup(sf::Vector2f position, sf::Vector2u size, unsigned int maxSize) : VGroup(maxSize)
{
	Sprite = new VRenderSprite(position);
	renderTex.create(size.x, size.y);
	postProcessTex.create(size.x, size.y);
	Sprite->Size = sf::Vector2f(size);

	Sprite->Radius = Sprite->Size.x < Sprite->Size.y ? Sprite->Size.x / 2 : Sprite->Size.y / 2;

	type = RENDERGROUP;
}

void VRenderGroup::updateTransform()
{
	sf::View NewView = renderTex.getDefaultView();

	if (RenderViewTransform)
	{
		NewView.move(Sprite->Position + Sprite->Origin - Sprite->Offset);
		NewView.rotate(Sprite->Angle);
		NewView.setSize(sf::Vector2f(Sprite->Size.x * Sprite->Scale.x, Sprite->Size.y * Sprite->Scale.y));
	}
	
	renderTex.setView(NewView);
}

void VRenderGroup::updateTexture(const sf::Texture& texture)
{
	VRenderSprite* render = dynamic_cast<VRenderSprite*>(Sprite);
	render->SetTexture(texture);
}

void VRenderGroup::Resize(int width, int height)
{
	renderTex.create(width, height);
	postProcessTex.create(width, height);
	Sprite->Size = sf::Vector2f(sf::Vector2u(width, height));
}

void VRenderGroup::Kill()
{
	VSUPERCLASS::Kill();
	Sprite->Kill();
}

void VRenderGroup::Revive()
{
	VSUPERCLASS::Revive();
	Sprite->Revive();
}

void VRenderGroup::Destroy()
{
	VSUPERCLASS::Destroy();

	if (Sprite != NULL)
	{
		delete Sprite;
		Sprite = NULL;
	}

	if (PostEffect != NULL)
	{
		delete PostEffect;
		PostEffect = NULL;
	}
}

const sf::Texture& VRenderGroup::GetTexture()
{
	renderTex.clear(sf::Color::Transparent);
	VSUPERCLASS::Draw(renderTex);
	renderTex.display();

	return renderTex.getTexture();
}

void VRenderGroup::Update(float dt)
{
	VSUPERCLASS::Update(dt);
	updateTransform();
	Sprite->Update(dt);

	if (PostEffect != NULL)
	{
		PostEffect->Update(dt);
	}
}

void VRenderGroup::Draw(sf::RenderTarget& RenderTarget)
{
	bool drawDebug = VGlobal::p()->DrawDebug;
	VGlobal::p()->DrawDebug = false;

	renderTex.clear(sf::Color::Transparent);
	VSUPERCLASS::Draw(renderTex);
	renderTex.display();
	
	if (RenderOutside) VSUPERCLASS::Draw(RenderTarget);

	if (PostEffect != NULL && VPostEffectBase::isSupported())
	{
		postProcessTex.clear(sf::Color::Transparent);
		PostEffect->Apply(renderTex, postProcessTex);
		postProcessTex.display();

		updateTexture(postProcessTex.getTexture());
	}
	else
	{
		updateTexture(renderTex.getTexture());
	}

	VGlobal::p()->DrawDebug = drawDebug;
	Sprite->Draw(RenderTarget);
}