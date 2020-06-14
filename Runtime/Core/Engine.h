#pragma once

#include <memory>
#include "EngineDefs.h"


namespace V
{

	class Context;

	enum Engine_Mode : uint32_t
	{
		Engine_Physics = 1UL << 0,
		Engine_Game    = 1UL << 1,

	};

	class V_API Engine
	{
	public:
		Engine(void* draw_handle, void* window_handle, void* window_instance, float window_width, float window_height);
		~Engine();


		void Tick();

		auto EngineMode_GetAll() { return m_flags; }
		void EngineMode_SetAll(const uint32_t flags) { m_flags = flags; }
		void EngineMode_Enable(const Engine_Mode flag) { m_flags |= flag; }
		void EngineMode_Disable(const Engine_Mode flag) { m_flags &= ~flag; }
		void EngineMode_Toggle(const Engine_Mode flag) { m_flags = !EngineMode_IsSet(flag) ? m_flags | flag : m_flags & ~flag; }
		bool EngineMode_IsSet(const Engine_Mode flag) const { return m_flags & flag; }

		const auto& GetWindowHandle() { return m_window_handle; }
		const auto& GetWindowInstance() { return m_window_instance; }
		const auto GetWindowWidth() { return m_window_width; }
		const auto GetWindowHeight() { return m_window_height; }

		auto GetContext() const { return m_context.get(); }


	private:
		void* m_draw_handle = nullptr;
		void* m_window_handle = nullptr;
		void* m_window_instance = nullptr;
		float m_window_width = 0;
		float m_window_height = 0;
		uint32_t m_flags = 0;
		std::shared_ptr<Context> m_context;

	};


}