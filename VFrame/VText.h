#pragma once
#include "VObject.h"

#include <SFML\Graphics\Font.hpp>
#include <SFML\Graphics\Text.hpp>
#include <SFML\Graphics\VertexArray.hpp>
#include <SFML\Graphics\Transformable.hpp>
#include <SFML\Graphics\Color.hpp>
#include <SFML\System\String.hpp>

enum VTextAlign
{
	ALIGNLEFT	= 0,
	ALIGNCENTER = 1,
	ALIGNCENTRE = 1,
	ALIGNRIGHT	= 2
};

enum VTextWrap
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
	sf::Font font;
	sf::String helperText;
	sf::Vector2f helperOrigin;
	VTextAlign helperAlignment = ALIGNLEFT;
	int helperStyle = 0;
	int helperLineSpace = 0;
	float helperOutlineThickness = 0;

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
	sf::String Text = "";
	unsigned int FontSize = 8;
	unsigned int Style = sf::Text::Regular;
	unsigned int Wrap = WRAPWORD;
	int LineSpaceModifier = 0;
	sf::Vector2f Scale = sf::Vector2f(1,1);
	VTextAlign Alignment = ALIGNLEFT;
	sf::Vector2f Origin;
	float OutlineThickness = 0.0f;

	VText(float x = 0, float y = 0, float width = 0, const sf::String& text = "", int charSize = 8) : 	VObject(x,y),
																										Text(text),
																										helperText(text)
	{
		Size.x = width;
		FontSize = charSize;

		transformable.setPosition(x, y);

		Moves = false;

		SetFormat("VFrame//Arial.ttf", charSize);

#if _DEBUG
		DebugColor = sf::Color(0, 0, 255, 128);
#endif
	}

	VText(sf::Vector2f position, float width = 0, const sf::String& text = "", int charSize = 8) : 	VObject(position),
																									Text(text),
																									helperText(text)
	{
		Size.x = width;
		FontSize = charSize;

		transformable.setPosition(position);

		Moves = false;

		SetFormat("VFrame//Arial.ttf", charSize);

#if _DEBUG
		DebugColor = sf::Color(0, 0, 255, 128);
#endif
	}

	virtual ~VText()
	{
		Destroy();
	}

	VText* SetFormat(sf::String filename, int charSize = 8, sf::Color colour = sf::Color::White,
		VTextAlign alignment = VTextAlign::ALIGNLEFT, int style = sf::Text::Regular);

	VText* SetFormat(sf::Font fontData, int charSize = 8, sf::Color colour = sf::Color::White,
		VTextAlign alignment = VTextAlign::ALIGNLEFT, int style = sf::Text::Regular);

	void SetStyle(int style = sf::Text::Regular);
	void SetFillTint(const sf::Color &colour);
	void SetOutlineTint(const sf::Color &colour);

	sf::Color const& GetFillTint()
	{
		return fillColour;
	}

	sf::Color const& GetOutlineTint()
	{
		return outlineColour;
	}

	virtual void Destroy() override;
	virtual void Update(float dt) override;
	virtual void Draw(sf::RenderTarget& RenderTarget) override;
};