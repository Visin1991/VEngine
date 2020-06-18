#pragma once

#include "VEngineConfig.h"
#include "RHI/RHI.h"

namespace V
{
	class V_API VEngine
	{
	public:
		VEngine() = default;
		void ResizeSurface(unsigned int width, unsigned int height, void* _handle = nullptr, void* instance = nullptr);
		void Update();
		bool Init(void* wc, void* hwnd);
	public:
		RHI m_rhi;
	};
}