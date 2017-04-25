#pragma once
#include "VText.h"

class VTextPath : public VText
{
protected:
	virtual void updateTextRender(sf::String text);
	static float vmodf(float x, float y);

public:
	enum PathCurveType
	{
		PATH_NONE,
		PATH_ANCHOR,
		PATH_BEZIER,
		NUM_PATH_CURVES,
	};

	typedef VText VSUPERCLASS;
	VTextPath(float x = 0, float y = 0, float width = 0, const sf::String& text = "", int charSize = 8);

	virtual ~VTextPath()
	{
		Destroy();
	}

	virtual void Update(float dt) override;

	void AddPoint(const sf::Vector2f& point);
	void SetPoints(const std::vector<sf::Vector2f>& points);
	void UpdatePoint(unsigned int index, float x, float y);

	void SetCurveType(PathCurveType curve) { curveType = curve; setDimensions(); }
	PathCurveType GetCurveType() { return curveType; }

	void SetPathOffset(float offset) { pathOffset = offset; setDimensions(); }
	float GetPathOffset() { return pathOffset; }

private:
	PathCurveType curveType = PATH_NONE;
	float pathOffset = 0.0f;

	std::vector<sf::Vector2f> pointList;
	sf::Vector2f GetBezierPoint(float t);
};

