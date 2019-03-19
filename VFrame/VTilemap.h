/**
* @file    VTilemap.h
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
* Tilemap rendering.
*/

#pragma once
#include "VObject.h"

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/System/String.hpp>

#include <vector>
#include <algorithm>
#include <map>
#include <functional>
#include <memory>

///Infomation for rendering individual tiles.
struct VTileRenderInfo
{
	///Frame Index to Render if not using AutoTile mode.
	int TileNumber;
	///Which level (multiplied by 16) of tiles to use if rendering in AutoTile mode.
	int AutoTileLevel;
};

///Infomation for colliding sets of tiles.
struct VTileCollisionInfo
{
	///Filters the sides of a tile that allows collisions.
	uint8_t AllowCollisions = VObject::TOUCHALL;
	///Callback function if an object collides the tile.
	std::function<void(VObject*, VObject*)> Callback = nullptr;
};

class VTile;

///Object that renders a 2D tilemap, with custom collision and autotiling support.
class VTilemap : public VObject
{
protected:
	///Tilemap data
	std::vector<char> tilemap;
	///List of tile collision information.
	std::map<char, VTileCollisionInfo*> collisionDir;
	///List of tile render information.
	std::map<char, VTileRenderInfo*> renderDir;
	///Autotile Map data (used when in AutoTile mode).
	std::vector<int> autotile;
	///The amount of tiles horizontally in a texture.
	int tileMapWidth = 0;
	///The offset of the texture to use.
	sf::Vector2u textureOffset;

	///Width of the Tilemap in Tiles.
	int mapWidth = 0;
	///Height of the Tilemap in Tiles.
	int mapHeight = 0;
	///Position during the last update of the collision boxes.
	sf::Vector2f last;
	///Updates the collision and render infomation of the tilemap if true.
	bool dirty = true;
	///Updates the render infomation of the tilemap if different from AutoTile.
	bool helperAutoTile = false;
	///Vertex data for render.
	sf::VertexArray vertices;
	///Colour Tint of rendered tilemap.
	sf::Color colour = sf::Color::White;
	///List of Collideable Tilesets.
	std::vector<VTile*> tiles;

#if _DEBUG
	///Vertex data for displaying collision bounderies of the tilemap.
	sf::VertexArray debuggingVertices;
#endif

	/**
	* Sets up all the tilemap visual infomation.
	* @param graphicFile The filename and path of the texture file.
	* @param tileWidth Frame width of a tile.
	* @param tileHeight Frame height of a tile.
	* @param autoTile If true the tilemap will be rendered using the tilemap mode.
	* @param collision A list of tiles that are collideable in the tilemap data.
	* @param graphicsArea The area of the texture to use. Default is the entire texture area.
	*/
	void setupTilemap(sf::String graphicFile,
		int tileWidth = 0, int tileHeight = 0, bool autoTile = false,
		const std::vector<char>& collision = { '#' }, const sf::IntRect& graphicsArea = sf::IntRect());

	///Updates the transform of the tilemap.
	virtual void updateTransform() override;
	///Updates the tilemap rendering info.
	void updateTilemap();
	///Updates the collision data in the tiles list.
	void updateCollisionBox();
	///Clears all the tilemap data.
	void clearTiles();

public:
	///Used to call parent class functions when they are overrided in class.
	typedef VObject VSUPERCLASS;
	///RenderState for the Tilemap.
	sf::RenderStates RenderState = sf::RenderStates::Default;
	///Size of an individual tile in the tilemap.
	sf::Vector2u TileSize;
	///Scale of the tilemap.
	sf::Vector2f Scale = sf::Vector2f(1, 1);

	///Sets the tilemap to use AutoTile mode or not.
	bool AutoTile = false;

	VTilemap()
	{
		type = VType::TILEMAP; 
		Moves = false;
#if _DEBUG
		DebugColor = sf::Color::Transparent;
		debuggingVertices.setPrimitiveType(sf::Lines);
#endif
	}

	/**
	* Load map from a text file. Maps should be formatted so each tile is separated by comma (,).
	* The amount of tiles on each row makes up the width of the map, and amount of lines the height.
	* 
	* Example:
	* 0,0,0,0,0,0,0,0,0,0,0,0,0
	* 0,0,0,0,0,0,0,0,0,0,0,0,0
	* 1,1,1,0,0,0,0,0,0,0,1,1,1
	* 0,0,0,0,0,0,0,0,0,0,0,0,0
	* 0,0,0,0,1,1,1,1,1,0,0,0,0
	* 0,0,0,0,0,0,0,0,0,0,0,0,0
	* 1,1,1,1,1,1,1,1,1,1,1,1,1
	*
	* @param mapData The path and filename of the csv/text file of the path data.
	* @param graphicFile The path and filename of the texture file.
	* @param tileWidth Frame Width of a single tile.
	* @param tileHeight Frame Height of a single tile.
	* @param autoTile If true, the tilemap will render as an autotiled map.
	* @param collision A list of chars in the mapData that are completely collideable.
	* @param graphicsArea The area of the texture to display. Default is the entire texture.
	*/
	void LoadFromCSV(sf::String mapData, sf::String graphicFile, 
		int tileWidth = 0, int tileHeight = 0, bool autoTile = false,
		const std::vector<char>& collision = { '#' }, const sf::IntRect& graphicsArea = sf::IntRect());

	/**
	* Load map as a single char array.
	*
	* @param mapData The char array that makes up the tilemap data.
	* @param mapWidth Map Width in tiles.
	* @param mapHeight Map Height in tiles.
	* @param graphicFile The path and filename of the texture file.
	* @param tileWidth Frame Width of a single tile.
	* @param tileHeight Frame Height of a single tile.
	* @param autoTile If true, the tilemap will render as an autotiled map.
	* @param collision A list of chars in the mapData that are completely collideable.
	* @param graphicsArea The area of the texture to display. Default is the entire texture.
	*/
	void LoadFromArray(std::vector<char> mapData, int mapWidth, int mapHeight,
		sf::String graphicFile, int tileWidth = 0, int tileHeight = 0, bool autoTile = false,
		const std::vector<char>& collision = { '#' }, const sf::IntRect& graphicsArea = sf::IntRect());

	/**
	* Load map as a single two-dimensional char array.
	*
	* @param mapData The two-dimensional char array that makes up the tilemap data.
	* @param graphicFile The path and filename of the texture file.
	* @param tileWidth Frame Width of a single tile.
	* @param tileHeight Frame Height of a single tile.
	* @param autoTile If true, the tilemap will render as an autotiled map.
	* @param collision A list of chars in the mapData that are completely collideable.
	* @param graphicsArea The area of the texture to display. Default is the entire texture.
	*/
	void LoadFrom2DArray(std::vector<std::vector<char>> mapData, sf::String graphicFile, 
		int tileWidth = 0, int tileHeight = 0, bool autoTile = false,
		const std::vector<char>& collision = { '#' }, const sf::IntRect& graphicsArea = sf::IntRect());

	/**
	* Sets tile to render at specific char in tilemap (ignored if using autotile)
	* @param ID The tile ID as defined in the tilemap data.
	* @param tileNumber The frame index the tile with the specific ID should use.
	* @param autoTileNumber The autotile level the tile should used if in AutoTile (useful if you want to display more than one set of autotiles in a single tilemap).
	*/
	void SetTileRenderID(char ID, int tileNumber = 0, int autoTileNumber = 0);
	
	/**
	* Sets tile with specific char to have certain collision restrictions and responses.
	* @param ID The tile ID as defined in the tilemap data.
	* @param AllowCollisions A filter to set which sides of a tile and object can collide with.
	* @param Callback A callback function to call when a player is either colliding or overlapping a tile with the specified ID.
	*/
	void SetTileCollisionID(char ID, int AllowCollisions = TOUCHALL, std::function<void(VObject*, VObject*)> Callback = nullptr);

	/**
	* Get tile at specified map coordinate.
	* @param x X Coordinate in Tiles.
	* @param y Y Coordinate in Tiles.
	* @return Tile ID as a char value. Returns \0 of coordinates are invalid.
	*/
	char GetTileID(unsigned int x, unsigned int y);
	
	/**
	* Get tile at specified map coordinate.
	* @param position Coordinate in Tiles.
	* @return Tile ID as a char value. Returns \0 of coordinates are invalid.
	*/
	char GetTileID(sf::Vector2i position);
	
	/**
	* Get tile at specified world position.
	* @param tilemapPosition Gets a tile at a specific position in the world (Position / TileSize).
	* @return Tile ID as a char value. Returns \0 of coordinates are invalid.
	*/
	char GetTileIDFromPosition(sf::Vector2f tilemapPosition);

	/**
	* Change the tile at a specific point to an ID.
	* @param x X Coordinate in Tiles.
	* @param y Y Coordinate in Tiles.
	* @param ID The new tile ID for that position in the tilemap.
	*/
	void ChangeTile(int x, int y, char ID);
	
	/**
	* Change a list of tiles to a single ID.
	* @param positions A list of coordinates to change the tile for.
	* @param ID The new tile ID for that position in the tilemap.
	*/
	void ChangeTile(const std::vector<sf::Vector2u>& positions, char ID);

	/**
	* Reset collision infomation to all colliding tiles having default collision properties.
	* The default tile collision allows collisions on all sides with no callback.
	* @param collision List of tile IDs which are collideable.
	*/
	void ResetCollision(const std::vector<char>& collision = { '#' });
	
	/**
	* When object overlaps tilemap, this function goes through all tiles to test overlap with specified overlap.
	* This function is idea to test if an object overlaps any of the collieable tiles instead of using VGlobal::Overlaps (which will only test if the object overlaps the tilemap area).
	* @param object VObject to test for overlap with the tilemap.
	* @param Callback The Callback function that is used if an overlap has been found in the tilemap area (not individual tile sets!).
	* @param FlipCallback If true, will swap the parameters when calling the Callback.
	*/
	virtual bool OverlapWithCallback(VObject* object, std::function<bool(VObject*, VObject*)> Callback = nullptr, bool FlipCallback = false);

	/**
	* Set overall tint of tilemap.
	* @param color The colour tint to set the tilemap to.
	*/
	void SetTint(const sf::Color& color);
	///Get overall tint of tilemap.
	sf::Color const& GetTint();

	///Not used in VTilemap.
	virtual void Reset(sf::Vector2f newPos) override {}
	///Not used in VTilemap.
	virtual void Reset(float x, float y) override {}
	///Not used in VTilemap.
	virtual void Hurt(float damage) override {}
	///Not used in VTilemap.
	virtual void Kill() override {};

	///Deletes and clears all the tilemap data.
	virtual void Destroy() override;

	/**
	* Updates the tilemap. Sets up new render and collision infomation if flags have been set.
	* @param dt Delta Time between the current and previous frames of the game.
	*/
	virtual void Update(float dt) override;

	/**
	* Draws the tilemap.
	* @param RenderTarget The game's sf::RenderTarget object to render data onto.
	*/
	virtual void Draw(sf::RenderTarget& RenderTarget) override;
};

