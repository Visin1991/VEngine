#pragma once

#include <chrono>
#include <deque>

#include "EngineDefs.h"
#include "IModule.h"

namespace V
{
	class Context;

	enum FPS_Policy
	{
		Fps_Unlocked,
		Fps_Fixed,
		Fps_FixedMonitor
	};

	class V_API Timer : public IModule
	{
	public:
		Timer(Context* context);
		~Timer() = default;

		void Tick(float delta_time) override;

		void SetTargetFps(double fps);
		auto GetTargetFps() const { return m_fps_target; }
		auto GetMinFps() const { return m_fps_min; }
		auto GetFpsPolicy() const { return m_fps_policy; }
		void AddMonitorRefreshRate(double refresh_rate);
		auto GetMonitorRefreshRate() { return m_monitor_refresh_rate; }

		auto GetDeltaTimeMs()           const { return m_delta_time_ms; }
		auto GetDeltaTimeSec()          const { return static_cast<float>(m_delta_time_ms / 1000.0); }
		auto GetDeltaTimeSmoothedMs()   const { return m_delta_time_smoothed_ms; }
		auto GetDeltaTimeSmoothedSec()  const { return static_cast<float>(m_delta_time_smoothed_ms / 1000.0); }

	private:
		std::chrono::high_resolution_clock::time_point time_a;
		std::chrono::high_resolution_clock::time_point time_b;
		double m_delta_time_ms = 0.0f;
		double m_delta_time_smoothed_ms = 0.0f;

		double m_fps_min = 25.0;
		double m_fps_max = 200.0;
		double m_fps_target = m_fps_max; 
		double m_monitor_refresh_rate = 60.0;
		bool m_user_selected_fps_target = false;
		FPS_Policy m_fps_policy = Fps_Unlocked;
	};

}