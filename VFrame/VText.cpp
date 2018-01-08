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
			SetFormat(*font, FontSize);

		disposible = true;
	}

	vertices.clear();
	vertices.setPrimitiveType(sf::Triangles);

	sf::String& printText = Text;

	bool bold = Style & sf::Text::Bold;
	int newLineCount = 1;

	if (Wrap)
	{
		unsigned int currentOffset = 0;
		bool firstWord = true;
		size_t wordBeginning = 0;
	
		for (size_t p(0); p < Text.getSize(); ++p)
		{
			auto currentChar = Text[p];
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

			sf::Glyph glyph = font->getGlyph(currentChar, FontSize, bold);
			currentOffset += static_cast<unsigned int>(glyph.advance);

			if (Wrap == WRAPWORD)
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
			else if (Wrap == WRAPLETTER)
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

	Size.y = font->getLineSpacing(FontSize) * (Wrap != WRAPNONE ? newLineCount : 1);
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

	bool  bold = (Style & sf::Text::Bold) != 0;
	bool  underlined = (Style & sf::Text::Underlined) != 0;
	bool  strikeThrough = (Style & sf::Text::StrikeThrough) != 0;
	float italic = (Style & sf::Text::Italic) ? 0.208f : 0.f;
	float underlineOffset = font->getUnderlinePosition(FontSize);
	float underlineThickness = font->getUnderlineThickness(FontSize);
	bool outlined = OutlineThickness > 0.0f;

	sf::FloatRect xBounds = font->getGlyph(L'x', FontSize, bold).bounds;
	float strikeThroughOffset = xBounds.top + xBounds.height / 2.f;

	float hspace = static_cast<float>(font->getGlyph(L' ', FontSize, bold).advance);

	std::wstring item;
	float y = static_cast<float>(FontSize);
	while (std::getline(ss, item, L'\n'))
	{
		float x = 0, xOffset = 0;

		sf::VertexArray verts(sf::Triangles, outlined ? item.length() * 6 * 2 : item.length() * 6);
		sf::Uint32 prevChar = 0;
		int outlineOffset = item.length() * 6;

		for (size_t i = 0; i < item.length(); ++i)
		{
			sf::Uint32 curChar = item[i];
			const sf::Glyph& glyph = font->getGlyph(curChar, FontSize, bold);

			x += font->getKerning(prevChar, curChar, FontSize);
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
				const sf::Glyph& outlineGlyph = font->getGlyph(curChar, FontSize, bold, OutlineThickness);
				setCharacterRender(outlineGlyph, x + OutlineOffset.x, y + OutlineOffset.y, outlineColour, bold, italic, (i * 6), verts, OutlineThickness);
				setCharacterRender(glyph, x, y, fillColour, bold, italic, outlineOffset + (i * 6), verts, 0.0f);
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
				setTextLine(x + OutlineOffset.x, y + OutlineOffset.y, outlineColour, underlineOffset, underlineThickness, vertOffset, verts, OutlineThickness);
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
				setTextLine(x + OutlineOffset.x, y + OutlineOffset.y, outlineColour, strikeThroughOffset, underlineThickness, vertOffset, verts, OutlineThickness);
				setTextLine(x, y, fillColour, strikeThroughOffset, underlineThickness, vertOffset + 6, verts, 0.0f);
			}
			else
			{
				setTextLine(x, y, fillColour, strikeThroughOffset, underlineThickness, vertOffset, verts, 0.0f);
			}
		}

		float textWidth = verts.getBounds().width;
		if (Alignment == VTextAlign::ALIGNCENTRE)
			xOffset = (Size.x / 2) - (textWidth / 2);
		else if (Alignment == VTextAlign::ALIGNRIGHT)
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

		y += font->getLineSpacing(FontSize) + LineSpaceModifier;
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

	vertices[index + 0].position = sf::Vector2f(x + left	- italic * top		- OutlineThickness, y + top		- OutlineThickness);	vertices[index + 0].color = color; vertices[index + 0].texCoords = sf::Vector2f(u1, v1);
	vertices[index + 1].position = sf::Vector2f(x + right	- italic * top		- OutlineThickness, y + top		- OutlineThickness);	vertices[index + 1].color = color; vertices[index + 1].texCoords = sf::Vector2f(u2, v1);
	vertices[index + 2].position = sf::Vector2f(x + left	- italic * bottom	- OutlineThickness, y + bottom	- OutlineThickness);	vertices[index + 2].color = color; vertices[index + 2].texCoords = sf::Vector2f(u1, v2);
	vertices[index + 3].position = sf::Vector2f(x + left	- italic * bottom	- OutlineThickness, y + bottom	- OutlineThickness);	vertices[index + 3].color = color; vertices[index + 3].texCoords = sf::Vector2f(u1, v2);
	vertices[index + 4].position = sf::Vector2f(x + right	- italic * top		- OutlineThickness, y + top		- OutlineThickness);	vertices[index + 4].color = color; vertices[index + 4].texCoords = sf::Vector2f(u2, v1);
	vertices[index + 5].position = sf::Vector2f(x + right	- italic * bottom	- OutlineThickness, y + bottom	- OutlineThickness);	vertices[index + 5].color = color; vertices[index + 5].texCoords = sf::Vector2f(u2, v2);
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
	Alignment = alignment;
	FontSize = charSize;
	Style = style;
	SetStyle(style);
	fillColour = colour;
	ApplyChanges();
	
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
	Alignment = alignment;
	FontSize = charSize;
	Style = style;
	SetStyle(style);
	fillColour = colour;
	ApplyChanges();

	return this;
}

void VText::SetStyle(int style)
{
	Style = style;
}

void VText::SetFillTint(const sf::Color &colour)
{
	if (fillColour != colour)
	{
		fillColour = colour;
		ApplyChanges();
	}
}

void VText::SetOutlineTint(const sf::Color &colour)
{
	if (outlineColour != colour)
	{
		outlineColour = colour;

		if (OutlineThickness > 0)
			ApplyChanges();
	}
}

void VText::ApplyChanges()
{
	dirty = true;
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
		RenderState.texture = &font->getTexture(FontSize);
		dirty = false;
	}
}

void VText::Draw(sf::RenderTarget& RenderTarget)
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
	scrollView.setViewport(renderTargetView.getViewport());

	sf::FloatRect renderBox = sf::FloatRect(Position, Size);
	float maxSize = fmaxf(scrollView.getSize().x, scrollView.getSize().y);
	sf::FloatRect scrollBox = sf::FloatRect(scrollView.getCenter() - sf::Vector2f(maxSize, maxSize) / 2.0f, sf::Vector2f(maxSize, maxSize));

	if (renderBox.left < scrollBox.left + scrollBox.width &&
		renderBox.left + renderBox.width > scrollBox.left &&
		renderBox.top <	 scrollBox.top + scrollBox.height &&
		renderBox.top + renderBox.height > scrollBox.top)
	{
		RenderTarget.setView(scrollView);
		RenderTarget.draw(vertices, RenderState);
		RenderTarget.setView(renderTargetView);
	}
}
