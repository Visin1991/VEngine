#ifdef _WIN32

#include <iostream>

#include "../RHI.h"
#include "../../ImGui/ImGui_Platform/imgui_impl_dx11.h"
#include "../../ImGui/ImGui_Platform/imgui_impl_win32.h"


#include <d3d11.h>
#pragma comment (lib, "d3d11.lib")


#include "../../Core/VEngine.h"

static const DXGI_FORMAT D3d11_Format[] =
{
    DXGI_FORMAT_R8_UNORM,
    DXGI_FORMAT_R16_FLOAT,

    DXGI_FORMAT_R11G11B10_FLOAT,

    DXGI_FORMAT_R8G8B8A8_UNORM,
    DXGI_FORMAT_R16G16B16A16_FLOAT
};

// Data
static ID3D11Device* gg_pd3dDevice = NULL;
static ID3D11DeviceContext* gg_pd3dDeviceContext = NULL;
static IDXGISwapChain* gg_pSwapChain = NULL;
static ID3D11RenderTargetView* gg_mainRenderTargetView = NULL;

void CreateDefaultRenderTarget()
{
    //Create the default render target
    ID3D11Texture2D* pBackBuffer;
    gg_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
    gg_pd3dDevice->CreateRenderTargetView(pBackBuffer, NULL, &gg_mainRenderTargetView);
    pBackBuffer->Release();
}

void CleanupRenderTarget()
{
    if (gg_mainRenderTargetView) { gg_mainRenderTargetView->Release(); gg_mainRenderTargetView = NULL; }
}

void CleanupDeviceD3D()
{
    CleanupRenderTarget();
    if (gg_pSwapChain) { gg_pSwapChain->Release(); gg_pSwapChain = NULL; }
    if (gg_pd3dDeviceContext) { gg_pd3dDeviceContext->Release(); gg_pd3dDeviceContext = NULL; }
    if (gg_pd3dDevice) { gg_pd3dDevice->Release(); gg_pd3dDevice = NULL; }
}

namespace V
{
    bool RHI::Platform_Create_Context(RHI_SwapChain& _swapChian)
    {
        auto engine = VEngine::GetEngine();
        assert(engine!=nullptr);

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
        sd.OutputWindow = (HWND)engine->unique_IOHI->window;
        sd.SampleDesc.Count = _swapChian.SamplerCount;
        sd.SampleDesc.Quality = _swapChian.SamplerQuality;
        sd.Windowed = TRUE;
        sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

        UINT createDeviceFlags = 0;
        D3D_FEATURE_LEVEL featureLevel;
        const D3D_FEATURE_LEVEL featureLevelArray[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, };
        if (D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &gg_pSwapChain, &gg_pd3dDevice, &featureLevel, &gg_pd3dDeviceContext) != S_OK)
            return false;

        CreateDefaultRenderTarget();

        //Create IMGUI
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
            ImGui_ImplWin32_Init(engine->unique_IOHI->window);
            ImGui_ImplDX11_Init(gg_pd3dDevice, gg_pd3dDeviceContext);


            show_demo_window = true;
            show_another_window = false;
            clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
        }

		return true;

    }

    void RHI::Platform_Update_ImGui_NewFrame()
    {
        // Start the Dear ImGui frame
        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();
    }

    void RHI::Platform_Rendering_ImGUI()
    {
        // Rendering
        ImGui::Render();
        gg_pd3dDeviceContext->OMSetRenderTargets(1, &gg_mainRenderTargetView, NULL);
        gg_pd3dDeviceContext->ClearRenderTargetView(gg_mainRenderTargetView, (float*)&clear_color);
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
        gg_pSwapChain->Present(1, 0); // Present with vsync
        //g_pSwapChain->Present(0, 0); // Present without vsync
    }

    void RHI::Platform_Destroy()
    {

        // Cleanup
        ImGui_ImplDX11_Shutdown();
        ImGui_ImplWin32_Shutdown();
        ImGui::DestroyContext();
        CleanupDeviceD3D();
    }

    void RHI::ResizeSurface(unsigned int _width, unsigned int _height, void* _handle, void* _instance)
    {

        if (gg_pd3dDevice != NULL)
        {
            CleanupRenderTarget();
            gg_pSwapChain->ResizeBuffers(0, _width, _height, DXGI_FORMAT_UNKNOWN, 0);
            CreateDefaultRenderTarget();
        }
        std::cout << "Ret Surface Info : Width " << _width << " | Height : " << _height << std::endl;
    }

}

#endif