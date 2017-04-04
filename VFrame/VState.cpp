#include "VState.h"
#include "VCamera.h"
#include "VGroup.h"

using std::vector;

void VState::Cleanup()
{
	if (!cleanup)
	{
		Destroy();

		for (unsigned int i = 0; i < Cameras.size(); i++)
		{
			delete Cameras[i];
			Cameras[i] = NULL;
		}

		Cameras.clear();
		Cameras.shrink_to_fit();

		cleanup = true;
		VLog("State cleanup successful\n");
	}
}

void VState::OpenSubState(VSubState* subState)
{
	if (SubState) //If substate is already active, close it and reset the substate.
	{
		CloseSubState();
		ResetSubState();
	}

	SubState = subState;
	openSubState = true;

	VLog("Substate open\n");
}

void VState::CloseSubState()
{
	if (SubState)
	{
		if (SubState->OnClose)
			SubState->OnClose();

		active = true;
		closeSubstate = true;

		VLog("Substate closed\n");
	}
}

void VState::ResetSubState()
{
	if (openSubState)
	{ 
		if (SubState)
		{
			SubState->ParentState = this;
			SubState->Initialise();
		}
		openSubState = false;
	}

	if (closeSubstate)
	{
		SubState->Cleanup();

		delete SubState;
		SubState = NULL;

		closeSubstate = false;
	}
}

void VSubState::Cleanup()
{
	if (!cleanup)
	{
		Destroy();
		cleanup = true;
		VLog("Substate cleanup successful\n");
	}
}

void VSubState::Close()
{
	if (ParentState)
		ParentState->CloseSubState();
}

void VSubState::Draw(sf::RenderTarget &RenderTarget)
{
	sf::View defaultView = RenderTarget.getDefaultView();
	sf::View currentView = RenderTarget.getView();

	if (!UseParentCamera)
	{
		RenderTarget.setView(defaultView);
		bgRect.setSize(defaultView.getSize());
		RenderTarget.draw(bgRect);
	}

	VSUPERCLASS::Draw(RenderTarget);
	RenderTarget.setView(currentView);
}

VState* VStateManager::CurrentState()
{
	return states.back();
}

void VStateManager::ChangeState(VState* state)
{
	if (states.size())
	{
		states.back()->Cleanup();
		delete states.back();
		states.pop_back();
	}

	state->Initialise();
	states.push_back(state);
}

void VStateManager::PushState(VState* state)
{
	if (states.size())
	{
		states.back()->Pause();
	}

	state->Initialise();
	states.push_back(state);
}

void VStateManager::PopState()
{
	if (states.size())
	{
		states.back()->Cleanup();
		delete states.back();
		states.pop_back();
	}

	if (!states.empty())
	{
		states.back()->Resume();
	}
}

void VStateManager::Clear()
{
	for (unsigned int i = 0; i < states.size(); i++)
	{
		states[i]->Cleanup();
		delete states[i];
		states[i] = NULL;
	}
	
	states.clear();
	states.shrink_to_fit();

	VBase::VLog("States cleared\n");
}