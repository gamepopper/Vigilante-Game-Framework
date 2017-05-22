#include "VGroup.h"
#include "VGlobal.h"

using std::vector;

VBase* VGroup::Add(VBase* object)
{
	if (object == NULL)
	{
		VLog("Cannot add NULL to a VGroup");
		return NULL;
	}

	if (GetIndexOfItem(object) >= 0)
		return NULL;

	if (MaxSize > 0 && length >= static_cast<int>(MaxSize))
	{
		return object;
	}

	int index = FirstNULL();
	if (index != -1)
	{
		members[index] = object;
	}
	else
	{
		members.push_back(object);
	}

	object->RefCount++;
	length++;

	return object;
}

VBase* VGroup::Remove(VBase* object, bool splice)
{
	if (members.size() == 0)
		return NULL;

	auto pos = GetIndexOfItem(object);

	if (pos < 0)
		return NULL;

	if (splice)
	{
		members.erase(members.begin() + pos);
	}
	else
	{
		members[pos] = NULL;
	}

	object->RefCount--;
	length--;

	return object;
}

VBase* VGroup::FirstAvailable()
{
	for (unsigned int i = 0; i < members.size(); i++)
	{
		VBase* base = dynamic_cast<VBase*>(members[i]);

		if (base != NULL && !base->exists)
		{
			return members[i];
		}
	}

	return NULL;
}

int VGroup::FirstNULL()
{
	for (unsigned int i = 0; i < members.size(); i++)
	{
		VBase* base = dynamic_cast<VBase*>(members[i]);

		if (base == NULL)
		{
			return i;
		}
	}

	return -1;
}

VBase* VGroup::FirstExisting()
{
	for (unsigned int i = 0; i < members.size(); i++)
	{
		VBase* base = dynamic_cast<VBase*>(members[i]);

		if (base != NULL && base->exists)
		{
			return members[i];
		}
	}

	return NULL;
}

VBase* VGroup::FirstAlive()
{
	for (unsigned int i = 0; i < members.size(); i++)
	{
		VBase* base = dynamic_cast<VBase*>(members[i]);

		if (base != NULL && base->exists && base->alive)
		{
			return members[i];
		}
	}

	return NULL;
}

VBase* VGroup::FirstDead()
{
	for (unsigned int i = 0; i < members.size(); i++)
	{
		VBase* base = dynamic_cast<VBase*>(members[i]);

		if (base != NULL && !base->alive)
		{
			return members[i];
		}
	}

	return NULL;
}

int VGroup::CountAlive()
{
	int count = 0;
	for (unsigned int i = 0; i < members.size(); i++)
	{
		VBase* base = dynamic_cast<VBase*>(members[i]);

		if (base != NULL && base->exists && base->alive)
		{
			count++;
		}
	}

	return count;
}

int VGroup::CountDead()
{
	int count = 0;
	for (unsigned int i = 0; i < members.size(); i++)
	{
		VBase* base = dynamic_cast<VBase*>(members[i]);

		if (base != NULL && !base->alive)
		{
			count++;
		}
	}

	return count;
}

VBase* VGroup::GetRandom(int min, int max)
{
	return members[VGlobal::p()->Random.GetInt(max, min)];
}

void VGroup::ForEach(std::function<void(VBase*)> function, bool recursive)
{
	VBase* base = NULL;

	for (int i = 0; i < length; i++)
	{
		base = dynamic_cast<VBase*>(members[i]);

		if (base != NULL)
		{
			if (recursive)
			{
				VGroup* group = dynamic_cast<VGroup*>(base);
				if (group != NULL)
				{
					group->ForEach(function, recursive);
				}
			}

			function(base);
		}
	}
}

void VGroup::ForEachAlive(std::function<void(VBase*)> function, bool recursive)
{
	VBase* base = NULL;

	for (int i = 0; i < length; i++)
	{
		base = dynamic_cast<VBase*>(members[i]);

		if (base != NULL && base->exists && base->alive)
		{
			if (recursive)
			{
				VGroup* group = dynamic_cast<VGroup*>(base);
				if (group != NULL)
				{
					group->ForEachAlive(function, recursive);
				}
			}

			function(base);
		}
	}
}

void VGroup::ForEachDead(std::function<void(VBase*)> function, bool recursive)
{
	VBase* base = NULL;

	for (int i = 0; i < length; i++)
	{
		base = dynamic_cast<VBase*>(members[i]);

		if (base != NULL && !base->alive)
		{
			if (recursive)
			{
				VGroup* group = dynamic_cast<VGroup*>(base);
				if (group != NULL)
				{
					group->ForEachDead(function, recursive);
				}
			}

			function(base);
		}
	}
}

void VGroup::ForEachExists(std::function<void(VBase*)> function, bool recursive)
{
	VBase* base = NULL;

	for (int i = 0; i < length; i++)
	{
		base = dynamic_cast<VBase*>(members[i]);

		if (base != NULL && base->exists)
		{
			if (recursive)
			{
				VGroup* group = dynamic_cast<VGroup*>(base);
				if (group != NULL)
				{
					group->ForEachExists(function, recursive);
				}
			}

			function(base);
		}
	}
}

template <class T>
void VGroup::ForEachType(void (*function)(T* object), bool recursive)
{
	VBase* base = NULL;

	for (int i = 0; i < length; i++)
	{
		base = dynamic_cast<VBase*>(members[i]);

		if (base != NULL && base->exists)
		{
			if (recursive)
			{
				VGroup* group = dynamic_cast<VGroup*>(base);
				if (group != NULL)
				{
					group->ForEachExists(function, recursive);
				}
			}

			T* object = dynamic_cast<T*>(base);
			if (object != NULL)
			{
				function(base);
			}
		}
	}
}

VBase* VGroup::GetGroupItem(int index)
{
	if (index >= 0 && index < length)
	{
		return members[index];
	}

	return NULL;
}

int VGroup::GetIndexOfItem(VBase* object)
{
	auto index = std::find(members.begin(), members.end(), object);

	if (index == members.end())
		return -1;

	return index - members.begin();
}

void VGroup::OrganiseNULLS()
{
	for (unsigned int i = 0; i < members.size(); i++)
	{
		int first = FirstNULL();

		if (first == -1)
			return;

		if (members[i] && first < (int)i)
		{
			Swap(first, i);
		}
	}
}

void VGroup::Swap(int a, int b)
{
	VBase* temp = members[a];
	members[a] = members[b];
	members[b] = temp;
}

void VGroup::Sort(std::function<bool(VBase*, VBase*)> func)
{
	std::sort(members.begin(), members.end(), func);
}

void VGroup::Reverse()
{
	std::reverse(members.begin(), members.end());
	OrganiseNULLS();
}

void VGroup::Clear()
{
	for (int i = 0; i < length; i++)
	{
		if (members[i] != nullptr)
		{
			if (members[i]->RefCount > 1)
			{
				members[i]->RefCount--;
				members[i] = nullptr;
			}
		}
	}

	length = 0;
	members.clear();
	members.shrink_to_fit();
}

void VGroup::Destroy()
{
	VSUPERCLASS::Destroy();

	for (unsigned int i = 0; i < members.size(); i++)
	{
		if (members[i] != nullptr)
		{
			if (members[i]->RefCount <= 1)
			{
				members[i]->Destroy();
				delete members[i];
				members[i] = nullptr;
			}
			else
			{
				members[i]->RefCount--;
				members[i] = nullptr;
			}
		}
	}

	Clear();
}

void VGroup::Kill()
{
	for (unsigned int i = 0; i < members.size(); i++)
	{
		VBase* base = dynamic_cast<VBase*>(members[i]);

		if (base != NULL && base->exists)
			base->Kill();
	}

	VSUPERCLASS::Kill();
}

void VGroup::Revive()
{
	for (unsigned int i = 0; i < members.size(); i++)
	{
		VBase* base = dynamic_cast<VBase*>(members[i]);

		if (base != NULL && !base->exists)
			base->Revive();
	}

	VSUPERCLASS::Revive();
}

void VGroup::Update(float dt)
{
	if (!active)
		return;
	
	for (unsigned int i = 0; i < members.size(); i++)
	{
		VBase* base = dynamic_cast<VBase*>(members[i]);

		if (base != NULL && base->exists && base->active)
		{
			base->Update(dt);
		}
	}
}

#include "VRenderGroup.h"

void VGroup::Draw(sf::RenderTarget& RenderTarget)
{
	if (!visible)
		return;

	for (unsigned int i = 0; i < members.size(); i++)
	{
		VBase* base = dynamic_cast<VBase*>(members[i]);

		if (base != NULL && base->exists && base->visible)
		{
			base->Draw(RenderTarget);
		}
	}

#if _DEBUG
	if (VGlobal::p()->DrawDebug)
	{
		int memberLength = members.size();
		debuggingVertices.resize(memberLength * 8);
		for (unsigned int i = 0; i < members.size(); i++)
		{
			VObject* object = dynamic_cast<VObject*>(members[i]);

			if (object == NULL && members[i]->type == RENDERGROUP)
			{
				VRenderGroup* renderGroup = dynamic_cast<VRenderGroup*>(members[i]);
				object = dynamic_cast<VObject*>(renderGroup->Sprite);
			}

			if (object == NULL)
				continue;

			if (object->alive)
			{
				debuggingVertices[0 + (i * 8)].position = object->Position;													debuggingVertices[0 + (i * 8)].color = object->DebugColor;
				debuggingVertices[1 + (i * 8)].position = object->Position + sf::Vector2f(object->Size.x, 0);				debuggingVertices[1 + (i * 8)].color = object->DebugColor;
				debuggingVertices[2 + (i * 8)].position = object->Position + sf::Vector2f(object->Size.x, 0);				debuggingVertices[2 + (i * 8)].color = object->DebugColor;
				debuggingVertices[3 + (i * 8)].position = object->Position + sf::Vector2f(object->Size.x, object->Size.y);	debuggingVertices[3 + (i * 8)].color = object->DebugColor;
				debuggingVertices[4 + (i * 8)].position = object->Position + sf::Vector2f(object->Size.x, object->Size.y);	debuggingVertices[4 + (i * 8)].color = object->DebugColor;
				debuggingVertices[5 + (i * 8)].position = object->Position + sf::Vector2f(0, object->Size.y);				debuggingVertices[5 + (i * 8)].color = object->DebugColor;
				debuggingVertices[6 + (i * 8)].position = object->Position + sf::Vector2f(0, object->Size.y);				debuggingVertices[6 + (i * 8)].color = object->DebugColor;
				debuggingVertices[7 + (i * 8)].position = object->Position;													debuggingVertices[7 + (i * 8)].color = object->DebugColor;
			}
			else
			{
				debuggingVertices[0 + (i * 8)].color = sf::Color::Transparent;
				debuggingVertices[1 + (i * 8)].color = sf::Color::Transparent;
				debuggingVertices[2 + (i * 8)].color = sf::Color::Transparent;
				debuggingVertices[3 + (i * 8)].color = sf::Color::Transparent;
				debuggingVertices[4 + (i * 8)].color = sf::Color::Transparent;
				debuggingVertices[5 + (i * 8)].color = sf::Color::Transparent;
				debuggingVertices[6 + (i * 8)].color = sf::Color::Transparent;
				debuggingVertices[7 + (i * 8)].color = sf::Color::Transparent;
			}
		}

		RenderTarget.draw(debuggingVertices);
	}
	else
	{
		debuggingVertices.clear();
	}
#endif
}