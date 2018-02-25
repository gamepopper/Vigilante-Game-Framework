/**
* @file    VText.h
* @author  Tim Stoddard <tim@gamepopper.co.uk>
*
* @section LICENSE
*
* MIT License
*
* Copyright (c) 2018 Tim Stoddard
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*
* @section DESCRIPTION
*
* Text rendering object.
*/

#pragma once
#include "VObject.h"

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/System/String.hpp>

///Propertry for aligning the text within the text box.
enum VTextAlign : unsigned char
{
	ALIGNLEFT	= 0,
	ALIGNCENTER = 1,
	ALIGNCENTRE = 1,
	ALIGNRIGHT	= 2
};

///Property for the method of wrapping the text when it reaches tje edge of the box.
enum VTextWrap : unsigned char
{
	WRAPNONE	= 0,
	WRAPLETTER	= 1,
	WRAPWORD	= 2,
};

///Object that renders text.
class VText : public VObject
{
protected:
	///Vertex Data.
	sf::VertexArray vertices;
	///Transform Data.
	sf::Transformable transformable;
	///Font object.
	sf::Font* font = nullptr;
	///If true, the text vertex data is rebuilt for all the new properties.
	bool dirty = false;
	///If true, the font is disposed by the Text object (in case the font isn't loaded from VContent).
	bool disposible = false;
	///Origin point for rotation.
	sf::Vector2f origin;
	///Main colour of the font.
	sf::Color fillColour = sf::Color::White;
	///Colour of the font outline.
	sf::Color outlineColour = sf::Color::White;
	/**
	* Update the vertex data to display text and outline.
	* @param text The string that will be used for displaying text.
	*/
	virtual void updateTextRender(sf::String text);
	///Update the transformable object.
	virtual void updateTransform() override;
	///Setup the string of text so that it fits within the text width.
	void setDimensions();

	/**
	* Sets the render infomation for a single character.
	* @param glyph The character texture infomation taken directly from the font.
	* @param x The X Coordinate the character will be displayed in.
	* @param y The Y Coordinate the character will be displayed in.
	* @param color Colour Tint of the character to display.
	* @param bold If true, the bold version of the character will be displayed.
	* @param italic If true, the rendered vertices will be set so the character will be rendered in italics.
	* @param index The first index position in the vertex array to set the character. Six vertex indices will be used for a single character.
	* @param vertices The vertex array data to render the text into.
	* @param outline The outline thickness, used to setup the outline version of the character.
	*/
	void setCharacterRender(const sf::Glyph& glyph, float x, float y, sf::Color color, bool bold, float italic, int index, sf::VertexArray& vertices, float outline = 0);

	/**
	* Sets the render infomation for a single line, for displaying strikethroughs and underlines.
	* @param x The X Coordinate the line will be displayed in.
	* @param y The Y Coordinate the line will be displayed in.
	* @param color Colour Tint of the character to display.
	* @param offset Y offset the line will be diplayed at.
	* @param thickness The thickness of the line.
	* @param index The first index position in the vertex array to set the character. Six vertex indices will be used for a single character.
	* @param vertices The vertex array data to render the text into.
	* @param outline The outline thickness, used to setup the outline version of the character.
	*/
	void setTextLine(float x, float y, sf::Color color, float offset, float thickness, int index, sf::VertexArray& vertices, float outline = 0);

public:
	///Used to call parent class functions when they are overrided in class.
	typedef VObject VSUPERCLASS;

	///RenderState for the vertex data.
	sf::RenderStates RenderState = sf::RenderStates::Default;
	///String of text that will be rendered.
	sf::String Text;
	///Size of the text per character.
	unsigned int FontSize = 8;
	///Flags for setting a text's display style.
	unsigned int Style = sf::Text::Regular;
	///Text wrapping mode.
	VTextWrap Wrap = WRAPWORD;
	///Used for adjusting the distance between lines different to the default.
	int LineSpaceModifier = 0;
	///Scale transform.
	sf::Vector2f Scale = sf::Vector2f(1,1);
	///Text alignment property.
	VTextAlign Alignment = ALIGNLEFT;
	///Origin point as percentage.
	sf::Vector2f Origin;
	///The thickness of the outline.
	float OutlineThickness = 0.0f;
	///The position offset of the outline. Good to modify if you want to display the outline like a shadow.
	sf::Vector2f OutlineOffset;

	/**
	* @param x X position coordinate.
	* @param y Y position coordinate.
	* @param width The width of the text area that will bound the text.
	* @param text The string that the text object will display.
	* @param charSize The font size the text will be displayed at.
	*/
	VText(float x = 0, float y = 0, float width = 0, const sf::String& text = "", int charSize = 8) : 	VObject(x,y),
																										Text(text),
																										FontSize(charSize)
	{
		Size.x = width;
		transformable.setPosition(Position);

		Moves = false;

#ifdef _DEBUG
		DebugColor = sf::Color(0, 0, 255, 128);
#endif
	}

	/**
	* @param position Position coordinates.
	* @param width The width of the text area that will bound the text.
	* @param text The string that the text object will display.
	* @param charSize The font size the text will be displayed at.
	*/
	VText(sf::Vector2f position, float width = 0, const sf::String& text = "", int charSize = 8) : 	VObject(position),
																									Text(text)
	{
		Size.x = width;
		FontSize = charSize;

		transformable.setPosition(position);

		Moves = false;

#ifdef _DEBUG
		DebugColor = sf::Color(0, 0, 255, 128);
#endif
	}

	/**
	* Sets up the main format of how the text will be rendered.
	* @param filename Font file path and name.
	* @param charSize Size of the text.
	* @param colour Fill colour of the text.
	* @param alignment How the text will be anchored line by line within the text area.
	* @param style Text font styles (i.e. bold, italic, strikethrough, ect).
	*/
	VText* SetFormat(const sf::String& filename, int charSize = 8, sf::Color colour = sf::Color::White,
		VTextAlign alignment = VTextAlign::ALIGNLEFT, int style = sf::Text::Regular);

	/**
	* Sets up the main format of how the text will be rendered.
	* @param fontData sf::Font object.
	* @param charSize Size of the text.
	* @param colour Fill colour of the text.
	* @param alignment How the text will be anchored line by line within the text area.
	* @param style Text font styles (i.e. bold, italic, strikethrough, ect).
	*/
	VText* SetFormat(sf::Font& fontData, int charSize = 8, sf::Color colour = sf::Color::White,
		VTextAlign alignment = VTextAlign::ALIGNLEFT, int style = sf::Text::Regular);

	///@param style Font styles flags (i.e. Bold, Italic, Strikethrough, ect).
	void SetStyle(int style = sf::Text::Regular);
	///@param colour The main colour of the text.
	void SetFillTint(const sf::Color &colour);
	///@param colour The text outline (requires OutlineThickness to be greater than 0).
	void SetOutlineTint(const sf::Color &colour);
	///@return The main text colour.
	sf::Color const& GetFillTint() { return fillColour; }
	///@return The colour of the text outline.
	sf::Color const& GetOutlineTint() {	return outlineColour; }

	///Applies the changes to the text format, calling this is required when changing any public properties (including Text).
	void ApplyChanges();
	///Destroys the text rendering data (and font if disposable is set for true).
	virtual void Destroy() override;

	/**
	* Updates text transform and rendering info.
	* @param dt Delta Time between the previous and current frame.
	*/
	virtual void Update(float dt) override;

	/**
	* Renders the text.
	* @param RenderTarget The game's sf::RenderTarget object to render data onto.
	*/
	virtual void Draw(sf::RenderTarget& RenderTarget) override;
};
