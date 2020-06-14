#include "Timer.h"
#include "Settings.h"
#include "Log.h"

using namespace std;
using namespace chrono;

namespace V
{
	Timer::Timer(Context* context) : IModule(context)
	{
		time_a = chrono::high_resolution_clock::now();
		time_b = chrono::high_resolution_clock::now();
	}

	void Timer::Tick(float delta_time)
	{
		time_a = chrono::high_resolution_clock::now();
		duration<double, milli> time_work = time_a - time_b;

		//
		const double max_fps		= m_fps_target;
		const double min_dt			= 1000.0 / max_fps;
		const double dt_remainning  = min_dt - time_work.count();

		//
		if (dt_remainning > 0)
		{
			this_thread::sleep_for(milliseconds(static_cast<int64_t>(dt_remainning)));
		}

		//
		time_b										= high_resolution_clock::now();
		const duration<double, milli> time_sleep	= time_b - time_a;
		m_delta_time_ms								= (time_work + time_sleep).count();

		//Compute smoothed delta time
		double delta_max							= 1000.0 / m_fps_min;
		double delta_clamped						= m_delta_time_ms > delta_max ? delta_max : m_delta_time_ms;	//if frame time is too high/slow, clamp it
		double delta_feedback						= 0.2;
		m_delta_time_smoothed_ms					= m_delta_time_smoothed_ms * (1.0 - delta_feedback) + delta_clamped * delta_feedback;
	}

	void Timer::SetTargetFps(double fps)
	{
		if (fps < 0.0f)
		{
			m_fps_policy	= Fps_FixedMonitor;
			fps				= m_monitor_refresh_rate;
		}
		else if(fps >=0.0f && fps < 10.0f)
		{
			m_fps_policy = Fps_Unlocked;
			fps = m_fps_max;
		}
		else
		{
			m_fps_policy = Fps_Fixed;
		}

		if (m_fps_target == fps)
			return;

		m_fps_target = fps;
		m_user_selected_fps_target = true;
		LOGF_INFO("FPS limit set to %.2f", m_fps_target);

	}

	void Timer::AddMonitorRefreshRate(double refresh_rate)
	{
		refresh_rate = m_monitor_refresh_rate > refresh_rate ? m_monitor_refresh_rate : refresh_rate;

		if (m_monitor_refresh_rate == refresh_rate)
			return;

		m_monitor_refresh_rate = refresh_rate;

		// If the user hasn't specified an fps target, try to match the monitor
		if (!m_user_selected_fps_target)
		{
			m_fps_target = m_monitor_refresh_rate;
		}

		LOGF_INFO("Maximum monitor refresh rate set to %.2f hz", m_monitor_refresh_rate);
	}


}