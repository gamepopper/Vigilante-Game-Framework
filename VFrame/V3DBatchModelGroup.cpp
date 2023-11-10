#include "V3DBatchModelGroup.h"

#ifndef VFRAME_NO_3D
#include "V3DCamera.h"
#include "V3DShader.h"

V3DBatchModelGroup::V3DBatchModelGroup(V3DObject* BaseObject, unsigned int MaxSize) : VGroup(MaxSize)
{
	baseObject = BaseObject;
}

void V3DBatchModelGroup::Destroy()
{
	VSUPERCLASS::Destroy();
	delete baseObject;
}

V3DShader* V3DBatchModelGroup::GetOverrideShader()
{
	return baseObject->OverrideShader;
}

void V3DBatchModelGroup::UpdateShader(V3DCamera* Camera, V3DShader* Shader)
{
	currentCamera = Camera;
	currentShader = Shader;
}

void V3DBatchModelGroup::Draw(sf::RenderTarget& RenderTarget)
{
	for (VBase* it : members)
	{
		V3DObject* base = dynamic_cast<V3DObject*>(it);

		if (base != nullptr && base->exists && base->visible && currentCamera->BoxInView(base->Position, baseObject->GetMinimum(), baseObject->GetMaximum()))
		{
			baseObject->Position = base->Position;
			baseObject->Rotation = base->Rotation;
			baseObject->Scale = base->Scale;
			baseObject->UpdateShader(currentShader, currentCamera);
			baseObject->Update(0.0f);
			baseObject->Draw(RenderTarget);
		}
	}
}
#endif