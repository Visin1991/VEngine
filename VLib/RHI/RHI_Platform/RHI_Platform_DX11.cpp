#ifdef _WIN32

#include <iostream>

#include "../RHI.h"
#include "ImGui/ImGui_Platform/imgui_impl_dx11.h"
#include "ImGui/ImGui_Platform/imgui_impl_win32.h"


#include <d3d11.h>
#pragma comment (lib, "d3d11.lib")

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
	bool RHI::Create_RHI_Context_Platform(RHI_SwapChain& _swapChian)
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
        sd.OutputWindow = (HWND)hWnd;
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

        return true;
	}

    void RHI::Create_ImGui_Platform()
    {
        // Setup Platform/Renderer bindings
        ImGui_ImplWin32_Init(hWnd);
        ImGui_ImplDX11_Init(gg_pd3dDevice, gg_pd3dDeviceContext);
    }

    void RHI::Update_Platform()
    {
        // Our state
        bool show_demo_window = true;
        bool show_another_window = false;
        ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

        // Main loop
        MSG msg;
        ZeroMemory(&msg, sizeof(msg));
        while (msg.message != WM_QUIT)
        {
            // Poll and handle messages (inputs, window resize, etc.)
            // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
            // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application.
            // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application.
            // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
            if (::PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
            {
                ::TranslateMessage(&msg);
                ::DispatchMessage(&msg);
                continue;
            }

            // Start the Dear ImGui frame
            ImGui_ImplDX11_NewFrame();
            ImGui_ImplWin32_NewFrame();


            ImGui::NewFrame();

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

            // Rendering
            ImGui::Render();
            gg_pd3dDeviceContext->OMSetRenderTargets(1, &gg_mainRenderTargetView, NULL);
            gg_pd3dDeviceContext->ClearRenderTargetView(gg_mainRenderTargetView, (float*)&clear_color);
            ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

            gg_pSwapChain->Present(1, 0); // Present with vsync
            //g_pSwapChain->Present(0, 0); // Present without vsync
        }

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