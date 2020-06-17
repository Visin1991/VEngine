#ifdef _WIN32

#include "../RHI.h"


#include <d3d11.h>
#pragma comment (lib, "d3d11.lib")

namespace V
{
    static const DXGI_FORMAT D3d11_Format[] =
    {
        DXGI_FORMAT_R8_UNORM,
        DXGI_FORMAT_R16_FLOAT,

        DXGI_FORMAT_R11G11B10_FLOAT,

        DXGI_FORMAT_R8G8B8A8_UNORM,
        DXGI_FORMAT_R16G16B16A16_FLOAT
    };

    // Data
    //static ID3D11Device* g_pd3dDevice = NULL;
    //static ID3D11DeviceContext* g_pd3dDeviceContext = NULL;
    //static IDXGISwapChain* g_pSwapChain = NULL;
    //static ID3D11RenderTargetView* g_mainRenderTargetView = NULL;

	void RHI::Create_RHI_SwapChian_Impl(RHI_SwapChain& _swapChian)
	{
        // Setup swap chain
        DXGI_SWAP_CHAIN_DESC sd;
        ZeroMemory(&sd, sizeof(sd));
        sd.BufferCount = _swapChian.BufferCount;
        sd.BufferDesc.Width = _swapChian.Width;
        sd.BufferDesc.Height = _swapChian.Height;
        sd.BufferDesc.Format = D3d11_Format[_swapChian.format];
        sd.BufferDesc.RefreshRate.Numerator = _swapChian.Numerator;
        sd.BufferDesc.RefreshRate.Denominator = 1;
        sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
        sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        sd.OutputWindow = hWnd;
        sd.SampleDesc.Count = _swapChian.SamplerCount;
        sd.SampleDesc.Quality = _swapChian.SamplerQuality;
        sd.Windowed = TRUE;
        sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	}

}

#endif