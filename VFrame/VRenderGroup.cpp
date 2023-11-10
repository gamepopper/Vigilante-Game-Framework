#include "VRenderGroup.h"
#include "VGlobal.h"

///A regular VSprite that allows instant updates of the texture.
class VRenderSprite : public VSprite
{
public:
	VRenderSprite(float x, float y) : VSprite(x, y) {}
	VRenderSprite(sf::Vector2f position) : VSprite(position) {}

	void SetTexture(const sf::Texture& texture)
	{
		RenderState.texture = &texture;
		FrameSize = texture.getSize();

		vertexArray[0].position = sf::Vector2f();
		vertexArray[1].position = sf::Vector2f((float)FrameSize.x, 0.0f);
		vertexArray[2].position = sf::Vector2f((float)FrameSize.x, (float)FrameSize.y);
		vertexArray[3].position = sf::Vector2f(0.0f, (float)FrameSize.y);

		vertexArray[0].texCoords = vertexArray[0].position;
		vertexArray[1].texCoords = vertexArray[1].position;
		vertexArray[2].texCoords = vertexArray[2].position;
		vertexArray[3].texCoords = vertexArray[3].position;

		disposible = true;
	}
};

VRenderGroup::VRenderGroup(unsigned int maxSize) : VGroup(maxSize)
{
	Sprite = std::make_unique<VRenderSprite>(0.0f, 0.0f);
	Camera = std::make_unique<VCamera>();
	type = RENDERGROUP;
}

VRenderGroup::VRenderGroup(float x, float y, unsigned int width, unsigned int height, unsigned int maxSize) : VGroup(maxSize)
{
	Sprite = std::make_unique<VRenderSprite>(x, y);
	Sprite->Size = sf::Vector2f(sf::Vector2u(width, height));
	Sprite->Radius = Sprite->Size.x < Sprite->Size.y ? Sprite->Size.x / 2 : Sprite->Size.y / 2;

	renderTex.create(width, height);
	sf::View defaultView = renderTex.getDefaultView();
	Camera = std::make_unique<VCamera>(defaultView);

	type = RENDERGROUP;
}

VRenderGroup::VRenderGroup(sf::Vector2f position, sf::Vector2u size, unsigned int maxSize) : VGroup(maxSize)
{
	Sprite = std::make_unique<VRenderSprite>(position);
	Sprite->Size = sf::Vector2f(size);
	Sprite->Radius = Sprite->Size.x < Sprite->Size.y ? Sprite->Size.x / 2 : Sprite->Size.y / 2;

	renderTex.create(size.x, size.y);
	sf::View defaultView = renderTex.getDefaultView();
	Camera = std::make_unique<VCamera>(defaultView);

	type = RENDERGROUP;
}

void VRenderGroup::updateTransform()
{
	sf::View NewView = Camera->GetView();

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
	VRenderSprite* render = dynamic_cast<VRenderSprite*>(Sprite.get());
	render->SetTexture(texture);
}

void VRenderGroup::Resize(int width, int height)
{
	postProcessTex.create(width, height);
	renderTex.create(width, height);
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

	Camera = nullptr;
	Sprite = nullptr;
	PostEffect = nullptr;
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
	Sprite->Update(dt);
	Camera->Update(dt);

	if (PostEffect != nullptr)
	{
		PostEffect->Update(dt);
	}
}

void VRenderGroup::Draw(sf::RenderTarget& RenderTarget)
{
	updateTransform();

#ifdef _DEBUG
	bool drawDebug = VGlobal::p()->DrawDebug;
	VGlobal::p()->DrawDebug = false;
#endif

	renderTex.clear(BackgroundTint);
	VSUPERCLASS::Draw(renderTex);
	renderTex.display();

	if (PostEffect != nullptr && VPostEffectBase::isSupported())
	{
		if (postProcessTex.getSize().x == 0 || postProcessTex.getSize().y == 0)
		{
			postProcessTex.create(renderTex.getSize().x, renderTex.getSize().y);
		}

		postProcessTex.clear(sf::Color::Transparent);
		PostEffect->Apply(renderTex.getTexture(), postProcessTex);
		postProcessTex.display();

		updateTexture(postProcessTex.getTexture());
	}
	else
	{
		updateTexture(renderTex.getTexture());
	}

#ifdef _DEBUG
	VGlobal::p()->DrawDebug = drawDebug;
#endif

	if ((RenderOutside & VRENDERGROUP_BEFORE) != 0) 
		VSUPERCLASS::Draw(RenderTarget);

	Sprite->Draw(RenderTarget);
	Camera->Render(RenderTarget);
	
	if ((RenderOutside & VRENDERGROUP_AFTER) != 0) 
		VSUPERCLASS::Draw(RenderTarget);
}