#include <iostream>

#include "Engine.h"
#include "Context.h"
#include "FileSystem.h"
#include "EventSystem.h"

#include "Timer.h"
#include "Settings.h"


using namespace std;

namespace V
{
	Engine::Engine(void* draw_handle, void* window_handle, void* window_instance, float window_width, float window_height)
	{
		m_flags |= Engine_Physics;
		m_flags |= Engine_Game;

		m_draw_handle = draw_handle;
		m_window_handle = window_handle;
		m_window_instance = window_instance;
		m_window_width = window_width;
		m_window_height = window_height;

		m_context = make_shared<Context>();
		m_context->m_engine = this;

		//Rgister all Modules
		m_context->RegisterSubsystem<Timer>(Tick_Variable);
		m_context->RegisterSubsystem<Settings>(Tick_Variable);
		

		//Initialize global/static 
		FileSystem::Initialize();

		m_context->Initialize();

	}

	Engine::~Engine()
	{
		EventSystem::Get().Clear();
	}

	void Engine::Tick()
	{
		Timer* timer = m_context->GetSubsystem<Timer>().get();
		m_context->Tick(Tick_Variable, timer->GetDeltaTimeSec());
		m_context->Tick(Tick_Smoothed, timer->GetDeltaTimeSmoothedSec());

		std::cout << timer->GetDeltaTimeSec() << std::endl;
	}



}
