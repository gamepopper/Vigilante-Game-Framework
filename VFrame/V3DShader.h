#pragma once
#ifndef VFRAME_NO_3D
#include <SFML/OpenGL.hpp>
#include <SFML/System/String.hpp>

enum class ShaderType { Vertex, Fragment, Geometry, Count };
enum class UniformType { TransformPVM, TransformVM, TransformM, Material, Count };
enum class V3DVertexAttribute;

class V3DShader
{
public:
	V3DShader();
	~V3DShader();

	void Bind() const;
	void LoadFromFile(const sf::String& vertexFile, const sf::String& geometryFile, const sf::String& fragmentFile);
	void LoadFromFile(const sf::String& vertexFile, const sf::String& fragmentFile);
	void LoadFromFile(const sf::String& filename, ShaderType type);
	void LoadFromMemory(const std::string& vertexShader, const std::string& geometryShader, const std::string& fragmentShader);
	void LoadFromMemory(const std::string& vertexShader, const std::string& fragmentShader);
	void LoadFromMemory(const std::string& shader, ShaderType type);

	void SetAttribute(V3DVertexAttribute Attribute, const std::string& name);
	virtual void UpdateUniform(UniformType type, void* data);

	GLuint GetProgram();
	GLuint GetShader(ShaderType type);
	GLuint GetUniform(UniformType type);

	virtual void Update();

private:
	GLuint program;
	GLuint shader[static_cast<unsigned int>(ShaderType::Count)];
	GLint uniform[static_cast<unsigned int>(UniformType::Count)];

	static const std::string defaultVertexShader;
	static const std::string defaultFragShader;

	static std::string readFile(const std::string& filename);
	static void checkError(GLuint l_shader, GLuint l_flag, bool l_program, const std::string& l_errorMsg);
	static GLuint buildShader(const std::string& l_src, unsigned int l_type);
};
#endif