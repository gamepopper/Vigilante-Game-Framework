#pragma once

#ifndef VFRAME_NO_CANDLE
#include "VGroup.h"
#include "VObject.h"
#include <SFML/System/String.hpp>

namespace sfu
{
	struct Line;
}

namespace candle
{
	class LightSource;
	class LightingArea;
}

struct VEdgeList;

class VLight
{
private:
	std::unique_ptr<candle::LightSource> light;

public:
	VLight(bool radial);

	void SetPosition(const sf::Vector2f& pos);
	void SetAngle(float angle);
	void SetWidth(float width);
	void SetRange(float range);
	void SetFade(float fade);
	void SetIntensity(float intensity);
	void SetTint(const sf::Color& tint);

	sf::Vector2f GetPosition();
	float GetAngle();
	float GetWidth();
	float GetRange();
	float GetFade();
	float GetIntensity();
	sf::Color GetTint();
	void Cast(const VEdgeList* edgeData);
	const candle::LightSource* Source();
};

class VEdge : public VObject
{
public:
	VEdge() = default;
};

class VCandle : public VBase
{
protected:
	std::vector<VBase*> members;
	std::vector<VLight*> lights;
	std::unique_ptr<VEdgeList> edgeList;

	unsigned int numberOfEdges(VBase* b);
	void applyEdge(VBase* b, unsigned int& i);

public:
	typedef VBase VSUPERCLASS;

	VCandle() = default;
	
	VLight* AddDirectedLight();
	VLight* AddRadialLight();
	void RemoveLight(VLight* light, bool destroy = true);

	void AddEdgesFromObjects(VBase* o);
	VObject* MakeBoxEdge();
	VEdge* MakeLineEdge();

	void UpdateEdges();

	virtual void Destroy();
	virtual void Update(float dt);
	virtual void Draw(sf::RenderTarget& RenderTarget);
};

class VCandleArea : public VCandle
{
private:
	std::unique_ptr<candle::LightingArea> area;

public:
	typedef VBase VSUPERCLASS;

	VCandleArea(bool fog, const sf::Vector2f& position, const sf::Vector2f& size);
	
	/**
	 * @brief Get the local bounding rectangle of the area.
	 * @details The rectangle returned bounds the area before any
	 * transformations.
	 * @returns Local bounding rectangle of the area.
	 */
	sf::FloatRect GetLocalBounds() const;

	/**
	 * @brief Get the global bounding rectangle of the area.
	 * @details The rectangle returned bounds the area with the
	 * transformation already applied.
	 * @returns Global bounding rectangle of the area.
	 */
	sf::FloatRect GetGlobalBounds() const;

	/**
	 * @brief Set color of the fog/light.
	 * @details If the area has no texture, the plain color is used in
	 * the next calls to @ref clear. Otherwise, the texture is multiplied
	 * by the color. In both cases, the alpha value of the color is
	 * preserved.
	 *
	 * The default color is sf::Color::White.
	 * @param color
	 * @see getAreaColor, setAreaOpacity
	 */
	void SetAreaColor(sf::Color color);

	/**
	 * @brief Get color of the fog/light.
	 * @returns The plain color of the fog/light.
	 * @see setAreaColor, setAreaOpacity
	 */
	sf::Color GetAreaColor() const;

	/**
	 * @brief Set the opacity of the fog/light.
	 * @details The opacity is a value multiplied to the alpha value before
	 * any use of the color, to ease the separate manipulation.
	 * @param opacity
	 * @see getAreaOpacity, setAreaColor
	 */
	void SetAreaOpacity(float opacity);

	/**
	 * @brief Get the opacity of the fog/light.
	 * @returns The opacity of the fog/light.
	 * @see setAreaOpacity getAreaColor
	 */
	float GetAreaOpacity() const;

	/**
	 * @brief Set the texture of the fog/light.
	 * @param texture Pointer to the new texture. Pass a null pointer to
	 * just unset the texture.
	 * @param rect Optional rectangle to call @ref setTextureRect. If none
	 * is specified, the whole texture is used.
	 * @see getAreaTexture
	 */
	void SetAreaTexture(const sf::String& filename, sf::IntRect rect = sf::IntRect());

	/**
	 * @brief Get the texture of the fog/light.
	 * @returns Pointer to the texture of the fog/light.
	 * @see getAreaTexture
	 */
	const sf::Texture* GetAreaTexture() const;

	/**
	 * @brief Set the rectangle of the used sub-section of the texture.
	 * @details Note that this function won't adjust the size of the area
	 * to fit the new rectangle.
	 * @param rect
	 * @see getTextureRect
	 */
	void SetTextureRect(const sf::IntRect& rect);

	/**
	 * @brief Get the rectangle of the used sub-section of the texture.
	 * @returns The rectangle of the used sub-section of the texture.
	 * @see setTextureRect
	 */
	sf::IntRect GetTextureRect() const;

	/**
	 * @brief Set the lighting mode.
	 * @param mode
	 * @see LightingArea::Mode, setMode
	 */
	void FogMode(bool fog);

	/**
	 * @brief Set the lighting mode.
	 * @returns The lighting mode.
	 * @see LightingArea::Mode, setMode
	 */
	bool IsFogMode() const;

	virtual void Draw(sf::RenderTarget& RenderTarget);
};
#endif