#pragma once

#include <string>
#include <functional>
#include "EngineDefs.h"


struct LogPackage
{
	std::string text;
	unsigned int error_level = 0;
};

namespace V
{
	class V_API Logger
	{
	public:
		typedef std::function<void(LogPackage)> log_func;
		void SetCallback(log_func&& func)
		{
			m_log_func = std::forward<log_func>(func);
		}

		void Log(const std::string& text, const unsigned int error_level)
		{
			LogPackage package;
			package.text = text;
			package.error_level = error_level;
			m_log_func(package);
		}

	private:
		log_func m_log_func;
	};
}