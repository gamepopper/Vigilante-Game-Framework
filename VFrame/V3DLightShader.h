/**
* @file    V3DLightShader.h
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
* Shader that has working fragment shading lighting.
*/

#pragma once

#ifndef VFRAME_NO_3D
#include "V3DShader.h"
#include "V3DLight.h"

#include <memory>

class V3DCamera;
///3D Shader with Lighting Support.
class V3DLightShader : public V3DShader
{
public:
	///@param camera Pointer the scene camera.
	V3DLightShader(V3DCamera* camera);
	///List of lights.
	std::unique_ptr<V3DLight> Lights[LIGHT_COUNT];

	///@param camera Pointer the scene camera.
	void SetCamera(V3DCamera* camera);
	virtual void Update();

private:
	V3DCamera* cam;
};
#endif