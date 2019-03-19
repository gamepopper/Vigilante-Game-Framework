#include "VGlobal.h"
#include "VObject.h"
#include "VPostEffect.h"
#include "VTimer.h"
#include "VCollision.h"

VGlobal* VGlobal::Instance = nullptr;

VGlobal::VGlobal()
{
	if (App == nullptr) //RenderWindow is most required, so we check if this is NULL.
	{
		App = std::unique_ptr<sf::RenderWindow>(new sf::RenderWindow());
		Input = std::unique_ptr<VInputHandler>(new VInputHandler());
		Content = std::unique_ptr<VContent>(new VContent());
		Music = std::unique_ptr<VMusic>(new VMusic());
		Sound = std::unique_ptr<VSoundManager>(new VSoundManager());
		Random = std::unique_ptr<VRandom>(new VRandom());
		Async = std::unique_ptr<VAsync>(new VAsync());

		gsm = std::unique_ptr<VStateManager>(new VStateManager());
		collision = std::unique_ptr<VCollision>(new VCollision());

		rectCollision = [](VObject* a, VObject* b)
		{
			if (a->Position.x < b->Position.x + b->Size.x &&
				a->Position.x + a->Size.x > b->Position.x &&
				a->Position.y < b->Position.y + b->Size.y &&
				a->Position.y + a->Size.y > b->Position.y)
			{
				return true;
			}

			return false;
		};

		circleCollision = [](VObject* a, VObject* b)
		{
			sf::Vector2f aCentre = a->Position + (a->Size / 2.0f);
			sf::Vector2f bCentre = b->Position + (b->Size / 2.0f);

			sf::Vector2f diff = aCentre - bCentre;
			float length = sqrtf((diff.x * diff.x) + (diff.y * diff.y));

			if (length < a->Radius + b->Radius)
			{
				return true;
			}

			return false;
		};
	}
}

VGlobal::~VGlobal()
{
	gsm.reset();
	Input.reset();
	Content.reset();
	Music.reset();
	Sound.reset();
	Random.reset();
	Async.reset();
	PostProcess.reset();
	App.reset();
}

VGlobal* VGlobal::p() ///Access and creation of VGlobal
{
	if (!Instance)
	{
		Instance = new VGlobal();
	}

	return Instance;
}

void VGlobal::SetTitle(const char* newTitle)
{
	App->setTitle(newTitle);
	title = newTitle;
}

void VGlobal::SetTitle(std::string newTitle)
{
	SetTitle(newTitle.c_str());
}

const char* VGlobal::GetTitle()
{
	return title;
}

#ifndef VFRAME_NO_3D
#include "V3DScene.h"
#endif

void VGlobal::SetFullscreen(bool set)
{
#ifndef VFRAME_NO_3D
	//Push any GL States from non-legacy contexts to avoid any removals.
	auto pushStatesFunc = [](VBase* base) {
		V3DScene* scene = dynamic_cast<V3DScene*>(base);
		if (scene)
		{
			scene->SetActive(false);
		}
	};
	CurrentState()->ForEach(pushStatesFunc, true);
#endif

	if (set && !fullscreen)
	{
		App->create(sf::VideoMode::getDesktopMode(), title, sf::Style::Fullscreen);
		App->setMouseCursorVisible(!set);
		fullscreen = true;
	}
	else if (!set && fullscreen)
	{
		App->create(sf::VideoMode(Width, Height), title, WindowStyle);
		App->setMouseCursorVisible(!set);
		fullscreen = false;
	}

#ifndef VFRAME_NO_3D
	//Restore the GLStates.
	auto popStatesFunc = [](VBase* base) {
		V3DScene* scene = dynamic_cast<V3DScene*>(base);
		if (scene)
		{
			//scene->PopGLStates();
		}
	};
	CurrentState()->ForEach(popStatesFunc, true);
#endif
}

void VGlobal::SetMouseCursorVisible(bool set)
{
	mouseCursorVisible = set;
	App->setMouseCursorVisible(set);
}

void VGlobal::ToggleFullscreen()
{
	SetFullscreen(!fullscreen);
}

bool VGlobal::IsFullscreen()
{
	return fullscreen;
}

bool VGlobal::IsRunning()
{
	return running;
}

sf::Vector2f VGlobal::GetMousePosition()
{
	sf::Vector2f actualMousePos = sf::Vector2f(sf::Mouse::getPosition(*App));
	sf::Vector2f renderAreaOffset = VGlobal::p()->ViewportOffset;
	float sx = App->getSize().x / (float)Width; 
	float sy = App->getSize().y / (float)Height;

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
		if (!Async->ActiveAsyncFunctions())
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
	running = false;
}

bool VGlobal::OverlapAtPoint(const sf::Vector2f& point, VBase* a)
{
	VObject* o = new VObject(point, sf::Vector2f(1, 1));

	bool r = Overlaps(o, a);
	delete o;
	return r;
}

bool VGlobal::Overlaps(VBase* a, VBase* b, std::function<void(VObject*, VObject*)>const& responseCall, std::function<bool(VObject*, VObject*)>const& processCall)
{
	bool result = false;

	if (a == nullptr) //if a is null, then use the entirety of the state. Basically assume all objects will be checked.
	{
		a = CurrentState();
	}

	if (b == nullptr) //if b is null, then use whatever is in A. This is so you can check all in a with themselves.
	{
		b = a;
	}

	collision->Initialise(WorldBounds);
	collision->AddToList(a, A);
	collision->AddToList(b, B);
	result = collision->Run(rectCollision, responseCall, processCall);

	return result;
}

bool VGlobal::Collides(VBase* a, VBase* b, std::function<void(VObject*, VObject*)>const& responseCall)
{
	return Overlaps(a, b, responseCall, VObject::separate);
}

bool VGlobal::OverlapCircleAtPoint(const sf::Vector2f& point, VBase* a)
{
	VObject* o = new VObject(point, sf::Vector2f(1, 1));

	bool r = OverlapsCircle(o, a);
	delete o;
	return r;
}

bool VGlobal::OverlapsCircle(VBase* a, VBase* b, std::function<void(VObject*, VObject*)>const& responseCall, std::function<bool(VObject*, VObject*)>const& processCall)
{
	bool result = false;

	if (a == nullptr) //if a is null, then use the entirety of the state. Basically assume all objects will be checked.
	{
		a = CurrentState();
	}

	if (b == nullptr) //if b is null, then use whatever is in A. This is so you can check all in a with themselves.
	{
		b = a;
	}

	collision->Initialise(WorldBounds);
	collision->AddToList(a, A);
	collision->AddToList(b, B);
	result = collision->Run(circleCollision, responseCall, processCall);

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