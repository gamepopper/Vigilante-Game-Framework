#include "VTilemap.h"
#include "VGlobal.h"

#include <iostream>
#include <sstream>
#include <fstream>
#include <codecvt>

using std::wifstream;
using std::vector;

void VTilemap::setupTilemap(sf::String graphicFile, int tileWidth, int tileHeight, bool autoTile,
	vector<char> collideFilter)
{
	if (VGlobal::p()->Content->LoadTexture(graphicFile, texture))
	{
		TileSize.x = tileWidth;
		TileSize.y = tileHeight;
		Size.x *= TileSize.x;
		Size.y *= TileSize.y;
		collisionFilter = collideFilter;

		tileMapWidth = texture.getSize().x / TileSize.x;

		AutoTile = autoTile;

		updateCollisionBox();

		dirty = true;
	}
}

void VTilemap::updateTransform()
{
	if (Position != transformable.getPosition())
	{
		transformable.setPosition(Position);
		updateCollisionBox();
	}

	transformable.setRotation(Angle);
	transformable.setScale(Scale);
}

void VTilemap::updateTilemap()
{
	autotile.clear();

	if (AutoTile)
	{
		autotile = vector<int>(mapWidth * mapHeight);
		for (int y = 0; y < mapHeight; y++)
		{
			for (int x = 0; x < mapWidth; x++)
			{
				if (renderDir.find(tilemap[(y * mapWidth) + x]) != renderDir.end())
				{
					int tileValue = 0;

					if (y == 0) tileValue += 1;
					else if (renderDir.find(tilemap[((y - 1) * mapWidth) + x]) != renderDir.end()) tileValue += 1;

					if (x == mapWidth - 1) tileValue += 2;
					else if (renderDir.find(tilemap[(y * mapWidth) + x + 1]) != renderDir.end()) tileValue += 2;

					if (y == mapHeight - 1) tileValue += 4;
					else if (renderDir.find(tilemap[((y + 1) * mapWidth) + x]) != renderDir.end()) tileValue += 4;

					if (x == 0) tileValue += 8;
					else if (renderDir.find(tilemap[(y * mapWidth) + x - 1]) != renderDir.end()) tileValue += 8;

					autotile[(y * mapWidth) + x] = tileValue;
				}
			}
		}
	}

	vertices.clear();
	vertices.setPrimitiveType(sf::Quads);

	for (int y = 0; y < mapHeight; y++)
		for (int x = 0; x < mapWidth; x++)
		{
			if (renderDir.find(tilemap[(y * mapWidth) + x]) != renderDir.end())
			{
				sf::Vertex quad[4];
				quad[0] = sf::Vertex();
				quad[1] = sf::Vertex();
				quad[2] = sf::Vertex();
				quad[3] = sf::Vertex();

				quad[0].position = sf::Vector2f(sf::Vector2u(x * TileSize.x,		y * TileSize.y));
				quad[1].position = sf::Vector2f(sf::Vector2u((x + 1) * TileSize.x,	y * TileSize.y));
				quad[2].position = sf::Vector2f(sf::Vector2u((x + 1) * TileSize.x,	(y + 1) * TileSize.y));
				quad[3].position = sf::Vector2f(sf::Vector2u(x * TileSize.x,		(y + 1) * TileSize.y));

				int TileID = AutoTile ? autotile[(y * mapWidth) + x] + (renderDir[tilemap[(y * mapWidth) + x]]->AutoTileLevel * 16) : 
					renderDir[tilemap[(y * mapWidth) + x]]->TileNumber;
				int u = TileID % tileMapWidth;
				int v = TileID / tileMapWidth;

				quad[0].texCoords = sf::Vector2f(sf::Vector2u(u * TileSize.x,		v * TileSize.y));
				quad[1].texCoords = sf::Vector2f(sf::Vector2u((u + 1) * TileSize.x, v * TileSize.y));
				quad[2].texCoords = sf::Vector2f(sf::Vector2u((u + 1) * TileSize.x, (v + 1) * TileSize.y));
				quad[3].texCoords = sf::Vector2f(sf::Vector2u(u * TileSize.x,		(v + 1) * TileSize.y));

				quad[0].color = colour;
				quad[1].color = colour;
				quad[2].color = colour;
				quad[3].color = colour;

				vertices.append(quad[0]);
				vertices.append(quad[1]);
				vertices.append(quad[2]);
				vertices.append(quad[3]);
			}
		}

	dirty = false;
}

void VTilemap::updateCollisionBox()
{
	vector<bool> processed(mapWidth * mapHeight);
	clearTiles();

	for (int y = 0; y < mapHeight; y++)
	{
		for (int x = 0; x < mapWidth; x++)
		{
			//If already processed then skip.
			if (processed[(y * mapWidth) + x]) continue;

			//If tilemap is not a wall, process then skip.
			if (find(collisionFilter.begin(), collisionFilter.end(), tilemap[(y * mapWidth) + x]) != collisionFilter.end())
			{
				processed[(y * mapWidth) + x] = true;
				continue;
			}

			//If tilemap is a wall, find largest width.
			int width = 1, height = 1;
			for (int i = x + 1; i < mapWidth; i++)
			{
				//If not wall or already processed, stop here, else increase width.
				if (find(collisionFilter.begin(), collisionFilter.end(), tilemap[(y * mapWidth) + i]) == collisionFilter.end() && !processed[(y * mapWidth) + i])
				{
					width++;
					processed[(y * mapWidth) + i] = true;
				}
				else
				{
					break;
				}
			}

			//If width greater than 0, find largest height.
			if (width > 0)
			{
				for (int j = y + 1; j < mapHeight; j++)
				{

					bool clear = true;
					for (int i = x; i < (x + width); i++)
					{
						if (find(collisionFilter.begin(), collisionFilter.end(), tilemap[(j * mapWidth) + i]) != collisionFilter.end() || processed[(j * mapWidth) + i] == true)
						{
							clear = false;
							break;
						}
						processed[(j * mapWidth) + i] = true;
					}

					//If row is clear, increase height.
					if (clear)
					{
						height++;
					}
					else
					{
						//else, reset flags to unprocessed.
						for (int i = x; i < (x + width); i++)
						{
							processed[(j * mapWidth) + i] = false;
						}
						break;
					}
				}
			}

			//If width and height are greater than 0, push Rectangle to Open Spaces.
			if (width * height > 0)
			{
				VTile* tile = new VTile(static_cast<float>(x * TileSize.x) + Position.x, static_cast<float>(y * TileSize.y) + Position.y, static_cast<float>(width * TileSize.x), static_cast<float>(height * TileSize.y));
				tile->Update(0);
				Tiles.push_back(tile);
			}
		}
	}
}

void VTilemap::clearTiles()
{
	for each (VTile* tile in Tiles)
	{
		delete tile;
		tile = NULL;
	}
	Tiles.clear();
}

void VTilemap::LoadFromCSV(sf::String mapData, sf::String graphicFile, int tileWidth, int tileHeight, bool autoTile,
	vector<char> collideFilter)
{
	tilemap.clear();
	std::locale ulocale(std::locale(), new std::codecvt_utf8<wchar_t>);
	std::ifstream file(mapData.toAnsiString());
	file.imbue(ulocale);

	std::string line;
	while (std::getline(file, line))
	{
		Size.x = 0;
		for (unsigned int i = 0; i < line.length(); i+=2)
		{
			tilemap.push_back(line[i]);
			Size.x++;
		}
		Size.y++;
	}

	mapWidth = static_cast<int>(Size.x);
	mapHeight = static_cast<int>(Size.y);

	setupTilemap(graphicFile, tileWidth, tileHeight, autoTile, collideFilter);
}

void VTilemap::LoadFromArray(vector<char> mapData, int mapWidth, int mapHeight,
	sf::String graphicFile, int tileWidth, int tileHeight, bool autoTile,
	vector<char> collideFilter)
{
	tilemap = mapData;

	if (tilemap.size())
	{
		this->mapWidth = mapWidth;
		this->mapHeight = mapHeight;
		Size.x = static_cast<float>(mapWidth);
		Size.y = static_cast<float>(mapHeight);
	}

	setupTilemap(graphicFile, tileWidth, tileHeight, autoTile, collideFilter);
}

void VTilemap::LoadFrom2DArray(vector<vector<char>> mapData, sf::String graphicFile, int tileWidth, int tileHeight, bool autoTile,
	vector<char> collideFilter)
{
	mapWidth = mapData.size() ? mapData[0].size() : 0;
	mapHeight = mapData.size();

	tilemap = vector<char>(mapWidth * mapHeight);
	for (int y = 0; y < mapHeight; y++)
	{
		for (int x = 0; x < mapWidth; x++)
		{
			tilemap[(y * mapWidth) + x] = mapData[y][x];
		}
	}

	Size.x = static_cast<float>(mapWidth);
	Size.y = static_cast<float>(mapHeight);

	setupTilemap(graphicFile, tileWidth, tileHeight, autoTile, collideFilter);
}

void VTilemap::SetTint(const sf::Color& color)
{
	if (colour != color)
	{
		colour = color;
		for (unsigned int i = 0; i < vertices.getVertexCount(); i++)
		{
			vertices[i].color = color;
		}
	}
}

void VTilemap::Destroy()
{
	clearTiles();
	tilemap.clear();

	for each (std::pair<char, VTileRenderInfo*> anim in renderDir)
	{
		delete anim.second;
		anim.second = NULL;
	}
	renderDir.clear();

	autotile.clear();
}

void VTilemap::Update(float dt)
{
	if (AutoTile != helperAutoTile)
	{
		helperAutoTile = AutoTile;
		dirty = true;
	}

	if (dirty)
	{
		updateTilemap();
	}
}

void VTilemap::Draw(sf::RenderTarget& RenderTarget)
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
		sf::RenderStates states = sf::RenderStates(RenderState);
		states.texture = &texture;
		states.transform *= transformable.getTransform();
		RenderTarget.draw(vertices, states);
		RenderTarget.setView(renderTargetView);
	}
}