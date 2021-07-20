#include "VCandle.h"

#ifndef VFRAME_NO_CANDLE
#include <Candle/Candle.hpp>

#include "VTilemap.h"
#include "VTile.h"
#include "VGlobal.h"

struct VEdgeList
{
	candle::EdgeVector edges;

	~VEdgeList()
	{
		edges.clear();
		edges.shrink_to_fit();
	}
};

VLight::VLight(bool radial)
{
	if (radial)
		light = std::make_unique<candle::RadialLight>();
	else
		light = std::make_unique<candle::DirectedLight>();
}

void VLight::SetPosition(const sf::Vector2f& pos)
{
	light->setPosition(pos);
}

void VLight::SetAngle(float angle)
{
	candle::DirectedLight* l = dynamic_cast<candle::DirectedLight*>(light.get());
	if (l) l->setRotation(angle);
}

void VLight::SetWidth(float width)
{
	candle::DirectedLight* l = dynamic_cast<candle::DirectedLight*>(light.get());
	if (l) l->setBeamWidth(width);
}

void VLight::SetRange(float range)
{
	light->setRange(range);
}

void VLight::SetFade(float fade)
{
	light->setFade(fade);
}

void VLight::SetIntensity(float intensity)
{
	light->setIntensity(intensity);
}

void VLight::SetTint(const sf::Color& tint)
{
	light->setColor(tint);
}

sf::Vector2f VLight::GetPosition()
{
	return light->getPosition();
}

float VLight::GetAngle()
{
	candle::DirectedLight* l = dynamic_cast<candle::DirectedLight*>(light.get());
	if (l) return l->getRotation();

	return 0.0f;
}

float VLight::GetWidth()
{
	candle::DirectedLight* l = dynamic_cast<candle::DirectedLight*>(light.get());
	if (l) return l->getBeamWidth();

	return 0.0f;
}

float VLight::GetRange()
{
	return light->getRange();
}

float VLight::GetFade()
{
	return light->getFade();
}

float VLight::GetIntensity()
{
	return light->getIntensity();
}

sf::Color VLight::GetTint()
{
	return light->getColor();
}

void VLight::Cast(const VEdgeList* edgeData)
{
	candle::EdgeVector& edges = const_cast<VEdgeList*>(edgeData)->edges;
	light->castLight(edges.begin(), edges.end());
}

const candle::LightSource* VLight::Source()
{
	return light.get();
}

#include <Candle/geometry/Polygon.hpp>
unsigned int VCandle::numberOfEdges(VBase* b)
{
	unsigned int numberEdges = 0;

	VTilemap* tilemap = nullptr;
	VGroup* group = nullptr;
	if (dynamic_cast<VEdge*>(b))
		numberEdges = 1;
	else if (tilemap = dynamic_cast<VTilemap*>(b))
		numberEdges = 4 * tilemap->GetCollisionTileCount();
	else if (group = dynamic_cast<VGroup*>(b))
	{
		for (int i = 0; i < group->Length(); i++)
			numberEdges += numberOfEdges(group->GetGroupItem(i));
	}
	else if (dynamic_cast<VObject*>(b))
		numberEdges = 4;

	return numberEdges;
}

void VCandle::applyEdge(VBase* b, unsigned int &i)
{
	VObject* o = dynamic_cast<VObject*>(b);
	if (o)
	{
		VTilemap* tilemap = nullptr;
		if (tilemap = dynamic_cast<VTilemap*>(b))
		{
			unsigned int num = tilemap->GetCollisionTileCount();
			for (unsigned int j = 0; j < num; j++)
			{
				VBase* tile = const_cast<VTile*>(tilemap->GetCollisionTile(j));
				applyEdge(tile, i);
			}
		}
		else if (dynamic_cast<VEdge*>(b))
		{
			edgeList->edges[i++] = sfu::Line(o->Position, o->Position + o->Size);
		}
		else
		{
			sf::Vector2f points[] = 
			{
				o->Position,
				o->Position,
				o->Position,
				o->Position,
			};

			points[1].y += o->Size.y;
			points[3].x += o->Size.x;
			points[2] += o->Size;

			sfu::Polygon p(points, 4);
			for (auto& l : p.lines) {
				edgeList->edges[i++] = l;
			}
		}
	}
	else
	{
		VGroup* group = nullptr;
		if (group = dynamic_cast<VGroup*>(b))
		{
			for (int j = 0; j < group->Length(); j++)
				applyEdge(group->GetGroupItem(j), i);
		}
	}
}

VLight* VCandle::AddDirectedLight()
{
	lights.push_back(new VLight(false));
	return lights.back();
}

VLight* VCandle::AddRadialLight()
{
	lights.push_back(new VLight(true));
	return lights.back();
}

void VCandle::RemoveLight(VLight* light, bool destroy)
{
	auto pos = std::find(lights.begin(), lights.end(), light);

	if (destroy)
		delete *pos;

	if (pos != lights.end())
		lights.erase(pos);
}

void VCandle::AddEdgesFromObjects(VBase* o)
{
	unsigned int edgeCount = numberOfEdges(o);

	if (edgeCount > 0)
	{
		if (edgeList == nullptr)
			edgeList = std::make_unique<VEdgeList>();

		members.push_back(o);
		edgeList->edges.reserve(edgeList->edges.size() + edgeCount);
		for (unsigned int i = 0; i < edgeCount; i++)
		{
			edgeList->edges.emplace_back(sf::Vector2f(), sf::Vector2f());
		}
	}
	else
	{
		VBase::VLogError("Object added with no edges!");
	}
}

VObject* VCandle::MakeBoxEdge()
{
	VObject* box = new VObject(0.f, 0.f, 10.f, 10.f);
	AddEdgesFromObjects(box);
	return box;
}

VEdge* VCandle::MakeLineEdge()
{
	VEdge* line = new VEdge();
	line->Size = sf::Vector2f(10.f, 10.f);
	AddEdgesFromObjects(line);
	return line;
}

void VCandle::UpdateEdges()
{
	unsigned int i = 0;

	for (auto m = members.begin(); m != members.end(); ++m)
	{
		applyEdge(*m, i);
	}

	while (i < edgeList->edges.size())
	{
		edgeList->edges.pop_back();
	}
}

void VCandle::Destroy()
{
	VSUPERCLASS::Destroy();

	members.clear();
	members.shrink_to_fit();

	for (auto l = lights.begin(); l != lights.end(); ++l)
	{
		delete *l;
	}

	lights.clear();
	lights.shrink_to_fit();
}

void VCandle::Update(float dt)
{
	VSUPERCLASS::Update(dt);

	UpdateEdges();

	for (auto l = lights.begin(); l != lights.end(); ++l)
	{
		(*l)->Cast(edgeList.get());
	}
}

void VCandle::Draw(sf::RenderTarget& RenderTarget)
{
	VSUPERCLASS::Draw(RenderTarget);
	for (auto l = lights.begin(); l != lights.end(); ++l)
	{
		RenderTarget.draw(*(*l)->Source());
	}
}

VCandleArea::VCandleArea(bool fog, const sf::Vector2f& position, const sf::Vector2f& size)
{
	area = std::make_unique<candle::LightingArea>(fog ? candle::LightingArea::FOG : candle::LightingArea::AMBIENT, position, size);
}

sf::FloatRect VCandleArea::GetLocalBounds() const
{
	return area->getLocalBounds();
}

sf::FloatRect VCandleArea::GetGlobalBounds() const
{
	return area->getGlobalBounds();
}

void VCandleArea::SetAreaColor(sf::Color color)
{
	area->setAreaColor(color);
}

sf::Color VCandleArea::GetAreaColor() const
{
	return area->getAreaColor();
}

void VCandleArea::SetAreaOpacity(float opacity)
{
	area->setAreaOpacity(opacity);
}

float VCandleArea::GetAreaOpacity() const
{
	return area->getAreaOpacity();
}

void VCandleArea::SetAreaTexture(const sf::String& filename, sf::IntRect rect)
{
	area->setAreaTexture(&VGlobal::p()->Content->LoadTexture(filename), rect);
}

const sf::Texture* VCandleArea::GetAreaTexture() const
{
	return area->getAreaTexture();
}

void VCandleArea::SetTextureRect(const sf::IntRect& rect)
{
	area->setTextureRect(rect);
}

sf::IntRect VCandleArea::GetTextureRect() const
{
	return area->getTextureRect();
}

void VCandleArea::FogMode(bool fog)
{
	area->setMode(fog ? candle::LightingArea::FOG : candle::LightingArea::AMBIENT);
}

bool VCandleArea::IsFogMode() const
{
	return area->getMode() == candle::LightingArea::FOG;
}

void VCandleArea::Draw(sf::RenderTarget& RenderTarget)
{
	if (IsFogMode())
	{
		VBase::Draw(RenderTarget);
		area->clear();
		for (auto l = lights.begin(); l != lights.end(); ++l)
		{
			area->draw(*(*l)->Source());
		}
		area->display();
		RenderTarget.draw(*area);
	}
	else
	{
		VSUPERCLASS::Draw(RenderTarget);
	}
}
#endif