#include "VGroup.h"
#include "VGlobal.h"

using std::vector;


///@return The current size of the group based on amount of active elements.

int VGroup::Length()
{
	return length;
}

VBase* VGroup::Add(VBase* object)
{
	if (object == nullptr)
	{
		VLog("Cannot add nullptr to a VGroup");
		return nullptr;
	}

	if (object == this)
	{
		VLogError("Cannot add itself to it's own list");
		return nullptr;
	}

	if (GetIndexOfItem(object) >= 0)
		return nullptr;

	if (MaxSize > 0 && length >= static_cast<int>(MaxSize))
	{
		VLogError("Maximum amount of objects already reached.");
		return nullptr;
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
		return nullptr;

	auto pos = GetIndexOfItem(object);

	if (pos < 0)
		return nullptr;

	if (splice)
	{
		members.erase(members.begin() + pos);
	}
	else
	{
		members[pos] = nullptr;
	}

	object->RefCount--;
	length--;

	return object;
}

VBase* VGroup::FirstAvailable()
{
	for (VBase* base : members)
	{
		if (base != nullptr && !base->exists)
		{
			return base;
		}
	}

	return nullptr;
}

int VGroup::FirstNULL()
{
	for (unsigned int i = 0; i < members.size(); ++i)
	{
		VBase* base = dynamic_cast<VBase*>(members[i]);

		if (base == nullptr)
		{
			return i;
		}
	}

	return -1;
}

VBase* VGroup::FirstExisting()
{
	for (VBase* base : members)
	{
		if (base != nullptr && base->exists)
		{
			return base;
		}
	}

	return nullptr;
}

VBase* VGroup::FirstAlive()
{
	for (VBase* base : members)
	{
		if (base != nullptr && base->exists && base->alive)
		{
			return base;
		}
	}

	return nullptr;
}

VBase* VGroup::FirstDead()
{
	for (VBase* base : members)
	{
		if (base != nullptr && !base->alive)
		{
			return base;
		}
	}

	return nullptr;
}

int VGroup::CountAlive(bool Recursive)
{
	int count = 0;
	for (VBase* base : members)
	{
		if (base != nullptr && base->exists && base->alive)
		{
			if (Recursive)
			{
				VGroup* group = dynamic_cast<VGroup*>(base);
				if (group)
				{
					count += group->CountAlive(Recursive);
				}
				else
				{
					count++;
				}
			}
			else
			{
				count++;
			}
		}
	}

	return count;
}

int VGroup::CountDead(bool Recursive)
{
	int count = 0;
	for (VBase* base : members)
	{
		if (base != nullptr && !base->alive)
		{
			if (Recursive)
			{
				VGroup* group = dynamic_cast<VGroup*>(base);
				if (group)
				{
					count += group->CountDead(Recursive);
				}
				else
				{
					count++;
				}
			}
			else
			{
				count++;
			}
		}
	}

	return count;
}

VBase* VGroup::GetRandom(int min, int max)
{
	if (members.size() == 0)
		return NULL;

	min = min >= 0 ? min : 0;
	max = max - 1 < (int)members.size() - 1 && max > 0 ? max : (int)members.size();
	return members[VGlobal::p()->Random->GetInt(max - 1, min)];
}

void VGroup::ForEach(const std::function<void(VBase*)>& function, bool recursive)
{
	VBase* base = nullptr;

	for (VBase* base : members)
	{
		if (base != nullptr)
		{
			if (recursive)
			{
				VGroup* group = dynamic_cast<VGroup*>(base);
				if (group != nullptr)
				{
					group->ForEach(function, recursive);
				}
				else
				{
					function(base);
				}
			}
			else
			{
				function(base);
			}
		}
	}
}

void VGroup::ForEachAlive(const std::function<void(VBase*)>& function, bool recursive)
{
	VBase* base = nullptr;

	for (VBase* base : members)
	{
		if (base != nullptr && base->exists && base->alive)
		{
			if (recursive)
			{
				VGroup* group = dynamic_cast<VGroup*>(base);
				if (group != nullptr)
				{
					group->ForEachAlive(function, recursive);
				}
				else
				{
					function(base);
				}
			}
			else
			{
				function(base);
			}
		}
	}
}

void VGroup::ForEachDead(const std::function<void(VBase*)>& function, bool recursive)
{
	VBase* base = nullptr;

	for (VBase* base : members)
	{
		if (base != nullptr && !base->alive)
		{
			if (recursive)
			{
				VGroup* group = dynamic_cast<VGroup*>(base);
				if (group != nullptr)
				{
					group->ForEachDead(function, recursive);
				}
				else
				{
					function(base);
				}
			}
			else
			{
				function(base);
			}
		}
	}
}

void VGroup::ForEachExists(const std::function<void(VBase*)>& function, bool recursive)
{
	VBase* base = nullptr;

	for (VBase* base : members)
	{
		if (base != nullptr && base->exists)
		{
			if (recursive)
			{
				VGroup* group = dynamic_cast<VGroup*>(base);
				if (group != nullptr)
				{
					group->ForEachExists(function, recursive);
				}
				else
				{
					function(base);
				}
			}
			else
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

	return nullptr;
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
	for (unsigned int i = 0; i < members.size(); ++i)
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
	if (a < 0 || a >= length)
		return;

	if (b < 0 || b >= length)
		return;

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
	for (VBase* base : members)
	{
		if (base != nullptr)
		{
			if (base->RefCount > 1)
			{
				base->RefCount--;
				base = nullptr;
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

	for (unsigned int i = 0; i < members.size(); ++i)
	{
		VBase* base = dynamic_cast<VBase*>(members[i]);
		if (base != nullptr)
		{
			if (base->RefCount <= 1)
			{
				base->Destroy();
				delete base;
				members[i] = nullptr;
			}
			else
			{
				base->RefCount--;
				members[i] = nullptr;
			}
		}
	}

	Clear();
}

void VGroup::Kill()
{
	for (VBase* base : members)
	{
		if (base != nullptr && base->exists)
			base->Kill();
	}

	VSUPERCLASS::Kill();
}

void VGroup::Revive()
{
	for (VBase* base : members)
	{
		if (base != nullptr && !base->exists)
			base->Revive();
	}

	VSUPERCLASS::Revive();
}

void VGroup::Update(float dt)
{
	if (!active)
		return;
	
	for (VBase* base : members)
	{
		if (base != nullptr && base->exists && base->active)
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

	for (VBase* base : members)
	{
		if (base != nullptr && base->exists && base->visible)
		{
			base->Draw(RenderTarget);
		}
	}

#if _DEBUG
	if (VGlobal::p()->DrawDebug)
	{
		int memberLength = members.size();
		debuggingVertices.resize(memberLength * 8);
		for (unsigned int i = 0; i < members.size(); ++i)
		{
			VObject* object = dynamic_cast<VObject*>(members[i]);

			if (object == nullptr)
				continue;

			if (object->type == RENDERGROUP)
			{
				VRenderGroup* renderGroup = dynamic_cast<VRenderGroup*>(object);
				object = renderGroup->Sprite.get();
			}

			if (object->exists)
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