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
		bool Create_RHI_Context();
		bool Create_RHI_Context_Platform(RHI_SwapChain& swapChian);

		bool Create_ImGui();
		void Create_ImGui_Platform();

		void Update_ImGui();
		void Update_ImGui_NewFrame_Platform();

		void Rendering_ImGUI_Platform();

		void Destroy_Platform();

	public:
		void* wc;
		void* hWnd;

		// Our state
		bool show_demo_window;
		bool show_another_window;
		ImVec4 clear_color;
	};

}