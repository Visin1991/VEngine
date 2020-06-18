#pragma once

#include <memory>

#include "../VEngineConfig.h"

#include "RHI_SwapChain.h"
#include "RHI_RenderTarget.h"

namespace V
{

	class V_API RHI
	{
	public:
		RHI()=default;
		
		bool Init(void* _wc, void* _hwnd);

		void Update();

		void ResizeSurface(unsigned int _width, unsigned int _height, void* _handle, void* _instance);

	private:
		bool Create_RHI_Context();
		bool Create_RHI_Context_Platform(RHI_SwapChain& swapChian);

		bool Create_ImGui();
		void Create_ImGui_Platform();

		void Update_Platform();

	public:
		void* wc;
		void* hWnd;
	};

}