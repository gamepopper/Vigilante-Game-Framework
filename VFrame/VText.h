#pragma once
#include "VObject.h"

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/System/String.hpp>

enum VTextAlign : unsigned char
{
	ALIGNLEFT	= 0,
	ALIGNCENTER = 1,
	ALIGNCENTRE = 1,
	ALIGNRIGHT	= 2
};

enum VTextWrap : unsigned char
{
	WRAPNONE	= 0,
	WRAPLETTER	= 1,
	WRAPWORD	= 2,
};

class VText : public VObject
{
protected:
	sf::VertexArray vertices;
	sf::Transformable transformable;
	sf::Font* font = nullptr;
	bool dirty = false;

	sf::Vector2f origin;
	sf::Color fillColour = sf::Color::White;
	sf::Color outlineColour = sf::Color::White;

	virtual void updateTextRender(sf::String text);
	virtual void updateTransform() override;

	void setDimensions();
	void setCharacterRender(sf::Uint32 character, float x, float y, sf::Color color, bool bold, float italic, int index, sf::VertexArray& vertices, float outline = 0);
	void setTextLine(float x, float y, sf::Color color, float offset, float thickness, int index, sf::VertexArray& vertices, float outline = 0);

public:
	typedef VObject VSUPERCLASS;
	sf::RenderStates RenderState = sf::RenderStates::Default;
	sf::String Text;
	unsigned int FontSize = 8;
	unsigned int Style = sf::Text::Regular;
	VTextWrap Wrap = WRAPWORD;
	int LineSpaceModifier = 0;
	sf::Vector2f Scale = sf::Vector2f(1,1);
	VTextAlign Alignment = ALIGNLEFT;
	sf::Vector2f Origin;
	float OutlineThickness = 0.0f;
	sf::Vector2f OutlineOffset;

	VText(float x = 0, float y = 0, float width = 0, const sf::String& text = "", int charSize = 8) : 	VObject(x,y),
																										Text(text)
	{
		Size.x = width;
		FontSize = charSize;

		transformable.setPosition(x, y);

		Moves = false;

		try
		{
			SetFormat("VFrame/depend/arial.ttf", charSize);
		}
		#ifndef __linux__
		catch (std::exception e)
		{
			VLog(e.what());
		}
		#else
		catch (std::string e)
		{
			VLog(e.c_str());
		}
		#endif

#if _DEBUG
		DebugColor = sf::Color(0, 0, 255, 128);
#endif
	}

	VText(sf::Vector2f position, float width = 0, const sf::String& text = "", int charSize = 8) : 	VObject(position),
																									Text(text)
	{
		Size.x = width;
		FontSize = charSize;

		transformable.setPosition(position);

		Moves = false;

		try
		{
			SetFormat("VFrame/depend/arial.ttf", charSize);
		}
		#ifndef __linux__
		catch (std::exception e)
		{
			VLog(e.what());
		}
		#else
		catch (std::string e)
		{
			VLog(e.c_str());
		}
		#endif

#if _DEBUG
		DebugColor = sf::Color(0, 0, 255, 128);
#endif
	}

	/*
	Sets up the main format of how the text will be rendered.
	filename: Font file path.
	charSize: Size of the text.
	colour: Fill colour of the text.
	alignment: How the text will be anchored line by line within the text area.
	style: Text font styles (i.e. bold, italic, strikethrough, ect).
	*/
	VText* SetFormat(const sf::String& filename, int charSize = 8, sf::Color colour = sf::Color::White,
		VTextAlign alignment = VTextAlign::ALIGNLEFT, int style = sf::Text::Regular);

	/*
	Sets up the main format of how the text will be rendered.
	fontData: sf::Font object.
	charSize: Size of the text.
	colour: Fill colour of the text.
	alignment: How the text will be anchored line by line within the text area.
	style: Text font styles (i.e. bold, italic, strikethrough, ect).
	*/
	VText* SetFormat(sf::Font& fontData, int charSize = 8, sf::Color colour = sf::Color::White,
		VTextAlign alignment = VTextAlign::ALIGNLEFT, int style = sf::Text::Regular);

	//Set font styles (i.e. bold, italic, strikethrough, ect).
	void SetStyle(int style = sf::Text::Regular);
	//Set the main colour of the text.
	void SetFillTint(const sf::Color &colour);
	//Set the colour of the text outline (requires OutlineThickness to be greater than 0).
	void SetOutlineTint(const sf::Color &colour);

	//Get the main text colour.
	sf::Color const& GetFillTint()
	{
		return fillColour;
	}

	//Get the colour of the text outline.
	sf::Color const& GetOutlineTint()
	{
		return outlineColour;
	}

	//Applies the changes to the text format, calling this is required when changing properties.
	void ApplyChanges();

	virtual void Destroy() override;
	virtual void Update(float dt) override;
	virtual void Draw(sf::RenderTarget& RenderTarget) override;
};
