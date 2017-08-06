#pragma once
#include "VText.h"

//Special text path that renders text along a specific curve.
class VTextPath : public VText
{
protected:
	virtual void updateTextRender(sf::String text);
	static float vmodf(float x, float y);

public:
	enum PathCurveType : unsigned char
	{
		PATH_NONE,
		PATH_ANCHOR,
		PATH_BEZIER,
		NUM_PATH_CURVES,
	};

	typedef VText VSUPERCLASS;
	VTextPath(float x = 0, float y = 0, float width = 0, const sf::String& text = "", int charSize = 8);

	virtual void Update(float dt) override;

	//Add a point to the path.
	void AddPoint(const sf::Vector2f& point);
	//Sets the path to a list of points.
	void SetPoints(const std::vector<sf::Vector2f>& points);
	//Update an existing point.
	void UpdatePoint(unsigned int index, float x, float y);

	//Set the type of curve that the points to follow.
	void SetCurveType(PathCurveType curve) { curveType = curve; setDimensions(); }
	//Get the current curve type set.
	PathCurveType GetCurveType() { return curveType; }

	//Set the offset that the text should start rendering on the curve (wrapped to 1.0f). Text wraps along path.
	void SetPathOffset(float offset) { pathOffset = offset; setDimensions(); }
	//Get current path offset.
	float GetPathOffset() { return pathOffset; }

private:
	PathCurveType curveType = PATH_NONE;
	float pathOffset = 0.0f;

	std::vector<sf::Vector2f> pointList;
	sf::Vector2f GetBezierPoint(float t);
};

