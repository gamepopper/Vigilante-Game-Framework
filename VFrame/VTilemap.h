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

struct VTileRenderInfo
{
	int TileNumber;
	int AutoTileLevel;
};

struct VTileCollisionInfo
{
	uint8_t AllowCollisions = SidesTouching::TOUCHALL;
	std::function<void(VObject*, VObject*)> Callback = nullptr;
};

class VTile;

class VTilemap : public VObject
{
protected:
	std::vector<char> tilemap;
	std::map<char, VTileCollisionInfo*> collisionDir;
	std::map<char, VTileRenderInfo*> renderDir;
	std::vector<int> autotile;
	int tileMapWidth = 0;

	int mapWidth = 0;
	int mapHeight = 0;

	bool dirty = true;
	bool helperAutoTile = false;
	
	sf::Texture texture;
	sf::VertexArray vertices;
	sf::Transformable transformable;
	sf::Color colour = sf::Color::White;

#if _DEBUG
	sf::VertexArray debuggingVertices;
#endif

	void setupTilemap(sf::String graphicFile,
		int tileWidth = 0, int tileWeight = 0, bool autoTile = false,
		const std::vector<char>& collision = { '#' });

	virtual void updateTransform() override;
	void updateTilemap();
	void updateCollisionBox();
	void clearTiles();

public:
	typedef VObject VSUPERCLASS;
	sf::RenderStates RenderState = sf::RenderStates::Default;
	sf::Vector2u TileSize;
	std::vector<VTile*> Tiles;
	sf::Vector2f Scale = sf::Vector2f(1, 1);

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

	//Load map from a text file. Maps should be formatted so each tile is separated by comma (,).
	//The amount of tiles on each row makes up the width of the map, and amount of lines the height.
	void LoadFromCSV(sf::String mapData, sf::String graphicFile, 
		int tileWidth = 0, int tileWeight = 0, bool autoTile = false,
		const std::vector<char>& collision = { '#' });

	//Load map from one-dimensional array.
	void LoadFromArray(std::vector<char> mapData, int mapWidth, int mapHeight,
		sf::String graphicFile, int tileWidth = 0, int tileWeight = 0, bool autoTile = false,
		const std::vector<char>& collision = { '#' });

	//Load map from two-dimensional array.
	void LoadFrom2DArray(std::vector<std::vector<char>> mapData, sf::String graphicFile, 
		int tileWidth = 0, int tileWeight = 0, bool autoTile = false,
		const std::vector<char>& collision = { '#' });

	//Sets tile to render at specific char in tilemap (ignored if using autotile)
	void SetTileRenderID(char ID, int tileNumber = 0, int autoTileNumber = 0);
	//Sets tile with specific char to have certain collision restrictions and responses.
	void SetTileCollisionID(char ID, int AllowCollisions = TOUCHALL, std::function<void(VObject*, VObject*)> Callback = nullptr);

	//Get tile at specified map coordinate.
	char GetTileID(unsigned int x, unsigned int y);
	//Get tile at specified map coordinate.
	char GetTileID(sf::Vector2i position);
	//Get tile at specified world position.
	char GetTileIDFromPosition(sf::Vector2f tilemapPosition);

	//Change the tile at a specific point to an ID.
	void ChangeTile(int x, int y, char ID);
	//Change a list of tiles to a single ID.
	void ChangeTile(const std::vector<sf::Vector2u>& positions, char ID);

	//Reset collision infomation to all colliding tiles having default collision properties.
	void ResetCollision(const std::vector<char>& collision = { '#' });
	//When object overlaps tilemap, this function goes through all tiles to test overlap with specified overlap.
	virtual bool OverlapWithCallback(VObject* object, std::function<bool(VObject*, VObject*)> Callback, bool FlipCallback = false);

	//Set overall tint of tilemap.
	void SetTint(const sf::Color& color);
	//Get overall tint of tilemap.
	sf::Color const& GetTint();

	virtual void Reset(sf::Vector2f newPos) {}
	virtual void Reset(float x, float y) {}
	virtual void Hurt(float damage) {}
	virtual void Kill() {};

	virtual void Destroy() override;
	virtual void Update(float dt) override;
	virtual void Draw(sf::RenderTarget& RenderTarget) override;
};

