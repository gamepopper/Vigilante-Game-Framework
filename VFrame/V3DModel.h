#pragma once
#include "V3DObject.h"
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/String.hpp>
#include <vector>
#include <memory>

#include "depend/glm/vec2.hpp"
#include "depend/glm/vec3.hpp"
#include "depend/glm/vec4.hpp"
#include "depend/glm/matrix.hpp"

enum class V3DVertexAttribute { Position, Normal, Color, TexCoord, COUNT };

struct V3DVertex
{
	V3DVertex(const glm::vec3& pos) : position(pos), color(1,1,1,1) {}
	V3DVertex(glm::vec3& pos, glm::vec3& norm) : position(pos), normal(norm), color(1, 1, 1, 1) {}
	V3DVertex(glm::vec3& pos, glm::vec2& tex) : position(pos), color(1, 1, 1, 1), texCoord(tex) {}
	V3DVertex(glm::vec3& pos, glm::vec4& tint) : position(pos), color(tint) {}

	V3DVertex(glm::vec3& pos, glm::vec3& norm, glm::vec2& tex) : position(pos), normal(norm), color(1, 1, 1, 1), texCoord(tex) {}
	V3DVertex(glm::vec3& pos, glm::vec3& norm, glm::vec4& tint) : position(pos), normal(norm), color(tint) {}
	V3DVertex(glm::vec3& pos, glm::vec2& tex,  glm::vec4& tint) : position(pos), texCoord(tex), color(tint) {}

	V3DVertex(glm::vec3& pos, glm::vec3& norm, glm::vec4& tint, glm::vec2& tex) : position(pos), normal(norm), color(tint), texCoord(tex) {}

	glm::vec3 position;
	glm::vec3 normal;
	glm::vec4 color;
	glm::vec2 texCoord;
};

typedef std::vector<V3DVertex> V3DVertexArray;

struct V3DMaterial;
class V3DModel : public V3DObject
{
protected:
	virtual void updateTransform();
	GLuint vao;
	GLuint vertexVBO;
	GLuint indexVBO;
	unsigned int drawCount;

	sf::Texture texture;
	glm::mat4 transform;

public:
	typedef V3DObject VSUPERCLASS;

	V3DMaterial* Material;

	V3DModel(sf::Vector3f position, sf::Vector3f rotation, sf::Vector3f scale);

	V3DModel(float posX = 0, float posY = 0, float posZ = 0,
		float rotX = 0, float rotY = 0, float rotZ = 0,
		float scaleX = 1, float scaleY = 1, float scaleZ = 1);

	bool LoadModelData(const V3DVertexArray& vertexArray, const std::vector<unsigned int>& indexArray = std::vector<unsigned int>());
	bool LoadTexture(const sf::String& filename);
	virtual void UpdateShader(V3DShader* shader, V3DCamera* camera);

	virtual void Destroy();
	virtual void Draw(sf::RenderTarget& RenderTarget) override;

	static GLuint DefaultTexture;
	static void GenerateDefaultTexture();
};