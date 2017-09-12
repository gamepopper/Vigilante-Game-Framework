#include "VEmitter.h"
#include "VGlobal.h"

VEmitter* VEmitter::LoadParticlesFromFile(int Amount, sf::String Filename, bool Animated, int Width, int Height, const sf::IntRect& Rect, bool RandomFrames)
{
	MaxSize = Amount;	
	RenderState.texture = &VGlobal::p()->Content->LoadTexture(Filename);
	setSize(Amount, Animated, Width, Height, Rect, RandomFrames);

	return this;
}

VEmitter* VEmitter::LoadParticles(int Amount, sf::Texture& Texture, bool Animated, int Width, int Height, const sf::IntRect& Rect, bool RandomFrames)
{
	MaxSize = Amount;
	RenderState.texture = &Texture;
	setSize(Amount, Animated, Width, Height, Rect, RandomFrames);

	return this;
}

VEmitter* VEmitter::MakeParticles(int Amount, int Width, int Height, sf::Color Color)
{
	MaxSize = Amount;

	sf::RenderTexture renderTex;
	renderTex.create(Width, Height);
	renderTex.clear(Color);
	RenderState.texture = &renderTex.getTexture();
	ParticleInstance->Size = sf::Vector2f(sf::Vector2u(Width, Height));

	setSize(Amount, false, Width, Height, sf::IntRect(0, 0, Width, Height), false);

	return this;
}

void VEmitter::setSize(int Amount, bool Animated, int Width, int Height, const sf::IntRect& Rect, bool RandomFrames)
{
	sf::Vector2f Size;
	int FrameCount;
	int FrameCountY;
	int TextureWidth = Rect.width == 0 ? RenderState.texture->getSize().x : Rect.width;
	sf::Vector2f Offset = sf::Vector2f((float)Rect.left, (float)Rect.top);

	if (Animated)
	{
		Size = sf::Vector2f(static_cast<float>(Width), static_cast<float>(Height));
		FrameCount = TextureWidth / Width;
		FrameCountY = RenderState.texture->getSize().y / Height;
	}
	else
	{
		Size = sf::Vector2f(RenderState.texture->getSize());
		FrameCount = 1;
		FrameCountY = 1;
	}

	ParticleInstance->Size = Size;

	vertices.clear();
	vertices.resize(Amount * 4);

	for (int i = 0; i < Amount; i++)
	{
		VParticle* particle = new VParticle(*ParticleInstance);
		Add(particle);

		int RandomFrame = RandomFrames ? VGlobal::p()->Random->GetInt((FrameCount * FrameCountY) - 1) : i;
		int FrameX = RandomFrame % FrameCount;
		int FrameY = RandomFrame / FrameCount;

		vertices[0 + (i * 4)].texCoords = Offset + sf::Vector2f(FrameX * Size.x, FrameY * Size.y);
		vertices[1 + (i * 4)].texCoords = Offset + sf::Vector2f((FrameX + 1) * Size.x, FrameY * Size.y);
		vertices[2 + (i * 4)].texCoords = Offset + sf::Vector2f((FrameX + 1) * Size.x, (FrameY + 1) * Size.y);
		vertices[3 + (i * 4)].texCoords = Offset + sf::Vector2f(FrameX * Size.x, (FrameY + 1) * Size.y);
	}

#if _DEBUG
	debuggingVertices.clear();
	debuggingVertices.resize(8);
#endif
}

void VEmitter::Destroy()
{
	VSUPERCLASS::Destroy();

	if (ParticleInstance)
	{
		delete ParticleInstance;
		ParticleInstance = nullptr;
	}

	vertices.clear();
}

void VEmitter::Update(float dt)
{
	if (running)
	{
		if (Explosion)
		{
			for (int i = 0; i < amount; i++)
			{
				EmitParticle();
			}

			running = false;
			willKill = true;
			amount = 0;
		}
		else
		{
			if (Frequency <= 0)
			{
				for (unsigned int i = 0; i < AmountPerEmit; i++) EmitParticle();

				if (!Constant && ++counter >= amount)
				{
					running = false;
					willKill = true;
					amount = 0;
				}
			}
			else
			{
				timer += dt;

				while (timer > Frequency)
				{
					timer -= Frequency;
					for (unsigned int i = 0; i < AmountPerEmit; i++) EmitParticle();

					if (!Constant && ++counter >= amount)
					{
						running = false;
						willKill = true;
						amount = 0;
					}
				}
			}
		}
	}
	else if (willKill)
	{
		timer += dt;

		if (Lifespan.B > 0 && timer > Lifespan.B)
		{
			Kill();
			return;
		}
	}

	VSUPERCLASS::Update(dt);
}

void VEmitter::Draw(sf::RenderTarget& RenderTarget)
{
	VParticle* base;
	sf::Transformable transformable;
	sf::Transform transform;

	sf::View renderTargetView = RenderTarget.getView();
	sf::View scrollView = RenderTarget.getDefaultView();
	
	sf::Vector2f scroll = renderTargetView.getCenter() - scrollView.getCenter();
	float rotate = renderTargetView.getRotation() - scrollView.getRotation();
	float zoom = renderTargetView.getSize().x / scrollView.getSize().x;

	if (ScrollFactor.x != 1 && ScrollFactor.y != 1 && RotateFactor != 1 && ZoomFactor != 1)
	{
		scroll.x *= ScrollFactor.x;
		scroll.y *= ScrollFactor.y;

		rotate *= RotateFactor;

		zoom--;
		zoom *= ZoomFactor;
		zoom++;
	}

	scrollView.move(scroll);
	scrollView.rotate(rotate);
	scrollView.zoom(zoom);
	scrollView.setViewport(renderTargetView.getViewport());

	for (unsigned int i = 0; i < members.size(); i++)
	{
		base = dynamic_cast<VParticle*>(members[i]);

		if (base != nullptr && base->exists && base->visible)
		{
			transformable.setOrigin(base->Size/2.0f);
			transformable.setPosition(base->Position + transformable.getOrigin());
			transformable.setScale(base->Scale);
			transformable.setRotation(base->Angle);
			transform = transformable.getTransform();

			vertices[0 + (i * 4)].position = transform.transformPoint(sf::Vector2f(0,0));
			vertices[1 + (i * 4)].position = transform.transformPoint(sf::Vector2f(base->Size.x, 0));
			vertices[2 + (i * 4)].position = transform.transformPoint(sf::Vector2f(base->Size.x, base->Size.y));
			vertices[3 + (i * 4)].position = transform.transformPoint(sf::Vector2f(0, base->Size.y));

			vertices[0 + (i * 4)].color = base->Tint;
			vertices[1 + (i * 4)].color = base->Tint;
			vertices[2 + (i * 4)].color = base->Tint;
			vertices[3 + (i * 4)].color = base->Tint;
		}
		else
		{
			vertices[0 + (i * 4)].position = Position;
			vertices[1 + (i * 4)].position = Position;
			vertices[2 + (i * 4)].position = Position;
			vertices[3 + (i * 4)].position = Position;
			
			vertices[0 + (i * 4)].color.a = 0;
			vertices[1 + (i * 4)].color.a = 0;
			vertices[2 + (i * 4)].color.a = 0;
			vertices[3 + (i * 4)].color.a = 0;
		}
	}

	sf::FloatRect renderBox = vertices.getBounds();;
	float maxSize = fmaxf(scrollView.getSize().x, scrollView.getSize().y);
	sf::FloatRect scrollBox = sf::FloatRect(scrollView.getCenter() - sf::Vector2f(maxSize, maxSize) / 2.0f, sf::Vector2f(maxSize, maxSize));

	if (renderBox.left < scrollBox.left + scrollBox.width &&
		renderBox.left + renderBox.width > scrollBox.left &&
		renderBox.top <	 scrollBox.top + scrollBox.height &&
		renderBox.top + renderBox.height > scrollBox.top)
	{
		RenderTarget.setView(scrollView);
		RenderTarget.draw(vertices, RenderState);

#ifdef _DEBUG
		if (VGlobal::p()->DrawDebug)
		{
			debuggingVertices[0].position = sf::Vector2f(renderBox.left, renderBox.top);
			debuggingVertices[1].position = sf::Vector2f(renderBox.left + renderBox.width, renderBox.top);
			debuggingVertices[2].position = sf::Vector2f(renderBox.left + renderBox.width, renderBox.top);
			debuggingVertices[3].position = sf::Vector2f(renderBox.left + renderBox.width, renderBox.top + renderBox.height);
			debuggingVertices[4].position = sf::Vector2f(renderBox.left + renderBox.width, renderBox.top + renderBox.height);
			debuggingVertices[5].position = sf::Vector2f(renderBox.left, renderBox.top + renderBox.height);
			debuggingVertices[6].position = sf::Vector2f(renderBox.left, renderBox.top + renderBox.height);
			debuggingVertices[7].position = sf::Vector2f(renderBox.left, renderBox.top);
		}
#endif
		RenderTarget.setView(renderTargetView);
	}
}

void VEmitter::Kill()
{
	running = false;
	VSUPERCLASS::Kill();
}

VEmitter* VEmitter::Start(int Amount)
{
	exists = true;
	visible = true;
	running = true;

	counter = 0;
	timer = 0;

	willKill = false;

	amount = Amount != 0 ? Amount : Length();
	return this;
}

void VEmitter::EmitParticle()
{
	auto particle = static_cast<VParticle*>(FirstAvailable());

	VRandom random;

	if (particle)
	{
		particle->Revive();
		particle->Reset(0, 0);

		particle->Immovable = Immovable;
		particle->AllowCollisions = AllowCollisions;

		if (Circular)
		{
			float angle = random.GetFloat(EmittingAngle.B, EmittingAngle.A) * (3.1415926f / 180);
			float speed = random.GetFloat(SpeedRange.B, SpeedRange.A);
			particle->Velocity = sf::Vector2f(cos(angle), sinf(angle)) * speed;
		}
		else
		{
			particle->Velocity = VGlobal::p()->Random->GetVector2f(VelocityRange.B, VelocityRange.A);
		}

		particle->AngleAcceleration = VGlobal::p()->Random->GetFloat(AngleAccelerationRange.B, AngleAccelerationRange.A);
		particle->AngleDrag = VGlobal::p()->Random->GetFloat(AngleDragRange.B, AngleDragRange.A);
		particle->AngleVelocity = VGlobal::p()->Random->GetFloat(AngleVelocityRange.B, AngleVelocityRange.A);

		particle->Angle = VGlobal::p()->Random->GetFloat(AngleRange.B, AngleRange.A);

		particle->Lifespan = VGlobal::p()->Random->GetFloat(Lifespan.B, Lifespan.A);

		particle->ScaleRange.A = VGlobal::p()->Random->GetVector2f(ScaleRange.A.B, ScaleRange.A.A);
		particle->ScaleRange.B = VGlobal::p()->Random->GetVector2f(ScaleRange.B.B, ScaleRange.B.A);

		particle->ColourRange.A = VGlobal::p()->Random->GetColor(ColourRange.A.B, ColourRange.A.A);
		particle->ColourRange.B = VGlobal::p()->Random->GetColor(ColourRange.B.B, ColourRange.B.A);
		
		particle->AlphaRange.A = VGlobal::p()->Random->GetFloat(AlphaRange.A.B, AlphaRange.A.A);
		particle->AlphaRange.B = VGlobal::p()->Random->GetFloat(AlphaRange.B.B, AlphaRange.B.A);

		particle->Drag = VGlobal::p()->Random->GetVector2f(DragRange.B, DragRange.A);

		particle->Acceleration = VGlobal::p()->Random->GetVector2f(AccelerationRange.B, AccelerationRange.A);

		particle->Elasticity = VGlobal::p()->Random->GetFloat(ElasticityRange.B, ElasticityRange.A);

		particle->Position = VGlobal::p()->Random->GetVector2f(Position + Size, Position) - (Size / 2.0f);

		particle->OnEmit();
	}
}