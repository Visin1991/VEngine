#pragma once

#include <memory>
#include <string>
#include <mutex>
#include <vector>
#include "EngineDefs.h"

namespace V
{
	enum Log_Type
	{
		Log_Info,
		Log_Warning,
		Log_Error
	};

	struct LogCmd
	{
		LogCmd(const std::string& text, const Log_Type type)
		{
			this->text = text;
			this->type = type;
		}
		std::string text;
		Log_Type type;
	};


	class V_API Log
	{
		friend class Logger;
	public:
		Log() = default;

		static void SetLogger(const std::weak_ptr<Logger>& logger);

		static void Write(const char* text, const Log_Type type);

		static void WriteFInfo(const char* text, ...);

		static void WriteFWarning(const char* text, ...);

		static void WriteFError(const char* text, ...);

		static void Write(const std::string& text, const Log_Type type) { Write(text.c_str(), type); }

		template<class T,class = typename std::enable_if<
			std::is_same<T, int>::value ||
			std::is_same<T, long>::value ||
			std::is_same<T, long long>::value ||
			std::is_same<T, unsigned>::value ||
			std::is_same<T, unsigned long>::value ||
			std::is_same<T, unsigned long long>::value ||
			std::is_same<T, float>::value ||
			std::is_same<T, double>::value ||
			std::is_same<T, long double>::value
		>::type>
		static void Write(T value, Log_Type type)
		{
			Write(std::to_string(value), type);
		}

		static void Write(const bool value, const Log_Type type)							{ Write(value ? "True" : "False", type); }
		template<typename T> static void Write(std::weak_ptr<T> ptr, const Log_Type type)	{ Write(ptr.expired() ? "Expired" : typeid(ptr).name(), type); }
		template<typename T> static void Write(std::shared_ptr<T> ptr, const Log_Type type) { Write(ptr ? typeid(ptr).name() : "Null", type); }

		static bool m_log_to_file;

		//For C++17 or Later, we can declare the variable inline in our header to make it a definition.
		static inline std::string m_caller_name;

	private:
		static void FlushBuffer();
		static void LogString(const char* text, Log_Type type);
		static void LogToFile(const char* text, Log_Type type);

		static std::weak_ptr<Logger> m_logger;
		static std::ofstream m_fout;
		static std::mutex m_mutex_log;
		static std::string m_log_file_name;
		static bool m_first_log;
		static std::vector<LogCmd> m_log_buffer;


	};

	#define LOG_TO_FILE(value)		{ V::Log::m_log_to_file = value;}
	#define LOG_INFO(text)			{ V::Log::m_caller_name = __FUNCTION__; V::Log::Write(text, V::Log_Type::Log_Info);}
	#define LOG_WARNING(text)		{ V::Log::m_caller_name = __FUNCTION__; V::Log::Write(text, V::Log_Type::Log_Warning); }
	#define LOG_ERROR(text)			{ V::Log::m_caller_name = __FUNCTION__; V::Log::Write(text, V::Log_Type::Log_Error); }
	#define LOGF_INFO(text, ...)	{ V::Log::m_caller_name = __FUNCTION__; V::Log::WriteFInfo(text, __VA_ARGS__); }
	#define LOGF_WARNING(text, ...)	{ V::Log::m_caller_name = __FUNCTION__; V::Log::WriteFWarning(text, __VA_ARGS__); }
	#define LOGF_ERROR(text, ...)	{ V::Log::m_caller_name = __FUNCTION__; V::Log::WriteFError(text, __VA_ARGS__); }

	#define LOG_ERROR_GENERIC_FAILURE()		LOG_ERROR("Failed.")
	#define LOG_ERROR_INVALID_PARAMETER()	LOG_ERROR("Invalid parameter.")
	#define LOG_ERROR_INVALID_INTERNALS()	LOG_ERROR("Invalid internals.")

}