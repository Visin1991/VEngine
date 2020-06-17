#pragma once

#include "../VEngineConfig.h"
#include "RHI_Enum.h"

namespace V
{
	struct V_API RHI_SwapChain
	{
		unsigned int			BufferCount;
		unsigned int			Width;
		unsigned int			Height;
		RHI_RT_FORMAT			format;
		unsigned int			Numerator;
		void*					hWnd;
		unsigned int            SamplerCount;
		unsigned int			SamplerQuality;
	};
}