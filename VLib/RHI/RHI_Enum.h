#pragma once

#include <stdint.h>

namespace V
{

	enum RHI_RT_FORMAT : uint32_t
	{
		RHI_FORMAT_R8_UNORM,
		RHI_FORMAT_R16_FLOAT,

		RHI_FORMAT_R11G11B10_FLOAT,

		RHI_FORMAT_R8G8B8A8_UNORM,
		RHI_FORMAT_R16G16B16A16_FLOAT
	};

}