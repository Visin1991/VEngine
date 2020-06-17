#pragma once

#include <string>

#include "VEngineConfig.h"

namespace V
{
	class V_API VEngine
	{
	public:
		VEngine() = default;
		void InitRHI(std::string& info);
	};
}