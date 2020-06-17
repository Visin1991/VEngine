#include "RHI.h"
#include "RHI_SwapChain.h"

namespace V
{

	void RHI::Create_RHI_SwapChian()
	{
		//Create RHI SwapChain
		RHI_SwapChain swapChian;
		swapChian.BufferCount = 2;
		swapChian.Width = 0;
		swapChian.Height = 0;
		swapChian.format = RHI_RT_FORMAT_R8_G8_B8_A8;
		swapChian.Numerator = 60;
		swapChian.hWnd = hwnd;
		swapChian.SamplerCount = 1;
		swapChian.SamplerQuality = 0;

		//Create Sw
		
	}
}