#include "VGlobal.h"
#include "VObject.h"
#include "VPostEffect.h"
#include "VTimer.h"

VGlobal* VGlobal::Instance = nullptr;

VGlobal::VGlobal()
{
	gsm = new VStateManager();
	Content = new VContent();
	Music = new VMusic();
	Sound = new VSoundManager();
}

VGlobal::~VGlobal()
{
	if (Content)
	{
		delete Content;
		Content = nullptr;
	}
	if (Music)
	{
		delete Music;
		Music = nullptr;
	}
	if (Sound)
	{
		delete Sound;
		Sound = nullptr;
	}
	if (PostProcess)
	{
		delete PostProcess;
		PostProcess = nullptr;
	}
	if (gsm)
	{
		delete gsm;
		gsm = nullptr;
	}
}

void VGlobal::SetFullscreen(bool set)
{
	if (set && !fullscreen)
	{
		App.create(sf::VideoMode::getDesktopMode(), Title, sf::Style::Fullscreen);
		App.setMouseCursorVisible(set);
		fullscreen = true;
	}
	else if (!set && fullscreen)
	{
		App.create(sf::VideoMode(WindowWidth, WindowHeight), Title, WindowStyle);
		App.setMouseCursorVisible(set);
		fullscreen = false;
	}
}

void VGlobal::SetMouseCursorVisible(bool set)
{
	mouseCursorVisible = set;
	App.setMouseCursorVisible(set);
}

void VGlobal::ToggleFullscreen()
{
	SetFullscreen(!fullscreen);
}

sf::Vector2f VGlobal::GetMousePosition()
{
	sf::Vector2f actualMousePos = sf::Vector2f(sf::Mouse::getPosition(App));
	sf::Vector2f renderAreaOffset = VGlobal::p()->RenderSprite.getPosition();
	float sx = App.getSize().x / (float)Width; 
	float sy = App.getSize().y / (float)Height;

	actualMousePos -= renderAreaOffset;

	if (sx < sy)
	{
		actualMousePos.x /= sx;
		actualMousePos.y /= sx;
	}
	else
	{
		actualMousePos.x /= sy;
		actualMousePos.y /= sy;
	}

	return actualMousePos;
}

VState* VGlobal::CurrentState()
{
	return gsm->CurrentState();
}

void VGlobal::ChangeState(VState* state)
{
	if (IfChangedState && state == nullptr)
	{
		if (!Async.ActiveAsyncFunctions())
		{
			if (VTimeManager::AnyActiveTimers())
			{
				VTimeManager::p()->Clear();
			}

			gsm->ChangeState(nextState);
			nextState = nullptr;
			IfChangedState = false;
		}
	}
	else if (nextState == nullptr)
	{
		nextState = state;
		IfChangedState = true;
	}
}

void VGlobal::PushState(VState* state)
{
	if (IfPushedState && state == nullptr)
	{
		gsm->PushState(nextState);
		nextState = nullptr;
		IfPushedState = false;
	}
	else if (nextState == nullptr)
	{
		nextState = state;
		IfPushedState = true;
	}
}

void VGlobal::PopState()
{
	gsm->PopState();
}

void VGlobal::ClearState()
{
	gsm->Clear();
}

void VGlobal::Exit()
{
	App.close();
}

#include "VCollision.h"

bool VGlobal::Overlaps(VBase* a, VBase* b, std::function<void(VObject*, VObject*)>const& responseCall, std::function<bool(VObject*, VObject*)>const& processCall)
{
	bool result = false;

	if (a == nullptr)
	{
		a = CurrentState();
	}
	if (b == a)
	{
		b = nullptr;
	}

	VCollision* collision = new VCollision();
	if (b != nullptr) collision->AddToList(b, B);
	collision->AddToList(a, A);
	result = collision->Run(responseCall, processCall);
	delete collision;

	return result;
}

bool VGlobal::Collides(VBase* a, VBase* b, std::function<void(VObject*, VObject*)>const& responseCall)
{
	return Overlaps(a, b, responseCall, VObject::separate);
}

bool VGlobal::OverlapsCircle(VBase* a, VBase* b, std::function<void(VObject*, VObject*)>const& responseCall, std::function<bool(VObject*, VObject*)>const& processCall)
{
	bool result = false;

	if (a == nullptr)
	{
		a = CurrentState();
	}
	if (b == a)
	{
		b = nullptr;
	}

	VCircleCollision* collision = new VCircleCollision();
	if (b != nullptr) collision->AddToList(b, B);
	collision->AddToList(a, A);
	result = collision->Run(responseCall, processCall);
	delete collision;

	return result;
}

bool VGlobal::CollidesCircle(VBase* a, VBase* b, std::function<void(VObject*, VObject*)>const& responseCall)
{
	return OverlapsCircle(a, b, responseCall, VObject::separateCircle);
}

void VGlobal::Cleanup()
{
	if (Instance)
	{
		delete Instance;
		Instance = nullptr;
	}
}