#pragma once
#include "VGroup.h"
#include "V3DObject.h"

class V3DCamera;
class V3DShader;
class V3DBatchModelGroup : public VGroup
{
	V3DObject* baseObject;
	V3DCamera* currentCamera;
	V3DShader* currentShader;

public:
	V3DBatchModelGroup(V3DObject* BaseObject, unsigned int MaxSize = 0);
	virtual void Destroy() override;
	void UpdateShader(V3DCamera* Camera, V3DShader* Shader);
	virtual void Draw(sf::RenderTarget& RenderTarget) override;
};

