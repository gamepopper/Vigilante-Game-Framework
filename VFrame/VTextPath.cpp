#include "VTextPath.h"
#include "VInterpolate.h"
#include "VGlobal.h"

#include <limits>
#include <sstream>

using std::vector;
using std::getline;

VTextPath::VTextPath(float x, float y, float width, const sf::String& text, int charSize) : VText(x, y, width, text, charSize)
{

}

void VTextPath::Update(float dt)
{
	wrap = WRAPNONE;
	VSUPERCLASS::Update(dt);
}

void VTextPath::updateTextRender(sf::String text)
{
	std::wstringstream ss(text);

	bool  bold = (style & sf::Text::Bold) != 0;
	bool  underlined = (style & sf::Text::Underlined) != 0;
	bool  strikeThrough = (style & sf::Text::StrikeThrough) != 0;
	float italic = (style & sf::Text::Italic) ? 0.208f : 0.f;
	float underlineOffset = font->getUnderlinePosition(fontSize);
	float underlineThickness = font->getUnderlineThickness(fontSize);
	bool  outlined = outlineThickness > 0.0f;

	sf::FloatRect xBounds = font->getGlyph(L'x', fontSize, bold).bounds;
	float strikeThroughOffset = xBounds.top + xBounds.height / 2.f;

	//float hspace = static_cast<float>(font->getGlyph(L' ', fontSize, bold).advance);

	float left = (float)0xFFFFFF;
	float top = (float)0xFFFFFF;
	float right = -(float)0xFFFFFF;
	float bottom = -(float)0xFFFFFF;

	for (unsigned int i = 0; i < pointList.size(); i++)
	{
		left = pointList[i].x < left ? pointList[i].x : left;
		right = pointList[i].x > right ? pointList[i].x : right;
		top = pointList[i].y < top ? pointList[i].y : top;
		bottom = pointList[i].y > bottom ? pointList[i].y : bottom;
	}

	Size.x = right - left;
	Size.y = bottom - top;
	Origin.x = -left + (Size.x / 2);
	Origin.y = -top + (Size.y / 2);

	std::wstring item;
	while (std::getline(ss, item, L'\n'))
	{
		float x = 0, xOffset = 0;
		float y = static_cast<float>(fontSize);

		sf::VertexArray verts(sf::Triangles, outlined ? item.length() * 6 * 2 : item.length() * 6);
		//sf::Uint32 prevChar = 0;
		int outlineOffset = item.length() * 6;

		int textLength = item.length();

		for (size_t i = 0; i < item.length(); ++i)
		{
			sf::Uint32 curChar = item[i];
			const sf::Glyph& glyph = font->getGlyph(curChar, fontSize, bold);

			sf::Vector2f p = GetBezierPoint((float)i / textLength);
			x = p.x;
			y = p.y;

			if (outlined)
			{
				const sf::Glyph& outlineGlyph = font->getGlyph(curChar, fontSize, bold, outlineThickness);
				setCharacterRender(outlineGlyph, x, y, outlineColour, bold, italic, (i * 6), verts, outlineThickness);
				setCharacterRender(glyph, x, y, fillColour, bold, italic, outlineOffset + (i * 6), verts, 0.0f);
			}
			else
			{
				setCharacterRender(glyph, x, y, fillColour, bold, italic, (i * 6), verts, 0.0f);
			}

			x += font->getGlyph(curChar, fontSize, bold).advance;

			if (underlined)
			{
				int vertOffset = verts.getVertexCount();
				verts.resize(vertOffset + (outlined ? 12 : 6));

				if (outlined)
				{
					setTextLine(x, y, outlineColour, underlineOffset, underlineThickness, vertOffset, verts, outlineThickness);
					setTextLine(x, y, fillColour, underlineOffset, underlineThickness, vertOffset + 6, verts, 0.0f);
				}
				else
				{
					setTextLine(x, y, fillColour, underlineOffset, underlineThickness, vertOffset, verts, 0.0f);
				}
			}

			if (strikeThrough)
			{
				int vertOffset = verts.getVertexCount();
				verts.resize(vertOffset + (outlined ? 12 : 6));

				if (outlined)
				{
					setTextLine(x, y, outlineColour, strikeThroughOffset, underlineThickness, vertOffset, verts, outlineThickness);
					setTextLine(x, y, fillColour, strikeThroughOffset, underlineThickness, vertOffset + 6, verts, 0.0f);
				}
				else
				{
					setTextLine(x, y, fillColour, strikeThroughOffset, underlineThickness, vertOffset, verts, 0.0f);
				}
			}
		}

		float textWidth = verts.getBounds().width;
		if (alignment == VTextAlign::ALIGNCENTRE)
			xOffset = (Size.x / 2) - (textWidth / 2);
		else if (alignment == VTextAlign::ALIGNRIGHT)
			xOffset = Size.x - textWidth;
		else
			xOffset = 0;

		int vertOffset = vertices.getVertexCount();
		vertices.resize(vertOffset + verts.getVertexCount());
		for (size_t i = 0; i < verts.getVertexCount(); i++)
		{
			verts[i].position.x += xOffset;
			vertices[vertOffset + i] = verts[i];
		}

		y += font->getLineSpacing(fontSize) + lineSpaceModifier;
	}
}

float vmodf(float x, float y)
{
	if (x < 0)
	{
		float mod = -x;
		mod = fmod(mod, y);
		mod = -mod*y;
		return mod;
	}

	return fmod(x, y);
}

sf::Vector2f VTextPath::GetBezierPoint(float t)
{
	t = vmodf(t + pathOffset, 1.0f);
	if (t < std::numeric_limits<float>::min())
	{
		float ceiling = ceilf(-t);
		t += ceiling;
	}

	if (pointList.size() == 0)
		return sf::Vector2f();

	if (curveType == PATH_NONE)
	{
		t *= pointList.size() - 1;

		float curveCount = (float)pointList.size() - 1;
		int curveCutCount = (int)(t / curveCount);
		t = t - (curveCount * curveCutCount);

		//Line to use is the rounded value of t. (e.g. t = 2.4f is third line).
		int firstPoint = static_cast<int>(t);

		int timeCutCount = static_cast<int>(t);
		t = t - timeCutCount;

		if ((unsigned int)(firstPoint + 1) < pointList.size()) //If one point, then it's a staight line.
		{
			float u = 1 - t;

			sf::Vector2f p0 = pointList[firstPoint];
			sf::Vector2f p1 = pointList[firstPoint + 1];

			sf::Vector2f point;
			point.x = (u*p0.x) + (t*p1.x);
			point.y = (u*p0.y) + (t*p1.y);

			return point;
		}
		else
		{
			return pointList[pointList.size() - 1];
		}
	}
	else if (curveType == PATH_ANCHOR)
	{
		std::vector<sf::Vector2f> tmp(pointList);
		for (unsigned int i = tmp.size() - 1; i > 0; i--)
		{
			for (int k = 0; k < (int)i; k++)
			{
				tmp[k] = VInterpolate::Vector2f(tmp[k], tmp[k + 1], t);
			}
		}

		return tmp[0];
	}
	else if (curveType == PATH_BEZIER)
	{
		t *= pointList.size() - 1;

		float curveCount = (float)pointList.size();
		int curveCutCount = (int)(t / curveCount);
		t = t - (curveCount * curveCutCount);

		//Line to use is the rounded value of t. (e.g. t = 2.4f is third line).
		int controlLine = static_cast<int>(t);

		int timeCutCount = static_cast<int>(t);
		t = t - timeCutCount;

		//Assume all lines are cubic, unless there are less than three lines between first point and last control point.
		int firstPoint = (controlLine * 3);
		if ((unsigned int)(firstPoint + 3) < pointList.size())
		{
			float u = 1 - t;
			float uu = u*u;
			float uuu = uu*u;
			float tt = t*t;
			float ttt = tt*t;

			sf::Vector2f p0 = pointList[firstPoint];
			sf::Vector2f p1 = pointList[firstPoint + 1];
			sf::Vector2f p2 = pointList[firstPoint + 2];
			sf::Vector2f p3 = pointList[firstPoint + 3];

			sf::Vector2f point;
			point.x = (uuu * p0.x) + (3 * uu * t * p1.x) + (3 * u * tt * p2.x) + (ttt * p3.x);
			point.y = (uuu * p0.y) + (3 * uu * t * p1.y) + (3 * u * tt * p2.y) + (ttt * p3.y);
			return point;
		}
		else if ((unsigned int)(firstPoint + 2) < pointList.size()) //If two points between first point and last control point, then it's a quadratic curve.
		{
			float u = 1 - t;
			float uu = u*u;
			float tt = t*t;

			sf::Vector2f p0 = pointList[firstPoint];
			sf::Vector2f p1 = pointList[firstPoint + 1];
			sf::Vector2f p2 = pointList[firstPoint + 2];

			sf::Vector2f point;
			point.x = (uu * p0.x) + (2 * u * t * p1.x) + (tt * p2.x);
			point.y = (uu * p0.y) + (2 * u * t * p1.y) + (tt * p2.y);

			return point;
		}
		else if ((unsigned int)(firstPoint + 1) < pointList.size()) //If one point, then it's a staight line.
		{
			float u = 1 - t;

			sf::Vector2f p0 = pointList[firstPoint];
			sf::Vector2f p1 = pointList[firstPoint + 1];

			sf::Vector2f point;
			point.x = (u*p0.x) + (t*p1.x);
			point.y = (u*p0.y) + (t*p1.y);

			return point;
		}
		else
		{
			return pointList[pointList.size() - 1];
		}
	}

	return pointList[0];
}

void VTextPath::AddPoint(const sf::Vector2f& point)
{
	pointList.push_back(point);
	dirty = true;
}

void VTextPath::SetPoints(const std::vector<sf::Vector2f>& points)
{
	pointList.clear();
	pointList.shrink_to_fit();

	pointList = points;
	dirty = true;
}

void VTextPath::UpdatePoint(unsigned int index, float x, float y)
{
	if (index >= pointList.size())
		return;

	pointList[index].x = x;
	pointList[index].y = y;
}
