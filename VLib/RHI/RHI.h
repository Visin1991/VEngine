#pragma once

#include <memory>

#include "../ImGui/imgui.h"

#include "../Core/VEngineConfig.h"

#include "RHI_SwapChain.h"
#include "RHI_RenderTarget.h"

namespace V
{

	class V_API RHI
	{
	public:
		RHI()=default;
		~RHI();
		
		bool Init(void* _wc, void* _hwnd);

		void Update();

		void Rendering();

		void ResizeSurface(unsigned int _width, unsigned int _height, void* _handle, void* _instance);

	private:
		bool RHI_Create_Context();
		void RHI_Update_ImGui();
	private:
		bool Platform_Create_Context(RHI_SwapChain& swapChian);
		void Platform_Update_ImGui_NewFrame();
		void Platform_Rendering_ImGUI();
		void Platform_Destroy();

	public:
		void* wc;
		void* hWnd;

		// Our state
		bool show_demo_window;
		bool show_another_window;
		ImVec4 clear_color;
	};

}