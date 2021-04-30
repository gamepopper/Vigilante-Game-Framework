#include "VPath.h"
#include <math.h>

sf::Vector2f VPath::getBezierPoint(float t)
{
	tmp.resize(points.size());
	std::copy(points.begin(), points.end(), tmp.begin());

	unsigned int maxi = points.size() - 1;
	for (unsigned int i = 0; i < maxi; i++)
	{
		unsigned int maxj = maxi - i;
		for (unsigned int j = 0; j < maxj; j++)
			tmp[j] += t * (tmp[j + 1] - tmp[j]);
	}

	return tmp[0];
}

void VPath::SetPath(const std::vector<sf::Vector2f>& p)
{
	points.resize(p.size());
	std::copy(p.begin(), p.end(), points.begin());
	dirty = true;
}

void VPath::AddPoint(sf::Vector2f point)
{
	t *= 1.0f / points.size();
	points.push_back(point);
	dirty = true;
}

void VPath::RemovePoint(unsigned int index)
{
	if (index >= points.size())
		return;

	points.erase(points.begin() + index);
	dirty = true;
}

void VPath::ClearPath()
{
	points.clear();
	points.shrink_to_fit();
	tmp.clear();
	tmp.shrink_to_fit();
}

int VPath::GetNumPoints()
{
	return points.size();
}

void VPath::SetPathType(VPathType pathType)
{
	this->pathType = pathType;
	dirty = true;
}

VPath::VPathType VPath::GetPathType()
{
	return (VPath::VPathType)pathType;
}

void VPath::SetSpeed(float speed)
{
	this->speed = speed;
}

float VPath::GetSpeed()
{
	return speed;
}

void VPath::SetUpdateAngle(bool active)
{
	angle = active;
}

void VPath::SetInterpolationType(VInterpolate::VInterpolateType type)
{
	interpolationType = type;
}

VInterpolate::VInterpolateType VPath::GetInterpolationType()
{
	return interpolationType;
}

bool VPath::IsActive()
{
	return active;
}

void VPath::StartFollowing(VObject* object, std::function<void()> onComplete)
{
	if (active || points.size() == 0)
		return;

	this->object = object;
	this->onComplete = onComplete;

	t = 0.0f;

	active = true;
	lastPos = points[0];
}

void VPath::StopFollowing()
{
	active = false;
	object = NULL;
}

#include "VInterpolate.h"
sf::Vector2f VPath::GetPoint(float t)
{
	t = std::min(t, 1.0f);

	switch (pathType)
	{
		case VPath::LINE:
		{
			unsigned int size = points.size() - 1;
			float frac = 1.0f / size;
			unsigned int part = static_cast<int>(t / frac);

			if (part >= size)
				return points.back();

			float partT = t - (part * frac);
			return VInterpolate::Vector2f(points[part], points[part + 1], partT, frac);
		}
		case VPath::CURVE:
		{
			return getBezierPoint(t);
		}
		default:
		{
			return getBezierPoint(0);
		}
	}
}

void VPath::SetPoint(unsigned int index, sf::Vector2f point)
{
	if (index < points.size())
		points[index] = point;
}

void VPath::Update(float dt)
{
	if (!active)
		return;

	t += dt * speed;

	if (t > 1.0f)
	{
		if (object)
			object->SetPositionAtCentre(points[points.size() - 1]);

		StopFollowing();
		t = 1.0f;

		if (onComplete)
			onComplete();

		return;
	}

	sf::Vector2f p1 = GetPoint(VInterpolate::Float(0.0f, 1.0f, t, 1.0f, interpolationType));

	if (object)
	{
		object->SetPositionAtCentre(p1);
		if (angle)
		{
			sf::Vector2f diff = p1 - lastPos;
			object->Angle = atan2f(diff.y, diff.x) * (180 / VFRAME_PI);
		}
	}

	lastPos = p1;
}

void VPath::Draw(sf::RenderTarget& RenderTarget)
{
	if (GetNumPoints() < 2)
		return;

	if (dirty)
	{
		int points = 5 * GetNumPoints();
		float thickness = 5.0f / 2.0f;
		float dt = 0.0f;
		float step = 1.0f / points;

		vertexArray.setPrimitiveType(sf::TrianglesStrip);
		vertexArray.clear();
		while (dt < 1.0f)
		{
			sf::Vector2f p1 = GetPoint(dt);
			sf::Vector2f p2 = GetPoint(dt + step);

			sf::Vector2f diff = p2 - p1;
			diff /= sqrtf((diff.x * diff.x) + (diff.y * diff.y));
			sf::Vector2f norm(-diff.y, diff.x);

			vertexArray.append(sf::Vertex(p1 + (norm * thickness), sf::Color::White));
			vertexArray.append(sf::Vertex(p1 + (norm * -thickness), sf::Color::White));

			dt += step;
		}

		sf::Vector2f p1 = GetPoint(dt - step);
		sf::Vector2f p2 = GetPoint(dt);

		sf::Vector2f diff = p2 - p1;
		diff /= sqrtf((diff.x * diff.x) + (diff.y * diff.y));
		sf::Vector2f norm(-diff.y, diff.x);

		vertexArray.append(sf::Vertex(p2 + (norm * thickness), sf::Color::White));
		vertexArray.append(sf::Vertex(p2 + (norm * -thickness), sf::Color::White));
	}

	RenderTarget.draw(vertexArray);
}
