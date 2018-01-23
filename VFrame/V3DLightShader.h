#pragma once
#include "V3DShader.h"
#include "V3DLight.h"

#include <memory>

class V3DCamera;
class V3DLightShader : public V3DShader
{
public:
	V3DLightShader(V3DCamera* camera);
	std::unique_ptr<V3DLight> Lights[LIGHT_COUNT];

	void SetCamera(V3DCamera* camera);
	virtual void Update();

private:
	V3DCamera* cam;
};

