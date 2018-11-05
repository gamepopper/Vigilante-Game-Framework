/**
* @file    V3DShader.h
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
* Shader for 3D rendering.
*/

#pragma once
#ifndef VFRAME_NO_3D
#include <SFML/OpenGL.hpp>
#include <SFML/System/String.hpp>

///Shader Types
enum class ShaderType { Vertex, Fragment, Geometry, Count };
///Standard Uniforms in the shader.
enum class UniformType { TransformPVM, TransformVM, TransformM, Material, Count };
enum class V3DVertexAttribute;

class V3DCamera;

///Base class for rendering 3D shaders.
class V3DShader
{
public:
	V3DShader();
	~V3DShader();

	///Bind texture to the current scene.
	void Bind() const;

	/**
	* Loads shader from a file.
	* @param vertexFile The file path and name of the vertex file.
	* @param geometryFile The file path and name of the geometry file.
	* @param fragmentFile The file path and name of the fragment file.
	*/
	void LoadFromFile(const sf::String& vertexFile, const sf::String& geometryFile, const sf::String& fragmentFile);

	/**
	* Loads shader from a file.
	* @param vertexFile The file path and name of the vertex file.
	* @param fragmentFile The file path and name of the fragment file.
	*/
	void LoadFromFile(const sf::String& vertexFile, const sf::String& fragmentFile);

	/**
	* Loads shader from a file.
	* @param filename The file path and name of the shader file.
	* @param type The type of shader this is.
	*/
	void LoadFromFile(const sf::String& filename, ShaderType type);
	
	/**
	* Loads shader from memory (like a string).
	* @param vertexShader The vertex shader data.
	* @param geometryShader The geometry shader data.
	* @param fragmentShader The fragment shader data.
	*/
	void LoadFromMemory(const std::string& vertexShader, const std::string& geometryShader, const std::string& fragmentShader);

	/**
	* Loads shader from memory (like a string).
	* @param vertexShader The vertex shader data.
	* @param fragmentShader The fragment shader data.
	*/
	void LoadFromMemory(const std::string& vertexShader, const std::string& fragmentShader);

	/**
	* Loads shader from memory (like a string).
	* @param shader The vertex shader data.
	* @param type The type of shader this is.
	*/
	void LoadFromMemory(const std::string& shader, ShaderType type);

	/**
	* Set the name of a vertex attribute in a shader.
	* @param Attribute Vertex Attribute Type.
	* @param name Name of the variable in the shader this attribute associates with.
	*/
	void SetAttribute(V3DVertexAttribute Attribute, const std::string& name);

	/**
	* Set the name of a vertex attribute in a shader.
	* @param type The Uniform type.
	* @param data The data you want to set the Uniform to.
	*/
	virtual void UpdateUniform(UniformType type, void* data);

	///@return The shader program ID.
	GLuint GetProgram();
	
	/**
	* @param type The Shader Type.
	* @return The shader ID.
	*/
	GLuint GetShader(ShaderType type);

	/**
	* @param type The Uniform type.
	* @return The uniform ID.
	*/
	GLuint GetUniform(UniformType type);

	///@param camera Pointer to the currently referenced Camera.
	void SetCamera(V3DCamera * camera);

	///Update the shader.
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

protected:
	///Reference pointer for current V3DCamera.
	V3DCamera* cam;
};
#endif