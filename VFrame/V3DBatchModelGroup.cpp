#include "V3DBatchModelGroup.h"

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

void V3DBatchModelGroup::UpdateShader(V3DCamera* Camera, V3DShader* Shader)
{
	currentCamera = Camera;
	currentShader = Shader;
}

void V3DBatchModelGroup::Draw(sf::RenderTarget& RenderTarget)
{
	for (unsigned int i = 0; i < members.size(); i++)
	{
		V3DObject* base = dynamic_cast<V3DObject*>(members[i]);

		if (base != nullptr && base->exists && base->visible)
		{
			if (currentCamera->BoxInView(base->Position, baseObject->GetMinimum(), baseObject->GetMaximum()))
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
}