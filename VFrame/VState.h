#pragma once
#include "VGroup.h"
#include "VCamera.h"
#include <SFML\Window\Event.hpp>
#include <SFML\Graphics\RenderTarget.hpp>
#include <functional>

#if _DEBUG
#include <iostream>
#endif

class VState;
class VSubState;

class VState : public VGroup
{
private:
	bool openSubState = false;
	bool closeSubstate = false;
	bool cleanup = false;

public:
	typedef VGroup VSUPERCLASS;
	VState() : VGroup() 
	{
		Cameras.emplace_back();
		Cameras[0] = new VCamera();
	}
	virtual ~VState() 
	{
		Cleanup();
	}

	std::vector<VCamera*> Cameras;
	VSubState* SubState = NULL;

	void OpenSubState(VSubState* subState);
	void CloseSubState();
	void ResetSubState();

	virtual void Initialise() {}
	virtual void Cleanup();
	virtual void Pause() {}
	virtual void Resume() {}
	virtual void HandleEvents(sf::Event event) {}
};

class VSubState : public VGroup
{
private:
	bool cleanup = false;

protected:
	sf::RectangleShape bgRect;

public:
	std::function<void()> OnClose = nullptr;
	VState* ParentState = NULL;
	bool UseParentCamera = false;

	typedef VGroup VSUPERCLASS;
	VSubState(sf::Color colour = sf::Color::Transparent) : VGroup()
	{
		bgRect.setFillColor(colour);
	}
	virtual ~VSubState()
	{
		Cleanup();
		ParentState = NULL;
	}

	void Close();

	virtual void Initialise() {}
	virtual void Cleanup();
	virtual void HandleEvents(sf::Event event) {}
	virtual void Draw(sf::RenderTarget &RenderTarget);
};

class VStateManager
{
public:
	VStateManager() = default;
	~VStateManager()
	{
		Clear();
	}

	VState* CurrentState();

	void ChangeState(VState* state);
	void PushState(VState* state);
	void PopState();
	void Clear();

protected:
	VStateManager(VStateManager const&) = delete;
	void operator=(VStateManager const&) = delete;
	std::vector<VState*> states;
};