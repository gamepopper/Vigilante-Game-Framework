#ifndef VFRAME_NO_3D

#include <GL/glew.h>

#include "V3DModel.h"
#include "V3DShader.h"
#include "V3DLight.h"
#include "VBase.h"
#include <fstream>

const std::string V3DShader::defaultVertexShader =
"#version 330\n"\
"attribute vec3 position;"\
"attribute vec4 color;" \
"attribute vec2 texCoord;" \
"uniform mat4 pvm;" \

"varying vec4 tint;" \
"varying vec2 uv;" \

"void main() {"\
"	gl_Position = pvm * vec4(position, 1.0);"\
"	tint = color;"\
"	uv = texCoord;"\
"}";

const std::string V3DShader::defaultFragShader =
"#version 330\n" \
"uniform sampler2D texture;" \
"varying vec4 tint;" \
"varying vec2 uv;" \

"void main() {" \
"	gl_FragColor = texture2D(texture, uv) * tint;" \
"}";

V3DShader::V3DShader() : program(0)
{
	program = 0;
	memset(shader, NULL, sizeof(shader));
	memset(uniform, NULL, sizeof(uniform));
}

V3DShader::~V3DShader()
{
	for (unsigned int i = 0; i < static_cast<unsigned int>(ShaderType::Count); i++)
	{
		glDetachShader(program, shader[i]);
		glDeleteShader(shader[i]);
	}

	glDeleteProgram(program);
}

void V3DShader::Bind() const
{
	glUseProgram(program);
}

void V3DShader::LoadFromFile(const sf::String& filename, ShaderType type)
{
	std::string data = readFile(filename);

	if (data == "")
	{
		VBase::VLogError("Bad shader file: %s", filename);
		return;
	}

	LoadFromMemory(data, type);
}

void V3DShader::LoadFromFile(const sf::String& vertexFile, const sf::String& fragmentFile)
{
	LoadFromFile(vertexFile, ShaderType::Vertex);
	LoadFromFile(fragmentFile, ShaderType::Fragment);
}

void V3DShader::LoadFromFile(const sf::String& vertexFile, const sf::String& geometryFile, const sf::String& fragmentFile)
{
	LoadFromFile(vertexFile, ShaderType::Vertex);
	LoadFromFile(fragmentFile, ShaderType::Fragment);
	LoadFromFile(geometryFile, ShaderType::Geometry);
}

void V3DShader::LoadFromMemory(const std::string& shaderData, ShaderType type)
{
	if (shaderData.empty())
		return;

	if (shader[static_cast<unsigned int>(type)])
	{
		glDetachShader(program, shader[static_cast<unsigned int>(type)]);
		glDeleteShader(shader[static_cast<unsigned int>(type)]);
	}

	switch (type)
	{
	case ShaderType::Vertex:
		shader[static_cast<unsigned int>(type)] = buildShader(shaderData, GL_VERTEX_SHADER);
		break;
	case ShaderType::Geometry:
		shader[static_cast<unsigned int>(type)] = buildShader(shaderData, GL_GEOMETRY_SHADER);
		break;
	case ShaderType::Fragment:
		shader[static_cast<unsigned int>(type)] = buildShader(shaderData, GL_FRAGMENT_SHADER);
		break;
	default:
		VBase::VLogError("Invalid Shader Type");
		break;
	}

	if (program == 0)
	{
		program = glCreateProgram();
	}

	glAttachShader(program, shader[static_cast<unsigned int>(type)]);
	glBindAttribLocation(program, static_cast<GLuint>(V3DVertexAttribute::Position), "position");
	glBindAttribLocation(program, static_cast<GLuint>(V3DVertexAttribute::Normal), "normal");
	glBindAttribLocation(program, static_cast<GLuint>(V3DVertexAttribute::Color), "color");
	glBindAttribLocation(program, static_cast<GLuint>(V3DVertexAttribute::TexCoord), "texCoord");

	glLinkProgram(program);
	checkError(program, GL_LINK_STATUS, true, "Shader link error:");
	glValidateProgram(program);
	checkError(program, GL_VALIDATE_STATUS, true, "Invalid shader:");

	uniform[static_cast<unsigned int>(UniformType::TransformPVM)] = glGetUniformLocation(program, "pvm");
	uniform[static_cast<unsigned int>(UniformType::TransformVM)] = glGetUniformLocation(program, "viewModel");
	uniform[static_cast<unsigned int>(UniformType::TransformM)] = glGetUniformLocation(program, "transform");
	uniform[static_cast<unsigned int>(UniformType::Material)] = glGetUniformLocation(program, "material.specular");
}

void V3DShader::LoadFromMemory(const std::string& vertexShader, const std::string& fragmentShader)
{
	LoadFromMemory(vertexShader, ShaderType::Vertex);
	LoadFromMemory(fragmentShader, ShaderType::Fragment);
}

void V3DShader::LoadFromMemory(const std::string& vertexShader, const std::string& geometryShader, const std::string& fragmentShader)
{
	LoadFromMemory(vertexShader, ShaderType::Vertex);
	LoadFromMemory(fragmentShader, ShaderType::Fragment);
	LoadFromMemory(geometryShader, ShaderType::Geometry);
}

void V3DShader::SetAttribute(V3DVertexAttribute Attribute, const std::string& name)
{
	glBindAttribLocation(program, static_cast<GLuint>(Attribute), name.c_str());
}

void V3DShader::Update()
{
	if (program == 0)
		LoadFromMemory(defaultVertexShader, defaultFragShader);
}

void V3DShader::UpdateUniform(UniformType type, void* data)
{
	if (uniform[static_cast<int>(type)] < 0)
		return;

	switch (type)
	{
	case UniformType::TransformPVM:
		glUniformMatrix4fv(uniform[static_cast<int>(type)], 1, GL_FALSE, static_cast<GLfloat*>(data));
		break;
	case UniformType::TransformVM:
		glUniformMatrix4fv(uniform[static_cast<int>(type)], 1, GL_FALSE, static_cast<GLfloat*>(data));
		break;
	case UniformType::TransformM:
		glUniformMatrix4fv(uniform[static_cast<int>(type)], 1, GL_FALSE, static_cast<GLfloat*>(data));
		break;
	case UniformType::Material:
		V3DMaterial* mat = static_cast<V3DMaterial*>(data);
		glUniform3f(uniform[static_cast<int>(type)], mat->Specular.x, mat->Specular.y, mat->Specular.z);
		glUniform1f(glGetUniformLocation(program, "material.shininess"), mat->Shininess);
		break;
	}
}

GLuint V3DShader::GetProgram()
{
	return program;
}

GLuint V3DShader::GetShader(ShaderType type)
{
	return shader[static_cast<GLuint>(type)];
}

GLuint V3DShader::GetUniform(UniformType type)
{
	return uniform[static_cast<GLuint>(type)];
}


std::string V3DShader::readFile(const std::string& filename)
{
	std::ifstream file(filename);
	if (!file.is_open()) { return ""; }
	std::string output;
	std::string line;
	while (std::getline(file, line)) {
		output.append(line + '\n');
	}
	file.close();
	return output;
}

void V3DShader::checkError(GLuint l_shader, GLuint l_flag, bool l_program, const std::string& l_errorMsg)
{
	GLint success = 0;
	GLchar error[1024] = { 0 };
	if (l_program) { glGetProgramiv(l_shader, l_flag, &success); }
	else { glGetShaderiv(l_shader, l_flag, &success); }

	if (success) { return; }
	if (l_program) {
		glGetProgramInfoLog(l_shader, sizeof(error), nullptr, error);
	}
	else {
		glGetShaderInfoLog(l_shader, sizeof(error), nullptr, error);
	}

	VBase::VLogError(l_errorMsg.c_str());
}

GLuint V3DShader::buildShader(const std::string& l_src, unsigned int l_type)
{
	GLuint shaderID = glCreateShader(l_type);
	if (!shaderID) {
		VBase::VLogError("Bad shader type!"); 
		return 0;
	}
	const GLchar* sources[1];
	GLint lengths[1];
	sources[0] = l_src.c_str();
	lengths[0] = l_src.length();
	glShaderSource(shaderID, 1, sources, lengths);
	glCompileShader(shaderID);
	checkError(shaderID, GL_COMPILE_STATUS, false, "Shader compile error: ");
	return shaderID;
}
#endif