#include "RHI.h"
#include "RHI_SwapChain.h"
#include "RHI_Enum.h"

#include "ImGui/imgui.h"

namespace V
{

	bool RHI::Init(void* _wc, void* _hwnd)
	{
		hWnd = _hwnd;
		wc = _wc;

		bool result = true;
		result &= Create_RHI_Context();
		result &= Create_ImGui();

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

	bool RHI::Create_ImGui()
	{
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

		// Setup Dear ImGui style
		ImGui::StyleColorsDark();
		//ImGui::StyleColorsClassic();

		// Setup Platform/Renderer bindings
		Create_ImGui_Platform();

		return true;
	}

	void RHI::Update()
	{
		Update_Platform();
	}


}