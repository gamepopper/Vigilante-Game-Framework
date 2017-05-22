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

struct VTileRenderInfo
{
	int TileNumber;
	int AutoTileLevel;
};

class VTile;

class VTilemap : public VObject
{
protected:
	std::vector<char> tilemap;
	std::map<char, VTileRenderInfo*> renderDir;
	std::vector<int> autotile;
	std::vector<char> collisionFilter;
	int tileMapWidth = 0;

	int mapWidth = 0;
	int mapHeight = 0;

	bool dirty = true;
	bool helperAutoTile = false;
	
	sf::Texture texture;
	sf::VertexArray vertices;
	sf::Transformable transformable;
	sf::Color colour = sf::Color::White;

	void setupTilemap(sf::String graphicFile,
		int tileWidth = 0, int tileWeight = 0, bool autoTile = false,
		std::vector<char> collideFilter = { '.' });

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
		type = VType::TILE; 
		Moves = false;
#if _DEBUG
		DebugColor = sf::Color(255, 0, 0, 128);
#endif
	}

	void LoadFromCSV(sf::String mapData, sf::String graphicFile, 
		int tileWidth = 0, int tileWeight = 0, bool autoTile = false,
		std::vector<char> collideFilter = { '.' });

	void LoadFromArray(std::vector<char> mapData, int mapWidth, int mapHeight,
		sf::String graphicFile, int tileWidth = 0, int tileWeight = 0, bool autoTile = false,
		std::vector<char> collideFilter = { '.' });

	void LoadFrom2DArray(std::vector<std::vector<char>> mapData, sf::String graphicFile, 
		int tileWidth = 0, int tileWeight = 0, bool autoTile = false,
		std::vector<char> collideFilter = { '.' });

	//Sets tile to render at specific char in tilemap (ignored if using autotile)
	void SetTileRenderID(char ID, int tileNumber = 0, int autoTileNumber = 0)
	{
		VTileRenderInfo* tileInfo = new VTileRenderInfo();
		tileInfo->TileNumber = tileNumber;
		tileInfo->AutoTileLevel = autoTileNumber;

		renderDir.insert(renderDir.begin(), std::pair<char, VTileRenderInfo*>(ID, tileInfo));
		dirty = true;
	}

	char GetTileID(unsigned int x, unsigned int y)
	{
		if ((y * mapWidth) + x < tilemap.size())
		{
			return tilemap[(y * mapWidth) + x];
		}

		return '\0';
	}

	char GetTileID(sf::Vector2i position)
	{
		return GetTileID(position.x, position.y);
	}

	char GetTileIDFromPosition(sf::Vector2f tilemapPosition)
	{
		return GetTileID((unsigned int)(tilemapPosition.x / TileSize.x), (unsigned int)(tilemapPosition.y / TileSize.y));
	}

	void ChangeTile(int x, int y, char ID)
	{
		tilemap[(y * mapWidth) + x] = ID;

		updateTilemap();
		updateCollisionBox();
		dirty = true;
	}

	void ChangeTile(const std::vector<sf::Vector2u>& positions, char ID)
	{
		for(sf::Vector2u pos : positions)
		{
			tilemap[(pos.y * mapWidth) + pos.x] = ID;
		}

		updateTilemap();
		updateCollisionBox();
		dirty = true;
	}

	void UpdateCollisionFilter(std::vector<char> collideFilter)
	{
		collisionFilter = collideFilter;
		updateCollisionBox();
		dirty = true;
	}

	void SetTint(const sf::Color& color);
	sf::Color const& GetTint()
	{
		return colour;
	}

	virtual void Reset(sf::Vector2f newPos) {}
	virtual void Reset(float x, float y) {}
	virtual void Hurt(float damage) {}
	virtual void Kill() {};

	virtual void Destroy() override;
	virtual void Update(float dt) override;
	virtual void Draw(sf::RenderTarget& RenderTarget) override;
};

