#include <iostream>

#include "RHI.h"
#include "RHI_SwapChain.h"
#include "RHI_Enum.h"

namespace V
{
	RHI::~RHI()
	{
		Destroy_Platform();
	}

	bool RHI::Init(void* _wc, void* _hwnd)
	{
		hWnd = _hwnd;
		wc = _wc;

		bool result = true;
		result &= Create_RHI_Context();

		return result;
	}

	bool RHI::Create_RHI_Context()
	{
		//Create RHI SwapChain
		RHI_SwapChain swapChian;
		swapChian.BufferCount = 2;
		swapChian.Width = 0;
		swapChian.Height = 0;
		swapChian.format = RHI_FORMAT_R8G8B8A8_UNORM;
		swapChian.Numerator = 60;
		swapChian.hWnd = hWnd;
		swapChian.SamplerCount = 1;
		swapChian.SamplerQuality = 0;

		bool result = true;

		result &= Create_RHI_Context_Platform(swapChian);

		return true;
	}

	void RHI::Update()
	{
		Update_ImGui();
		Rendering();
	}

	void RHI::Rendering()
	{
		Rendering_ImGUI_Platform();
	}

	void RHI::Update_ImGui()
	{
		Update_ImGui_NewFrame_Platform();

		// 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
		if (show_demo_window)
			ImGui::ShowDemoWindow(&show_demo_window);

		// 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
		{
			static float f = 0.0f;
			static int counter = 0;

			ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

			ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
			ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
			ImGui::Checkbox("Another Window", &show_another_window);

			ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
			ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

			if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
				counter++;
			ImGui::SameLine();
			ImGui::Text("counter = %d", counter);

			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			ImGui::End();
		}

		// 3. Show another simple window.
		if (show_another_window)
		{
			ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
			ImGui::Text("Hello from another window!");
			if (ImGui::Button("Close Me"))
				show_another_window = false;
			ImGui::End();
		}
	}
}