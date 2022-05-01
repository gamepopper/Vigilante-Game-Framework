/**
* @file    V3DBatchModelGroup.h
* @author  Tim Stoddard <tim@gamepopper.co.uk>
*
* @section LICENSE
*
* MIT License
*
* Copyright (c) 2020 Tim Stoddard
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
* Special group that will render multiple instances of a single model.
*/

#pragma once
#ifndef VFRAME_NO_3D
#include "VGroup.h"
#include "V3DObject.h"

class V3DCamera;
class V3DShader;
///Renders multiple instances of a single base 3D renderable object.
class V3DBatchModelGroup : public VGroup
{
	V3DObject* baseObject;
	V3DCamera* currentCamera;
	V3DShader* currentShader;

public:

	/**
	* @param BaseObject The base 3D object that is used for rendering multiple instances.
	* @param MaxSize The fixed length of the group. If 0, then the VGroup has no limit in size.
	*/
	V3DBatchModelGroup(V3DObject* BaseObject, unsigned int MaxSize = 0);

	///Revives all dead objects before reviving the group itself.
	virtual void Destroy() override;

	V3DShader* GetOverrideShader();

	/**
	* Updates the shader with the current camera and object specific values.
	* @param shader The scene shader.
	* @param camera The scene camera.
	*/
	void UpdateShader(V3DCamera* camera, V3DShader* shader);

	virtual void Draw(sf::RenderTarget& RenderTarget) override;
};
#endif