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
		vertexArray[2].position = sf::Vector2f((float)FrameSize.x, 0.0f);
		vertexArray[3].position = sf::Vector2f((float)FrameSize.x, (float)FrameSize.y);
		vertexArray[1].position = sf::Vector2f(0.0f, (float)FrameSize.y);

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
	type = VType::RENDERGROUP;
}

VRenderGroup::VRenderGroup(float x, float y, unsigned int width, unsigned int height, unsigned int maxSize) : VGroup(maxSize)
{
	Sprite = std::make_unique<VRenderSprite>(x, y);
	Sprite->Size = sf::Vector2f(sf::Vector2u(width, height));
	Sprite->Radius = Sprite->Size.x < Sprite->Size.y ? Sprite->Size.x / 2 : Sprite->Size.y / 2;

	if (!renderTex.resize({ width, height }))
	{
		VLogError("Invalid Texture Size for RenderGroup");
	}
	sf::View defaultView = renderTex.getDefaultView();
	Camera = std::make_unique<VCamera>(defaultView);

	type = VType::RENDERGROUP;
}

VRenderGroup::VRenderGroup(sf::Vector2f position, sf::Vector2u size, unsigned int maxSize) : VGroup(maxSize)
{
	Sprite = std::make_unique<VRenderSprite>(position);
	Sprite->Size = sf::Vector2f(size);
	Sprite->Radius = Sprite->Size.x < Sprite->Size.y ? Sprite->Size.x / 2 : Sprite->Size.y / 2;

	if (!renderTex.resize(size))
	{
		VLogError("Invalid Texture Size for RenderGroup");
	}
	sf::View defaultView = renderTex.getDefaultView();
	Camera = std::make_unique<VCamera>(defaultView);

	type = VType::RENDERGROUP;
}

void VRenderGroup::updateTransform()
{
	sf::View NewView = Camera->GetView();

	if (RenderViewTransform)
	{
		NewView.setSize(sf::Vector2f(Sprite->Size.x * Sprite->Scale.x, Sprite->Size.y * Sprite->Scale.y));
		NewView.setCenter(Sprite->Position + Sprite->Origin - Sprite->Offset + (NewView.getSize() / 2.0f));
		NewView.setRotation(Sprite->Angle);
	}
	
	renderTex.setView(NewView);
}

void VRenderGroup::updateTexture(const sf::Texture& texture)
{
	VRenderSprite* render = dynamic_cast<VRenderSprite*>(Sprite.get());
	render->SetTexture(texture);
}

void VRenderGroup::Resize(unsigned int width, unsigned int height)
{
	if (postProcessTex.resize({ width, height }) && renderTex.resize({ width, height }))
	{
		Sprite->Size = sf::Vector2f(sf::Vector2u(width, height));
		return;
	}

	VLogError("Invalid Texture Size to resize RenderGroup");
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

#ifdef VFRAME_DEBUGDRAW
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
			if (!postProcessTex.resize(renderTex.getSize()))
			{
				VLogError("Invalid Texture Size to resize post processing texture in RenderGroup");
			}
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

#ifdef VFRAME_DEBUGDRAW
	VGlobal::p()->DrawDebug = drawDebug;
#endif

	if ((RenderOutside & VRENDERGROUP_BEFORE) != 0) 
		VSUPERCLASS::Draw(RenderTarget);

	Sprite->Draw(RenderTarget);
	Camera->Render(RenderTarget);
	
	if ((RenderOutside & VRENDERGROUP_AFTER) != 0) 
		VSUPERCLASS::Draw(RenderTarget);
}