#include "Gemstone.h"
#include "../VFrame/VGlobal.h"
#include "../VFrame/VRandom.h"

using std::vector;

bool sortByY(sf::Vector2f i, sf::Vector2f j) { return i.y < j.y; }

void Gemstone::updateTransform()
{
	//S * R * T
	glTranslatef(Position.x, Position.y, Position.z);
	glRotatef(Rotation.z, 0.0f, 0.0f, 1.0f);
	glRotatef(Rotation.x, 1.0f, 0.0f, 0.0f);
	glRotatef(Rotation.y, 0.0f, 1.0f, 0.0f);
	glScalef(Scale.x * scale.x, Scale.y * scale.y, Scale.z * scale.z);
}

sf::Vector3f Gemstone::rotateYVector3f(sf::Vector3f Point, float Angle, sf::Vector3f Origin)
{
	sf::Vector3f dir = Point - Origin;

	sf::Vector3f result;
	result.z = (dir.z * cosf(Angle)) - (dir.x * sinf(Angle));
	result.x = (dir.z * sinf(Angle)) + (dir.x * cosf(Angle));
	result.y = dir.y;

	Point = result + Origin;
	return Point;
}

sf::Vector3f Gemstone::reflectVector3f(sf::Vector3f incident, sf::Vector3f normal)
{
	return incident - 2.0f * dotVector3f(incident, normal) * normal;
}

sf::Vector3f Gemstone::crossVector3f(sf::Vector3f a, sf::Vector3f b)
{
	sf::Vector3f crossProduct = sf::Vector3f((a.y * b.z) - (a.z * b.y), (a.z * b.x) - (a.x * b.z), (a.x * b.y) - (a.y * b.x));
	float length = sqrtf((crossProduct.x * crossProduct.x) + (crossProduct.y * crossProduct.y) + (crossProduct.z * crossProduct.z));
	return sf::Vector3f(crossProduct.x / length, crossProduct.y / length, crossProduct.z / length);
}

float Gemstone::dotVector3f(sf::Vector3f a, sf::Vector3f b)
{
	return (a.x * b.x) + (a.y * b.y) + (a.z * b.z);
}

bool Gemstone::Generate(std::vector<sf::Vector2f> &coordinates, float symmetry, sf::Vector3f &scale, float slice, float angleOffset, unsigned int seed, float randomVertVar, float rotateY)
{
	if (coordinates.size() < 1)
		return false;

	if (symmetry < 1)
		return false;

	int twoSymmetry = static_cast<int>(symmetry * 2);

	vector<sf::Vector3f> gemVerts;
	vector<sf::Vector2f> gemTex;

	VRandom random = VRandom(seed);

	//Sort coordinates by order of Y
	std::sort(coordinates.begin(), coordinates.end(), sortByY);

	this->scale = scale / 2.0f;

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
				sf::Vector3f currentCoord = sf::Vector3f(coordinates[i].x + random.GetFloat(randomVertVar, -randomVertVar), coordinates[i].y + random.GetFloat(randomVertVar, -randomVertVar), 0);
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
				sf::Vector3f currentCoord = sf::Vector3f(coordinates[i].x, coordinates[i].y, 0);
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

	int indexCount = 0;
	for (int s = 0; s < twoSymmetry; s++)
	{
		int nextS = (s + 1) % twoSymmetry;
		int FirstCoord1stSym = ((newVerts.size()) / (twoSymmetry)) * s;
		int FirstCoord2ndSym = ((newVerts.size()) / (twoSymmetry)) * nextS;

		gemVerts.push_back(newVerts[FirstCoord1stSym]);
		gemVerts.push_back(newVerts[FirstCoord1stSym + 1]);
		gemVerts.push_back(newVerts[FirstCoord2ndSym + 1]);

		gemTex.push_back(newTex[FirstCoord1stSym]);
		gemTex.push_back(newTex[FirstCoord1stSym + 1]);
		gemTex.push_back(newTex[FirstCoord2ndSym + 1]);

		for (unsigned int i = 1; i < coordinates.size(); i++)
		{
			gemVerts.push_back(newVerts[FirstCoord1stSym + i]);
			gemVerts.push_back(newVerts[FirstCoord1stSym + i + 1]);
			gemVerts.push_back(newVerts[FirstCoord2ndSym + i + 1]);
			gemVerts.push_back(newVerts[FirstCoord1stSym + i]);
			gemVerts.push_back(newVerts[FirstCoord2ndSym + i + 1]);
			gemVerts.push_back(newVerts[FirstCoord2ndSym + i]);

			gemTex.push_back(newTex[FirstCoord1stSym + i]);
			gemTex.push_back(newTex[FirstCoord1stSym + i + 1]);
			gemTex.push_back(newTex[FirstCoord2ndSym + i + 1]);
			gemTex.push_back(newTex[FirstCoord1stSym + i]);
			gemTex.push_back(newTex[FirstCoord2ndSym + i + 1]);
			gemTex.push_back(newTex[FirstCoord2ndSym + i]);
		}

		int lastCoord = coordinates.size();

		gemVerts.push_back(newVerts[FirstCoord1stSym + lastCoord]);
		gemVerts.push_back(newVerts[FirstCoord2ndSym + lastCoord + 1]);
		gemVerts.push_back(newVerts[FirstCoord2ndSym + lastCoord]);

		gemTex.push_back(newTex[FirstCoord1stSym + lastCoord]);
		gemTex.push_back(newTex[FirstCoord2ndSym + lastCoord + 1]);
		gemTex.push_back(newTex[FirstCoord2ndSym + lastCoord]);
	}

	std::vector<GLfloat> data;
	data.reserve(gemVerts.size() * 8);

	for (unsigned int i = 0; i < gemVerts.size(); i += 3)
	{
		sf::Vector3f vert0 = gemVerts[i + 0];
		sf::Vector3f vert1 = gemVerts[i + 1];
		sf::Vector3f vert2 = gemVerts[i + 2];

		sf::Vector2f tex0 = gemTex[i + 0];
		sf::Vector2f tex1 = gemTex[i + 1];
		sf::Vector2f tex2 = gemTex[i + 2];

		sf::Vector3f vector1 = vert1 - vert0;
		sf::Vector3f vector2 = vert2 - vert1;
		sf::Vector3f normal = crossVector3f(vector2, vector1);

		data.push_back(vert0.x);	data.push_back(vert0.y);	data.push_back(vert0.z);
		data.push_back(normal.x);	data.push_back(normal.y);	data.push_back(normal.z);
		data.push_back(tex0.x);		data.push_back(tex0.y);

		data.push_back(vert1.x);	data.push_back(vert1.y);	data.push_back(vert1.z);
		data.push_back(normal.x);	data.push_back(normal.y);	data.push_back(normal.z);
		data.push_back(tex1.x);		data.push_back(tex1.y);

		data.push_back(vert2.x);	data.push_back(vert2.y);	data.push_back(vert2.z);
		data.push_back(normal.x);	data.push_back(normal.y);	data.push_back(normal.z);
		data.push_back(tex2.x);		data.push_back(tex2.y);
	}

	LoadModelData(data, 0, 3, 6);

	return true;
}