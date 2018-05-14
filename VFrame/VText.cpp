#include "VText.h"
#include "VGlobal.h"

#include <sstream>

using std::vector;
using std::getline;

void VText::setDimensions()
{
	if (font == nullptr)
	{
		font = new sf::Font();
		if (font->loadFromFile("arial.ttf"))
			SetFormat(*font, fontSize, GetFillTint(), alignment, style);

		disposible = true;
	}

	vertices.clear();
	vertices.setPrimitiveType(sf::Triangles);

	sf::String& printText = text;

	bool bold = style & sf::Text::Bold;
	int newLineCount = 1;

	if (wrap != VTextWrap::WRAPNONE)
	{
		unsigned int currentOffset = 0;
		bool firstWord = true;
		size_t wordBeginning = 0;
	
		for (size_t p(0); p < text.getSize(); ++p)
		{
			auto currentChar = text[p];
			if (currentChar == '\n')
			{
				newLineCount++;
				currentOffset = 0;
				firstWord = true;
				continue;
			}
			else if (currentChar == ' ')
			{
				wordBeginning = p;
				firstWord = false;
			}

			sf::Glyph glyph = font->getGlyph(currentChar, fontSize, bold);
			currentOffset += static_cast<unsigned int>(glyph.advance);

			if (wrap == WRAPWORD)
			{
				if (!firstWord && currentOffset > Size.x)
				{
					newLineCount++;
					p = wordBeginning;
					printText[p] = '\n';
					firstWord = true;
					currentOffset = 0;
				}
			}
			else if (wrap == WRAPLETTER)
			{
				if (currentOffset > Size.x)
				{
					newLineCount++;
					printText[p] = '\n';
					currentOffset = 0;
				}
			}
		}
	}

	Size.y = font->getLineSpacing(fontSize) * (wrap != WRAPNONE ? newLineCount : 1);
	updateTextRender(printText);

	origin = sf::Vector2f(Size.x * Origin.x, Size.y * Origin.y);
}

void VText::updateTransform()
{
	transformable.setPosition(Position + origin);
	transformable.setRotation(Angle);
	transformable.setScale(Scale);
	transformable.setOrigin(origin);

	RenderState.transform = sf::Transform::Identity;
	RenderState.transform *= transformable.getTransform();
}

void VText::updateTextRender(sf::String text)
{
	std::wstringstream ss(text);

	bool  bold = (style & sf::Text::Bold) != 0;
	bool  underlined = (style & sf::Text::Underlined) != 0;
	bool  strikeThrough = (style & sf::Text::StrikeThrough) != 0;
	float italic = (style & sf::Text::Italic) ? 0.208f : 0.f;
	float underlineOffset = font->getUnderlinePosition(fontSize);
	float underlineThickness = font->getUnderlineThickness(fontSize);
	bool outlined = outlineThickness > 0.0f;

	sf::FloatRect xBounds = font->getGlyph(L'x', fontSize, bold).bounds;
	float strikeThroughOffset = xBounds.top + xBounds.height / 2.f;

	float hspace = static_cast<float>(font->getGlyph(L' ', fontSize, bold).advance);

	unsigned int letterCount = 0;

	std::wstring item;
	float y = static_cast<float>(fontSize);
	while (std::getline(ss, item, L'\n'))
	{
		float x = 0, xOffset = 0;

		sf::VertexArray verts(sf::Triangles, outlined ? item.length() * 6 * 2 : item.length() * 6);
		sf::Uint32 prevChar = 0;
		int outlineVertOffset = item.length() * 6;

		for (size_t i = 0; i < item.length(); ++i)
		{
			letterCount++;
			if (letterCount > (unsigned)length)
				break;

			sf::Uint32 curChar = item[i];
			const sf::Glyph& glyph = font->getGlyph(curChar, fontSize, bold);

			x += font->getKerning(prevChar, curChar, fontSize);
			prevChar = curChar;

			if ((curChar == L' ') || (curChar == L'\t'))
			{
				switch (curChar)
				{
				case L' ':
					x += hspace;
					break;
				case L'\t': 
					x += hspace * 4;
					break;
				}
				continue;
			}

			if (outlined)
			{
				const sf::Glyph& outlineGlyph = font->getGlyph(curChar, fontSize, bold, outlineThickness);
				setCharacterRender(outlineGlyph, x + outlineOffset.x, y + outlineOffset.y, outlineColour, bold, italic, (i * 6), verts, outlineThickness);
				setCharacterRender(glyph, x, y, fillColour, bold, italic, outlineVertOffset + (i * 6), verts, 0.0f);
			}
			else
			{
				setCharacterRender(glyph, x, y, fillColour, bold, italic, (i * 6), verts, 0.0f);
			}

			x += glyph.advance;
		}

		if (underlined)
		{
			int vertOffset = verts.getVertexCount();
			verts.resize(vertOffset + (outlined ? 12 : 6));

			if (outlined)
			{
				setTextLine(x + outlineOffset.x, y + outlineOffset.y, outlineColour, underlineOffset, underlineThickness, vertOffset, verts, outlineThickness);
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
				setTextLine(x + outlineOffset.x, y + outlineOffset.y, outlineColour, strikeThroughOffset, underlineThickness, vertOffset, verts, outlineThickness);
				setTextLine(x, y, fillColour, strikeThroughOffset, underlineThickness, vertOffset + 6, verts, 0.0f);
			}
			else
			{
				setTextLine(x, y, fillColour, strikeThroughOffset, underlineThickness, vertOffset, verts, 0.0f);
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

void VText::setCharacterRender(const sf::Glyph& glyph, float x, float y, sf::Color color, bool bold, float italic, int index, sf::VertexArray& vertices, float outline)
{
	color.a = fillColour.a;

	float left		= std::floor(glyph.bounds.left);
	float top		= std::floor(glyph.bounds.top);
	float right		= std::ceil(glyph.bounds.left + glyph.bounds.width);
	float bottom	= std::ceil(glyph.bounds.top + glyph.bounds.height);

	float u1 = static_cast<float>(glyph.textureRect.left);
	float v1 = static_cast<float>(glyph.textureRect.top);
	float u2 = static_cast<float>(glyph.textureRect.left + glyph.textureRect.width);
	float v2 = static_cast<float>(glyph.textureRect.top + glyph.textureRect.height);

	vertices[index + 0].position = sf::Vector2f(x + left	- italic * top		- outline, y + top		- outline);	vertices[index + 0].color = color; vertices[index + 0].texCoords = sf::Vector2f(u1, v1);
	vertices[index + 1].position = sf::Vector2f(x + right	- italic * top		- outline, y + top		- outline);	vertices[index + 1].color = color; vertices[index + 1].texCoords = sf::Vector2f(u2, v1);
	vertices[index + 2].position = sf::Vector2f(x + left	- italic * bottom	- outline, y + bottom	- outline);	vertices[index + 2].color = color; vertices[index + 2].texCoords = sf::Vector2f(u1, v2);
	vertices[index + 3].position = sf::Vector2f(x + left	- italic * bottom	- outline, y + bottom	- outline);	vertices[index + 3].color = color; vertices[index + 3].texCoords = sf::Vector2f(u1, v2);
	vertices[index + 4].position = sf::Vector2f(x + right	- italic * top		- outline, y + top		- outline);	vertices[index + 4].color = color; vertices[index + 4].texCoords = sf::Vector2f(u2, v1);
	vertices[index + 5].position = sf::Vector2f(x + right	- italic * bottom	- outline, y + bottom	- outline);	vertices[index + 5].color = color; vertices[index + 5].texCoords = sf::Vector2f(u2, v2);
}

void VText::setTextLine(float x, float y, sf::Color color, float offset, float thickness, int index, sf::VertexArray& vertices, float outline)
{
	color.a = fillColour.a;
	
	float top		= std::floor(y + offset - (thickness / 2) + 0.5f									- outline);
	
	float bottom	= std::ceil(top + (thickness + 0.5f > 1.0f ? std::floor(thickness + 0.5f) : 1.0f)	+ outline);

	vertices[index + 0].position = sf::Vector2f(0, top);	vertices[index + 0].color = color; vertices[index + 0].texCoords = sf::Vector2f(1, 1);
	vertices[index + 1].position = sf::Vector2f(x, top);	vertices[index + 1].color = color; vertices[index + 1].texCoords = sf::Vector2f(1, 1);
	vertices[index + 2].position = sf::Vector2f(0, bottom);	vertices[index + 2].color = color; vertices[index + 2].texCoords = sf::Vector2f(1, 1);
	vertices[index + 3].position = sf::Vector2f(0, bottom);	vertices[index + 3].color = color; vertices[index + 3].texCoords = sf::Vector2f(1, 1);
	vertices[index + 4].position = sf::Vector2f(x, top);	vertices[index + 4].color = color; vertices[index + 4].texCoords = sf::Vector2f(1, 1);
	vertices[index + 5].position = sf::Vector2f(x, bottom);	vertices[index + 5].color = color; vertices[index + 5].texCoords = sf::Vector2f(1, 1);
}

VText* VText::SetFormat(const sf::String& filename, int charSize, sf::Color colour, VTextAlign alignment, int style)
{
	if (disposible)
	{
		delete font;
		font = nullptr;
		disposible = false;
	}

	font = &VGlobal::p()->Content->LoadFont(filename);
	SetAlignment(alignment);
	SetFontSize(charSize);
	SetStyle(style);
	SetFillTint(colour);
	dirty = true;
	
	return this;
}

VText* VText::SetFormat(sf::Font& font, int charSize, sf::Color colour, VTextAlign alignment, int style)
{
	if (disposible)
	{
		delete this->font;
		this->font = nullptr;
		disposible = false;
	}

	this->font = &font;
	SetAlignment(alignment);
	SetFontSize(charSize);
	SetStyle(style);
	SetFillTint(colour);
	dirty = true;

	return this;
}

void VText::SetText(const sf::String& text)
{
	if (this->text != text)
	{
		this->text = text;
		length = text.getSize();
		dirty = true;
	}
}

void VText::SetStyle(int style)
{
	if (this->style != style)
	{
		this->style = style;
		dirty = true;
	}
}

void VText::SetFillTint(const sf::Color &colour)
{
	if (fillColour != colour)
	{
		fillColour = colour;
		dirty = true;
	}
}

void VText::SetOutlineTint(const sf::Color &colour)
{
	if (outlineColour != colour)
	{
		outlineColour = colour;

		if (outlineThickness > 0)
			dirty = true;
	}
}

void VText::SetFontSize(unsigned int size)
{
	if (fontSize != size)
	{
		fontSize = size;
		dirty = true;
	}
}

void VText::SetWrap(const VTextWrap& wrapMode)
{
	if (wrap != wrapMode)
	{
		wrap = wrapMode;
		dirty = true;
	}
}

void VText::SetLineSpaceModifier(float lineSpacing)
{
	if (lineSpaceModifier != lineSpacing)
	{
		lineSpaceModifier = lineSpacing;
		dirty = true;
	}
}

void VText::SetAlignment(const VTextAlign& align)
{
	if (alignment != align)
	{
		alignment = align;
		dirty = true;
	}
}

void VText::SetOutlineThickness(float thickness)
{
	if (outlineThickness != thickness)
	{
		outlineThickness = thickness;
		dirty = true;
	}
}

void VText::SetOutlineOffset(const sf::Vector2f& offset)
{
	if (outlineOffset != offset)
	{
		outlineOffset = offset;
		dirty = true;
	}
}

void VText::Destroy()
{
	VSUPERCLASS::Destroy();
	vertices.clear();

	if (disposible)
	{
		delete font;
		font = nullptr;
		disposible = false;
	}
}

void VText::Update(float dt)
{
	VSUPERCLASS::Update(dt);

	if (dirty)
	{
		setDimensions();
		RenderState.texture = &font->getTexture(fontSize);
		dirty = false;
	}
}

void VText::Draw(sf::RenderTarget& RenderTarget)
{
	VSUPERCLASS::Draw(RenderTarget);

	sf::View renderTargetView = RenderTarget.getView();
	sf::View scrollView = RenderTarget.getDefaultView();

	if (TestInView(renderTargetView, scrollView, this))
	{
		RenderTarget.setView(scrollView);
		RenderTarget.draw(vertices, RenderState);
		RenderTarget.setView(renderTargetView);
	}
}
