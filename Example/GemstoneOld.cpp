#include "GemstoneOld.h"
#include "../VFrame/VGlobal.h"
#include "../VFrame/VRandom.h"

using std::vector;

void GemstoneOld::updateTransform()
{
	transformable.setPosition(Position);
	transformable.setRotation(Angle);
}

sf::Vector3f GemstoneOld::scaleVector3f(sf::Vector3f Point, sf::Vector3f Scale)
{
	Point.x *= Scale.x;
	Point.y *= Scale.y;
	Point.z *= Scale.y;

	return Point;
}

sf::Vector3f GemstoneOld::rotateYVector3f(sf::Vector3f Point, float Angle, sf::Vector3f Origin)
{
	sf::Vector3f dir = Point - Origin;

	sf::Vector3f result;
	result.z = (dir.z * cosf(Angle)) - (dir.x * sinf(Angle));
	result.x = (dir.z * sinf(Angle)) + (dir.x * cosf(Angle));
	result.y = dir.y;

	Point = result + Origin;
	return Point;
}

sf::Vector3f GemstoneOld::reflectVector3f(sf::Vector3f incident, sf::Vector3f normal)
{
	return incident - 2.0f * dotVector3f(incident, normal) * normal;
}

sf::Vector3f GemstoneOld::crossVector3f(sf::Vector3f a, sf::Vector3f b)
{
	sf::Vector3f crossProduct = sf::Vector3f((a.y * b.z) - (a.z * b.y), (a.z * b.x) - (a.x * b.z), (a.x * b.y) - (a.y * b.x));
	float length = sqrtf((crossProduct.x * crossProduct.x) + (crossProduct.y * crossProduct.y) + (crossProduct.z * crossProduct.z));
	return sf::Vector3f(crossProduct.x / length, crossProduct.y / length, crossProduct.z / length);
}

float GemstoneOld::dotVector3f(sf::Vector3f a, sf::Vector3f b)
{
	return (a.x * b.x) + (a.y * b.y) + (a.z * b.z);
}

void GemstoneOld::LoadTexture(sf::String filename)
{
	VGlobal::p()->Content->LoadTexture(filename, texture);
}

void GemstoneOld::LoadTexture(sf::Texture &texture)
{
	this->texture = texture;
}

bool GemstoneOld::Generate(std::vector<sf::Vector2f> &coordinates, float symmetry, sf::Vector3f &scale, float slice, float angleOffset, unsigned int seed, float randomVertVar, float rotateY)
{
	if (coordinates.size() < 1)
		return false;

	if (symmetry < 1)
		return false;

	int twoSymmetry = static_cast<int>(symmetry * 2);

	vector<sf::Vector3f> gemVerts;
	vector<sf::Vector2f> gemTex;

	VRandom random = VRandom(seed);
	std::vector<sf::Vector2f> coords(coordinates.size());
	for (unsigned int i = 0; i < coordinates.size(); i++)
	{
		coords[i] = coordinates[i] + random.GetVector2f(sf::Vector2f(randomVertVar, randomVertVar), -sf::Vector2f(randomVertVar, randomVertVar));
	}

	//Sort coordinates by order of Y

	float anglePerSym = (2 * 3.1415926f) / twoSymmetry;

	sf::Vector3f vertTop = sf::Vector3f(0, -1, 0);
	sf::Vector3f vertBottom = sf::Vector3f(0, 1, 0);

	vector<sf::Vector3f> newVerts;
	vector<sf::Vector2f> newTex;

	for (int s = 0; s < twoSymmetry; s++)
	{
		float sectionX = 1.0f / (twoSymmetry - 1);
		float currentAngle = (s * anglePerSym) + angleOffset;

		if (slice <= 0)
		{
			newVerts.push_back(vertTop);
			newTex.push_back(sf::Vector2f(0.5f, 0.0f));

			for (unsigned int i = 0; i < coordinates.size(); i++)
			{
				sf::Vector3f currentCoord = sf::Vector3f(coords[i].x, coords[i].y, 0);
				sf::Vector3f transformedVerts = rotateYVector3f(currentCoord, currentAngle, sf::Vector3f());
				sf::Vector2f transformedTexs = sf::Vector2f(sectionX * s, (coordinates[i].y + 1) / 2.0f);

				newVerts.push_back(transformedVerts);
				newTex.push_back(transformedTexs);
			}

			newVerts.push_back(vertBottom);
			newTex.push_back(sf::Vector2f(0.5f, 1.0f));
		}
		else if (slice <= 1)
		{
			newVerts.push_back(vertTop);
			newTex.push_back(sf::Vector2f(0.5f, 0.0f));

			for (unsigned int i = 0; i < coordinates.size(); i++)
			{
				sf::Vector3f currentCoord = sf::Vector3f(coordinates[i].x + random.GetFloat(randomVertVar, -randomVertVar), coordinates[i].y + random.GetFloat(randomVertVar, -randomVertVar), 0);
				sf::Vector3f transformedVerts = rotateYVector3f(currentCoord, currentAngle - (anglePerSym * slice * 0.5f), sf::Vector3f());
				sf::Vector2f transformedTexs = sf::Vector2f(sectionX * s - (sectionX * slice * 0.5f), (coordinates[i].y + 1) / 2.0f);

				newVerts.push_back(transformedVerts);
				newTex.push_back(transformedTexs);
			}

			newVerts.push_back(vertBottom);
			newTex.push_back(sf::Vector2f(0.5f, 1.0f));

			newVerts.push_back(vertTop);
			newTex.push_back(sf::Vector2f(0.5f, 0.0f));

			for (unsigned int i = 0; i < coordinates.size(); i++)
			{
				sf::Vector3f currentCoord = sf::Vector3f(coordinates[i].x, coordinates[i].y, 0);
				sf::Vector3f transformedVerts = rotateYVector3f(currentCoord, currentAngle + (anglePerSym * slice * 0.5f), sf::Vector3f());
				sf::Vector2f transformedTexs = sf::Vector2f(sectionX * s + (sectionX * slice * 0.5f), (coordinates[i].y + 1) / 2.0f);

				newVerts.push_back(transformedVerts);
				newTex.push_back(transformedTexs);
			}

			newVerts.push_back(vertBottom);
			newTex.push_back(sf::Vector2f(0.5f, 1.0f));
		}
		else
		{
			return false;
		}
	}

	if (slice > 0 && slice <= 1)
		twoSymmetry *= 2;

	scale.y *= -1;

	int indexCount = 0;
	for (int s = 0; s < twoSymmetry; s++)
	{
		int nextS = (s + twoSymmetry + 1) % twoSymmetry;
		int FirstCoord1stSym = ((newVerts.size()) / (twoSymmetry)) * s;
		int FirstCoord2ndSym = ((newVerts.size()) / (twoSymmetry)) * nextS;

		gemVerts.push_back(scaleVector3f(newVerts[FirstCoord1stSym],					scale / 2.0f));
		gemVerts.push_back(scaleVector3f(newVerts[FirstCoord1stSym + 1],				scale / 2.0f));
		gemVerts.push_back(scaleVector3f(newVerts[FirstCoord2ndSym + 1],				scale / 2.0f));

		gemTex.push_back(newTex[FirstCoord1stSym]);
		gemTex.push_back(newTex[FirstCoord1stSym + 1]);
		gemTex.push_back(newTex[FirstCoord2ndSym + 1]);

		for (unsigned int i = 1; i < coordinates.size(); i++)
		{
			gemVerts.push_back(scaleVector3f(newVerts[FirstCoord1stSym + i],			scale / 2.0f));
			gemVerts.push_back(scaleVector3f(newVerts[FirstCoord1stSym + i + 1],		scale / 2.0f));
			gemVerts.push_back(scaleVector3f(newVerts[FirstCoord2ndSym + i + 1],		scale / 2.0f));
			gemVerts.push_back(scaleVector3f(newVerts[FirstCoord1stSym + i],			scale / 2.0f));
			gemVerts.push_back(scaleVector3f(newVerts[FirstCoord2ndSym + i + 1],		scale / 2.0f));
			gemVerts.push_back(scaleVector3f(newVerts[FirstCoord2ndSym + i],			scale / 2.0f));

			gemTex.push_back(newTex[FirstCoord1stSym + i]);
			gemTex.push_back(newTex[FirstCoord1stSym + i + 1]);
			gemTex.push_back(newTex[FirstCoord2ndSym + i + 1]);
			gemTex.push_back(newTex[FirstCoord1stSym + i]);
			gemTex.push_back(newTex[FirstCoord2ndSym + i + 1]);
			gemTex.push_back(newTex[FirstCoord2ndSym + i]);
		}

		int lastCoord = coordinates.size();

		gemVerts.push_back(scaleVector3f(newVerts[FirstCoord1stSym + lastCoord],		scale / 2.0f));
		gemVerts.push_back(scaleVector3f(newVerts[FirstCoord2ndSym + lastCoord + 1],	scale / 2.0f));
		gemVerts.push_back(scaleVector3f(newVerts[FirstCoord2ndSym + lastCoord],		scale / 2.0f));

		gemTex.push_back(newTex[FirstCoord1stSym + lastCoord]);
		gemTex.push_back(newTex[FirstCoord2ndSym + lastCoord + 1]);
		gemTex.push_back(newTex[FirstCoord2ndSym + lastCoord]);
	}

	scale.y *= -1;

	Size = sf::Vector2f(scale.x, scale.y);

	if (vertices.getVertexCount() < gemVerts.size())
		vertices.resize(gemVerts.size());

	for (unsigned int i = 0; i < gemVerts.size(); i += 3)
	{
		sf::Vector3f vert0 = rotateYVector3f(gemVerts[i + 0], rotateY, sf::Vector3f());
		sf::Vector3f vert1 = rotateYVector3f(gemVerts[i + 1], rotateY, sf::Vector3f());
		sf::Vector3f vert2 = rotateYVector3f(gemVerts[i + 2], rotateY, sf::Vector3f());

		sf::Vector3f vector1 = vert1 - vert0;
		sf::Vector3f vector2 = vert2 - vert1;
		sf::Vector3f normal = crossVector3f(vector1, vector2);
		sf::Vector3f cameraDir = sf::Vector3f(0.0f, 0.0f, -1.0f);

		if (dotVector3f(cameraDir, normal) > 0.0f)
		{
			sf::Color diffuse = sf::Color(90, 90, 90, 180);
			sf::Color specular = sf::Color(220, 220, 220, 255);

			sf::Vector3f lightDir = sf::Vector3f(0.5f, -0.5f, -0.5f);
			float diff = dotVector3f(lightDir, normal);

			if (diff > 0.0f)
			{
				sf::Vector3f v = reflectVector3f(-lightDir, normal);
				float spec = powf(fmaxf(dotVector3f(v, sf::Vector3f(0, 0, 1)), 0.0f), 0.7f);

				specular.r = sf::Uint8(specular.r * spec);
				specular.g = sf::Uint8(specular.g * spec);
				specular.b = sf::Uint8(specular.b * spec);
				diffuse.r = sf::Uint8(diffuse.r * diff);
				diffuse.g = sf::Uint8(diffuse.g * diff);
				diffuse.b = sf::Uint8(diffuse.b * diff);
			}

			vertices[i + 0].position = (Size / 2.0f) + sf::Vector2f(vert0.x, vert0.y);
			vertices[i + 0].texCoords = sf::Vector2f(
				gemTex[i + 0].x * texture.getSize().x, 
				gemTex[i + 0].y * texture.getSize().y);
			vertices[i + 0].color = sf::Color(
				(ambient.r + diffuse.r + specular.r) / 3, 
				(ambient.g + diffuse.g + specular.g) / 3,
				(ambient.b + diffuse.b + specular.b) / 3,
				ambient.a);

			vertices[i + 1].position = (Size / 2.0f) + sf::Vector2f(vert1.x, vert1.y);
			vertices[i + 1].texCoords = sf::Vector2f(
				gemTex[i + 1].x * texture.getSize().x,
				gemTex[i + 1].y * texture.getSize().y);
			vertices[i + 1].color = sf::Color(
				(ambient.r + diffuse.r + specular.r) / 3,
				(ambient.g + diffuse.g + specular.g) / 3,
				(ambient.b + diffuse.b + specular.b) / 3,
				ambient.a);

			vertices[i + 2].position = (Size / 2.0f) + sf::Vector2f(vert2.x, vert2.y);
			vertices[i + 2].texCoords = sf::Vector2f(
				gemTex[i + 2].x * texture.getSize().x,
				gemTex[i + 2].y * texture.getSize().y);
			vertices[i + 2].color = sf::Color(
				(ambient.r + diffuse.r + specular.r) / 3,
				(ambient.g + diffuse.g + specular.g) / 3,
				(ambient.b + diffuse.b + specular.b) / 3,
				ambient.a);
		}
		else
		{
			vertices[i + 0].position = sf::Vector2f();
			vertices[i + 0].texCoords = sf::Vector2f();
			vertices[i + 0].color = sf::Color::Transparent;

			vertices[i + 1].position = sf::Vector2f();
			vertices[i + 1].texCoords = sf::Vector2f();
			vertices[i + 1].color = sf::Color::Transparent;

			vertices[i + 2].position = sf::Vector2f();
			vertices[i + 2].texCoords = sf::Vector2f();
			vertices[i + 2].color = sf::Color::Transparent;
		}
	}

	return true;
}

void GemstoneOld::Update(float dt)
{
	VSUPERCLASS::Update(dt);
}

void GemstoneOld::Draw(sf::RenderTarget& RenderTarget)
{
	VSUPERCLASS::Draw(RenderTarget);

	sf::View renderTargetView = RenderTarget.getView();
	sf::View scrollView = RenderTarget.getDefaultView();

	sf::Vector2f scroll = renderTargetView.getCenter() - scrollView.getCenter();
	scroll.x *= ScrollFactor.x;
	scroll.y *= ScrollFactor.y;

	float rotate = renderTargetView.getRotation() - scrollView.getRotation();
	rotate *= RotateFactor;

	float zoom = renderTargetView.getSize().x / scrollView.getSize().x;
	zoom--;
	zoom *= ZoomFactor;
	zoom++;

	scrollView.move(scroll);
	scrollView.rotate(rotate);
	scrollView.zoom(zoom);

	sf::FloatRect renderBox = sf::FloatRect(Position, Size);
	float maxSize = fmaxf(scrollView.getSize().x, scrollView.getSize().y);
	sf::FloatRect scrollBox = sf::FloatRect(scrollView.getCenter() - sf::Vector2f(maxSize, maxSize) / 2.0f, sf::Vector2f(maxSize, maxSize));

	if (renderBox.left < scrollBox.left + scrollBox.width &&
		renderBox.left + renderBox.width > scrollBox.left &&
		renderBox.top <	 scrollBox.top + scrollBox.height &&
		renderBox.top + renderBox.height > scrollBox.top)
	{
		RenderTarget.setView(scrollView);

		sf::RenderStates states = sf::RenderStates(RenderState);
		states.texture = &texture;
		states.transform *= transformable.getTransform();
		RenderTarget.draw(vertices, states);
		RenderTarget.setView(renderTargetView);
	}
}
