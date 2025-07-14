#include "VEmitter.h"
#include "VGlobal.h"

VEmitter* VEmitter::LoadParticlesFromFile(int Amount, const sf::String& Filename, bool Animated, unsigned int Width, unsigned int Height, const sf::IntRect& Rect, bool RandomFrames)
{
	Clear(true);

	MaxSize = Amount;
	RenderState.texture = &VGlobal::p()->Content->LoadTexture(Filename);
	setSize(Amount, Animated, Width, Height, Rect, RandomFrames);

	return this;
}

VEmitter* VEmitter::LoadParticles(int Amount, sf::Texture& Texture, bool Animated, unsigned int Width, unsigned int Height, const sf::IntRect& Rect, bool RandomFrames)
{
	Clear(true);

	MaxSize = Amount;
	RenderState.texture = &Texture;
	setSize(Amount, Animated, Width, Height, Rect, RandomFrames);

	return this;
}

VEmitter* VEmitter::MakeParticles(int Amount, unsigned int Width, unsigned int Height, sf::Color Color)
{
	Clear(true);

	MaxSize = Amount;

	if (disposible)
	{
		delete RenderState.texture;
		RenderState.texture = nullptr;
	}

	sf::Image image;
	image.resize({ Width, Height }, Color);

	sf::Texture* tex = new sf::Texture();
	disposible |= tex->loadFromImage(image);

	RenderState.texture = tex;
	setSize(Amount, false, Width, Height, sf::IntRect(sf::Vector2i(), sf::Vector2i(Width, Height)), false);

	return this;
}

void VEmitter::setSize(int Amount, bool Animated, unsigned int Width, unsigned int Height, const sf::IntRect& Rect, bool RandomFrames)
{
	sf::Vector2f Size;
	int FrameCount;
	int FrameCountY;
	int TextureWidth = Rect.size.x == 0 ? RenderState.texture->getSize().x : Rect.size.x;
	sf::Vector2f Offset = sf::Vector2f((float)Rect.position.x, (float)Rect.position.y);

	if (Animated)
	{
		Size = sf::Vector2f(static_cast<float>(Width), static_cast<float>(Height));
		FrameCount = TextureWidth / Width;
		FrameCountY = RenderState.texture->getSize().y / Height;
	}
	else
	{
		sf::Vector2u texRect = RenderState.texture->getSize();
		Size = sf::Vector2f(sf::Vector2i(Rect.size.x * (Rect.size.x > 0) + texRect.x * (Rect.size.x == 0), Rect.size.y * (Rect.size.y > 0) + texRect.y * (Rect.size.y == 0)));

		FrameCount = 1;
		FrameCountY = 1;
	}

	ParticleInstance->Size = Size;

	vertices.clear();
	vertices.resize(Amount * 6);

	for (int i = 0; i < Amount; ++i)
	{
		VParticle* particle = new VParticle(*ParticleInstance);
		Add(particle);

		int RandomFrame = RandomFrames ? VGlobal::p()->Random->GetInt((FrameCount * FrameCountY) - 1) : i % (FrameCount * FrameCountY);
		int FrameX = RandomFrame % FrameCount;
		int FrameY = RandomFrame / FrameCount;

		vertices[0 + (i * 6)].texCoords = Offset + sf::Vector2f(FrameX * Size.x, FrameY * Size.y);
		vertices[1 + (i * 6)].texCoords = Offset + sf::Vector2f((FrameX + 1) * Size.x, FrameY * Size.y);
		vertices[2 + (i * 6)].texCoords = Offset + sf::Vector2f((FrameX + 1) * Size.x, (FrameY + 1) * Size.y);
		vertices[3 + (i * 6)].texCoords = Offset + sf::Vector2f((FrameX + 1) * Size.x, (FrameY + 1) * Size.y);
		vertices[4 + (i * 6)].texCoords = Offset + sf::Vector2f(FrameX * Size.x, (FrameY + 1) * Size.y);
		vertices[5 + (i * 6)].texCoords = Offset + sf::Vector2f(FrameX * Size.x, FrameY * Size.y);
	}
}

void VEmitter::Destroy()
{
	VSUPERCLASS::Destroy();

	if (disposible)
	{
		delete RenderState.texture;
		RenderState.texture = nullptr;
	}

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
			for (int i = 0; i < amount; ++i)
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
				for (unsigned int i = 0; i < AmountPerEmit; ++i) EmitParticle();

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
					for (unsigned int i = 0; i < AmountPerEmit; ++i) EmitParticle();

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
	sf::Angle rotate = renderTargetView.getRotation() - scrollView.getRotation();
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

	for (unsigned int i = 0; i < members.size(); ++i)
	{
		base = dynamic_cast<VParticle*>(members[i]);

		if (base != nullptr && base->exists && base->visible)
		{
			transformable.setOrigin(base->Size/2.0f);
			transformable.setPosition(base->Position + transformable.getOrigin());
			transformable.setScale(base->Scale);
			transformable.setRotation(base->Angle);
			transform = transformable.getTransform();

			vertices[0 + (i * 6)].position = transform.transformPoint(sf::Vector2f(0,0));
			vertices[1 + (i * 6)].position = transform.transformPoint(sf::Vector2f(base->Size.x, 0));
			vertices[2 + (i * 6)].position = transform.transformPoint(sf::Vector2f(base->Size.x, base->Size.y));
			vertices[3 + (i * 6)].position = transform.transformPoint(sf::Vector2f(base->Size.x, base->Size.y));
			vertices[4 + (i * 6)].position = transform.transformPoint(sf::Vector2f(0, base->Size.y));
			vertices[5 + (i * 6)].position = transform.transformPoint(sf::Vector2f(0, 0));

			vertices[0 + (i * 6)].color = base->Tint;
			vertices[1 + (i * 6)].color = base->Tint;
			vertices[2 + (i * 6)].color = base->Tint;
			vertices[3 + (i * 6)].color = base->Tint;
			vertices[4 + (i * 6)].color = base->Tint;
			vertices[5 + (i * 6)].color = base->Tint;
		}
		else
		{
			vertices[0 + (i * 6)].position = Position;
			vertices[1 + (i * 6)].position = Position;
			vertices[2 + (i * 6)].position = Position;
			vertices[3 + (i * 6)].position = Position;
			vertices[4 + (i * 6)].position = Position;
			vertices[5 + (i * 6)].position = Position;

			vertices[0 + (i * 6)].color.a = 0;
			vertices[1 + (i * 6)].color.a = 0;
			vertices[2 + (i * 6)].color.a = 0;
			vertices[3 + (i * 6)].color.a = 0;
			vertices[4 + (i * 6)].color.a = 0;
			vertices[5 + (i * 6)].color.a = 0;
		}
	}

	sf::FloatRect renderBox = vertices.getBounds();;
	float maxSize = fmaxf(scrollView.getSize().x, scrollView.getSize().y);
	sf::FloatRect scrollBox = sf::FloatRect(scrollView.getCenter() - sf::Vector2f(maxSize, maxSize) / 2.0f, sf::Vector2f(maxSize, maxSize));

	if (renderBox.position.x < scrollBox.position.x + scrollBox.size.x &&
		renderBox.position.x + renderBox.size.x > scrollBox.position.x &&
		renderBox.position.y <	 scrollBox.position.y + scrollBox.size.y &&
		renderBox.position.y + renderBox.size.y > scrollBox.position.y)
	{
		RenderTarget.setView(scrollView);
		RenderTarget.draw(vertices, RenderState);
		RenderTarget.setView(renderTargetView);
	}
}

void VEmitter::Kill()
{
	running = false;
	VSUPERCLASS::Kill();
}

void VEmitter::Revive()
{
	running = false;
	VSUPERCLASS::Revive();
}

VEmitter* VEmitter::Start(int Amount)
{
	if (!running)
	{
		exists = true;
		visible = true;
		running = true;

		counter = 0;
		timer = 0;

		willKill = false;

		amount = Amount != 0 ? Amount : Length();
	}

	return this;
}

void VEmitter::Stop()
{
	running = false;
}

bool VEmitter::IsRunning()
{
	return running;
}

void VEmitter::EmitParticle()
{
	auto particle = static_cast<VParticle*>(FirstAvailable());

	VRandom random((unsigned int)((uint64_t)particle) + (unsigned int)(time(NULL)));

	if (particle)
	{
		particle->Revive();
		particle->Reset(0, 0);

		particle->Immovable = Immovable;
		particle->AllowCollisions = AllowCollisions;

		if (Circular)
		{
			float angle = random.GetFloat(EmittingAngle.B, EmittingAngle.A) * (VFRAME_PI / 180);
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

		particle->Angle = sf::degrees(VGlobal::p()->Random->GetFloat(AngleRange.B, AngleRange.A));

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

		particle->Position = VGlobal::p()->Random->GetVector2f(Position + (Size / 2.0f), Position - (Size / 2.0f));

		particle->OnEmit();
	}
}
