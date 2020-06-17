#pragma once

#include <memory>

#include "../VEngineConfig.h"

#include "RHI_SwapChain.h"

namespace V
{

	class V_API RHI
	{
	public:
		RHI()=default;
		
		bool Init(void* _wc, void* _hwnd);

		bool Create_RHI_SwapChian();
		bool Create_RHI_SwapChian_Impl(RHI_SwapChain& swapChian);



		void Update();

		void ResizeSurface(unsigned int _width, unsigned int _height, void* _handle, void* _instance);

	public:
		void* wc;
		void* hwnd;
	};

}