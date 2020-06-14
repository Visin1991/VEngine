#include <fstream>
#include <cstdarg>

#include "Log.h"
#include "Logger.h"
#include "FileSystem.h"

using namespace std;

namespace V
{
	std::weak_ptr<Logger> Log::m_logger;
	std::ofstream Log::m_fout;
	std::mutex Log::m_mutex_log;
	std::vector<LogCmd> Log::m_log_buffer;
	//std::string Log::m_caller_name;
	std::string Log::m_log_file_name = "log.txt";
	bool Log::m_log_to_file = true;
	bool Log::m_first_log = true;

	void Log::SetLogger(const std::weak_ptr<Logger>& logger)
	{
		m_logger = logger;
	}

	void Log::Write(const char* text, const Log_Type type)
	{
		if (!text)
		{
			LOG_ERROR_INVALID_PARAMETER();
			return;
		}

		std::lock_guard<std::mutex> guard(m_mutex_log);

		const auto log_to_file = m_logger.expired() || m_log_to_file;
		const auto formated_text = !m_caller_name.empty() ? m_caller_name + ": " + string(text) : string(text);

		if (log_to_file)
		{
			m_log_buffer.emplace_back(formated_text, type);
			LogToFile(formated_text.c_str(), type);
		}
		else
		{
			FlushBuffer();
			LogString(formated_text.c_str(), type);
		}
		m_caller_name.clear();
	}

	void Log::WriteFInfo(const char* text, ...)
	{
		char buffer[1024];
		va_list args;
		va_start(args, text);
		auto w = vsnprintf(buffer, sizeof(buffer), text, args);
		va_end(args);

		Write(buffer, Log_Info);
	}

	void Log::WriteFWarning(const char* text, ...)
	{
		char buffer[1024];
		va_list args;
		va_start(args, text);
		auto w = vsnprintf(buffer, sizeof(buffer), text, args);
		va_end(args);

		Write(buffer, Log_Warning);
	}

	void Log::WriteFError(const char* text, ...)
	{
		char buffer[1024];
		va_list args;
		va_start(args, text);
		auto w = vsnprintf(buffer, sizeof(buffer), text, args);
		va_end(args);

		Write(buffer, Log_Error);
	}

	void Log::FlushBuffer()
    {
        if (m_logger.expired() || m_log_buffer.empty())
            return;

         // Log everything from memory to the logger implementation
        for (const auto& log : m_log_buffer)
        {
            LogString(log.text.c_str(), log.type);
        }
        m_log_buffer.clear();
    }

	void Log::LogString(const char* text, const Log_Type type)
	{
		if (!text)
		{
			LOG_ERROR_INVALID_PARAMETER();
			return;
		}
		m_logger.lock()->Log(string(text), type);
	}

	void Log::LogToFile(const char* text, const Log_Type type)
	{
		if (!text)
		{
			LOG_ERROR_INVALID_PARAMETER();
			return;
		}

		const string prefix = (type == Log_Info) ? "Info:" : (type == Log_Warning) ? "Warning:" : "Error:";
		const auto final_text = prefix + " " + text;

		// Delete the previous log file (if it exists)
		if (m_first_log)
		{
			//ToDo ......
			//FileSystem::DeleteFile_(m_log_file_name);
			m_first_log = false;
		}

		// Open/Create a log file to write the error message to
		m_fout.open(m_log_file_name, ofstream::out | ofstream::app);

		if (m_fout.is_open())
		{
			// Write out the error message
			m_fout << final_text << endl;

			// Close the file
			m_fout.close();
		}

	}
}