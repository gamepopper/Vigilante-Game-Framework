#include "VTilemap.h"
#include "VGlobal.h"
#include "VTile.h"

#include <iostream>
#include <sstream>
#include <fstream>
#include <codecvt>

using std::wifstream;
using std::vector;

void VTilemap::setupTilemap(const sf::String& graphicFile, int tileWidth, int tileHeight, bool autoTile,
	const std::vector<char>& collision, const sf::IntRect& graphicsArea)
{
	RenderState.texture = &VGlobal::p()->Content->LoadTexture(graphicFile);
	TileSize.x = tileWidth;
	TileSize.y = tileHeight;
	Size.x *= TileSize.x;
	Size.y *= TileSize.y;

	tileMapWidth = (graphicsArea.width == 0 ? RenderState.texture->getSize().x : graphicsArea.width) / TileSize.x;
	textureOffset = sf::Vector2u(graphicsArea.left, graphicsArea.top);

	AutoTile = autoTile;

	ResetCollision(collision);
}

void VTilemap::updateTransform()
{
	if (Position != last)
	{
		updateCollisionBox();
	}

	float angle = -Angle * VFRAME_PI / 180.f;
	float cosine = static_cast<float>(std::cos(angle));
	float sine = static_cast<float>(std::sin(angle));
	float sxc = Scale.x * cosine;
	float syc = Scale.y * cosine;
	float sxs = Scale.x * sine;
	float sys = Scale.y * sine;
	float tx = -0 * sxc - 0 * sys + (Position).x;
	float ty = 0 * sxs - 0 * syc + (Position).y;

	RenderState.transform =
		sf::Transform(sxc, sys, tx,
			-sxs, syc, ty,
			0.f, 0.f, 1.f);
}

void VTilemap::updateTilemap()
{
	autotile.clear();
	vertices.setPrimitiveType(sf::Quads);

	if (renderDir.empty())
		return;

	int vertexOffset = 0;

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

				quad[0].texCoords = sf::Vector2f(textureOffset + sf::Vector2u(u * TileSize.x,		v * TileSize.y));
				quad[1].texCoords = sf::Vector2f(textureOffset + sf::Vector2u((u + 1) * TileSize.x, v * TileSize.y));
				quad[2].texCoords = sf::Vector2f(textureOffset + sf::Vector2u((u + 1) * TileSize.x, (v + 1) * TileSize.y));
				quad[3].texCoords = sf::Vector2f(textureOffset + sf::Vector2u(u * TileSize.x, (v + 1) * TileSize.y));

				quad[0].color = colour;
				quad[1].color = colour;
				quad[2].color = colour;
				quad[3].color = colour;

				if (vertexOffset < (int)vertices.getVertexCount())
				{
					vertices[vertexOffset + 0] = quad[0];
					vertices[vertexOffset + 1] = quad[1];
					vertices[vertexOffset + 2] = quad[2];
					vertices[vertexOffset + 3] = quad[3];
				}
				else
				{
					vertices.append(quad[0]);
					vertices.append(quad[1]);
					vertices.append(quad[2]);
					vertices.append(quad[3]);
				}

				vertexOffset += 4;
			}
		}

	vertices.resize(vertexOffset);

	dirty = false;
}

void VTilemap::updateCollisionBox()
{
	clearTiles();

	if (collisionDir.empty())
		return;

	vector<bool> processed(mapWidth * mapHeight);

	for (int y = 0; y < mapHeight; y++)
	{
		for (int x = 0; x < mapWidth; x++)
		{
			//If already processed then skip.
			if (processed[(y * mapWidth) + x]) continue;

			char tile = tilemap[(y * mapWidth) + x];

			//If tilemap is not a wall, process then skip.
			if (collisionDir.find(tile) == collisionDir.end())
			{
				processed[(y * mapWidth) + x] = true;
				continue;
			}

			//Any special collision conditions get their own VTile
			if (collisionDir[tile]->AllowCollisions != SidesTouching::TOUCHALL ||
				collisionDir[tile]->Callback != nullptr)
			{
				VTile* t = new VTile(static_cast<float>(x * TileSize.x) + Position.x, static_cast<float>(y * TileSize.y) + Position.y, static_cast<float>(TileSize.x), static_cast<float>(TileSize.y));
				t->AllowCollisions = collisionDir[tile]->AllowCollisions;
				t->Callback = collisionDir[tile]->Callback;
				t->MainTile = tile;
				t->Tilemap = this;
				t->Update(0);
				tiles.push_back(t);
				continue;
			}

			//If tilemap is a wall, find largest width.
			int width = 1, height = 1;
			for (int i = x + 1; i < mapWidth; ++i)
			{
				char nextTile = tilemap[(y * mapWidth) + i];

				//If not wall or already processed, stop here, else increase width.
				if (collisionDir.find(nextTile) != collisionDir.end() && !processed[(y * mapWidth) + i])
				{
					if (collisionDir[nextTile]->AllowCollisions != SidesTouching::TOUCHALL ||
						collisionDir[nextTile]->Callback != nullptr)
					{
						break;
					}
					else
					{
						width++;
						processed[(y * mapWidth) + i] = true;
					}
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
					for (int i = x; i < (x + width); ++i)
					{
						char nextTile = tilemap[(j * mapWidth) + i];
						if (collisionDir.find(nextTile) == collisionDir.end() || processed[(j * mapWidth) + i] == true)
						{
							clear = false;
							break;
						}
						else if (collisionDir[nextTile]->AllowCollisions != SidesTouching::TOUCHALL ||
							collisionDir[nextTile]->Callback != nullptr)
						{
							clear = false;
							break;
						}
						else
						{
							processed[(j * mapWidth) + i] = true;
						}
					}

					//If row is clear, increase height.
					if (clear)
					{
						height++;
					}
					else
					{
						//else, reset flags to unprocessed.
						for (int i = x; i < (x + width); ++i)
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
				tile->Tilemap = this;
				tile->Update(0);
				tiles.push_back(tile);
			}
		}
	}

	last = Position;
}

void VTilemap::clearTiles()
{
	for (VTile* tile : tiles)
	{
		delete tile;
		tile = nullptr;
	}
	tiles.clear();
}

void VTilemap::LoadFromCSV(const sf::String& mapData, const sf::String& graphicFile, int tileWidth, int tileHeight, bool autoTile,
	const std::vector<char>& collision, const sf::IntRect& rect)
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

	setupTilemap(graphicFile, tileWidth, tileHeight, autoTile, collision, rect);
}

void VTilemap::LoadFromArray(const vector<char>& mapData, int mapWidth, int mapHeight,
	const sf::String& graphicFile, int tileWidth, int tileHeight, bool autoTile,
	const std::vector<char>& collision, const sf::IntRect& rect)
{
	tilemap = mapData;

	if (tilemap.size())
	{
		this->mapWidth = mapWidth;
		this->mapHeight = mapHeight;
		Size.x = static_cast<float>(mapWidth);
		Size.y = static_cast<float>(mapHeight);
	}

	setupTilemap(graphicFile, tileWidth, tileHeight, autoTile, collision, rect);
}

void VTilemap::LoadFrom2DArray(vector<vector<char>> mapData, const sf::String& graphicFile, int tileWidth, int tileHeight, bool autoTile,
	const std::vector<char>& collision, const sf::IntRect& rect)
{
	mapWidth = mapData.size() ? mapData[0].size() : 0;
	mapHeight = mapData.size();

	tilemap.clear();
	tilemap.reserve(mapWidth * mapHeight);
	for (int y = 0; y < mapHeight; y++)
	{
		for (int x = 0; x < mapWidth; x++)
		{
			tilemap.emplace_back(mapData[y][x]);
		}
	}

	Size.x = static_cast<float>(mapWidth);
	Size.y = static_cast<float>(mapHeight);

	setupTilemap(graphicFile, tileWidth, tileHeight, autoTile, collision, rect);
}

void VTilemap::SetTileRenderID(char ID, int tileNumber, int autoTileNumber)
{
	VTileRenderInfo* tileInfo = nullptr;
	
	if (renderDir.find(ID) == renderDir.end())
	{
		tileInfo = new VTileRenderInfo();
		renderDir.insert(renderDir.begin(), std::make_pair(ID, tileInfo));
	}
	else
	{
		tileInfo = renderDir[ID];
	}

	tileInfo->TileNumber = tileNumber;
	tileInfo->AutoTileLevel = autoTileNumber;

	dirty = true;
}

void VTilemap::SetTileCollisionID(char ID, int AllowCollisions, std::function<void(VObject*, VObject*)> Callback)
{
	if (collisionDir.find(ID) == collisionDir.end())
	{
		collisionDir[ID] = new VTileCollisionInfo();
	}

	collisionDir[ID]->AllowCollisions = AllowCollisions;
	collisionDir[ID]->Callback = Callback;

	dirty = true;
}

char VTilemap::GetTileID(unsigned int x, unsigned int y) const
{
	if ((int)x < mapWidth && (y * mapWidth) + x < tilemap.size())
	{
		return tilemap[(y * mapWidth) + x];
	}

	return -1;
}

char VTilemap::GetTileID(sf::Vector2i position) const
{
	return GetTileID(position.x, position.y);
}

char VTilemap::GetTileIDFromPosition(sf::Vector2f tilemapPosition) const
{
	return GetTileID((unsigned int)(tilemapPosition.x / TileSize.x), (unsigned int)(tilemapPosition.y / TileSize.y));
}

void VTilemap::ChangeTile(int x, int y, char ID)
{
	if (tilemap[(y * mapWidth) + x] != ID)
	{
		tilemap[(y * mapWidth) + x] = ID;
		dirty = true;
	}
}

void VTilemap::ChangeTile(const std::vector<sf::Vector2u>& positions, char ID)
{
	for (sf::Vector2u pos : positions)
	{
		if (tilemap[(pos.y * mapWidth) + pos.x] != ID)
		{
			tilemap[(pos.y * mapWidth) + pos.x] = ID;
			dirty = true;
		}
	}
}

void VTilemap::ResetCollision(const std::vector<char>& collision)
{
	for (std::pair<char, VTileCollisionInfo*> t : collisionDir)
	{
		delete t.second;
		t.second = nullptr;
	}

	collisionDir.clear();
	
	for (unsigned int i = 0; i < collision.size(); ++i)
		collisionDir[collision[i]] = new VTileCollisionInfo();

	dirty = true;
}

bool VTilemap::OverlapWithCallback(VObject* object, std::function<bool(VObject*, VObject*)> Callback, bool FlipCallback)
{
	bool results = false;

	for (unsigned int i = 0; i < tiles.size(); ++i)
	{
		VTile* t = tiles[i];
		bool overlapFound =
			((object->Position.x + object->Size.x) > t->Position.x) &&
			(object->Position.x < (t->Position.x + t->Size.x)) &&
			((object->Position.y + object->Size.y) > t->Position.y) &&
			(object->Position.y < (t->Position.y + t->Size.y));

		if (Callback != nullptr && t->AllowCollisions != SidesTouching::TOUCHNONE)
		{
			if (FlipCallback)
			{
				overlapFound = Callback(object, t);
			}
			else
			{
				overlapFound = Callback(t, object);
			}
		}

		if (overlapFound)
		{
			if (t->Callback != nullptr)
				t->Callback(t, object);

			return true;
		}
	}

	return results;
}

void VTilemap::SetTint(const sf::Color& color)
{
	if (colour != color)
	{
		colour = color;
		for (unsigned int i = 0; i < vertices.getVertexCount(); ++i)
		{
			vertices[i].color = color;
		}
	}
}

sf::Color const& VTilemap::GetTint()
{
	return colour;
}

int const& VTilemap::GetMapWidth()
{
	return mapWidth;
}

int const& VTilemap::GetMapHeight()
{
	return mapHeight;
}

const VTile* VTilemap::GetCollisionTile(unsigned int index)
{
	if (index >= GetCollisionTileCount())
		return nullptr;

	return tiles[index];
}

const unsigned int VTilemap::GetCollisionTileCount()
{
	return tiles.size();
}

void VTilemap::Destroy()
{
	clearTiles();
	tilemap.clear();

	for (std::pair<char, VTileRenderInfo*> anim : renderDir)
	{
		delete anim.second;
		anim.second = nullptr;
	}
	renderDir.clear();

	autotile.clear();
}

void VTilemap::Update(float dt)
{
	if (!exists || !active)
		return;

	if (AutoTile != helperAutoTile)
	{
		helperAutoTile = AutoTile;
		dirty = true;
	}
}

void VTilemap::Draw(sf::RenderTarget& RenderTarget)
{
	VSUPERCLASS::Draw(RenderTarget);

	if (!exists || !visible)
		return;

	if (dirty)
	{
		updateTilemap();
		updateCollisionBox();
	}

	sf::View renderTargetView = RenderTarget.getView();
	sf::View scrollView = RenderTarget.getDefaultView();

	if (TestInView(renderTargetView, &scrollView, this))
	{
		RenderTarget.setView(scrollView);
		sf::RenderStates states = sf::RenderStates(RenderState);
		states.transform = RenderState.transform;
		RenderTarget.draw(vertices, states);
		RenderTarget.setView(renderTargetView);
	}
}

VTile::VTile(sf::Vector2f position, sf::Vector2f size) : VObject(position, size), Tilemap(nullptr)
{
	Immovable = true;
	Moves = false;
	type = VType::TILE;

	relativePos = Position;

#if _DEBUG
	DebugColor = sf::Color::Red;
#endif
}

VTile::VTile(float x, float y, float width, float height) : VObject(x, y, width, height), Tilemap(nullptr)
{
	Immovable = true;
	Moves = false;
	type = VType::TILE;

	relativePos = Position;

#if _DEBUG
	DebugColor = sf::Color::Red;
#endif
}

void VTile::Update(float dt)
{
	Position = relativePos + Tilemap->Position;
	VSUPERCLASS::Update(dt);
}
