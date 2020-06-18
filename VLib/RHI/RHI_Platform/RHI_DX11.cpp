//#pragma once
//
//#ifdef _WIN32
//
//#include <iostream>
//
//#include "../RHI.h"
//
//#include <ImGui/imgui.h>
//#include <ImGui/ImGui_Platform/imgui_impl_win32.h>
//#include <ImGui/ImGui_Platform/imgui_impl_dx11.h>
//
//#include <d3d11.h>
//#define DIRECTINPUT_VERSION 0x0800
//#include <dinput.h>
//
//// include the Direct3D Library file
//#pragma comment (lib, "d3d11.lib")
//
//namespace V
//{
//    // Data
//    static ID3D11Device* g_pd3dDevice = NULL;
//    static ID3D11DeviceContext* g_pd3dDeviceContext = NULL;
//    static IDXGISwapChain* g_pSwapChain = NULL;
//    static ID3D11RenderTargetView* g_mainRenderTargetView = NULL;
//
//    // Forward declarations of helper functions
//    bool CreateDeviceD3D(HWND hWnd);
//    void CleanupDeviceD3D();
//    void CreateRenderTarget();
//    void CleanupRenderTarget();
//
//    // Helper functions
//
//    bool CreateDeviceD3D(HWND hWnd)
//    {
//        // Setup swap chain
//        DXGI_SWAP_CHAIN_DESC sd;
//        ZeroMemory(&sd, sizeof(sd));
//        sd.BufferCount = 2;
//        sd.BufferDesc.Width = 0;
//        sd.BufferDesc.Height = 0;
//        sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
//        sd.BufferDesc.RefreshRate.Numerator = 60;
//        sd.BufferDesc.RefreshRate.Denominator = 1;
//        sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
//        sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
//        sd.OutputWindow = hWnd;
//        sd.SampleDesc.Count = 1;
//        sd.SampleDesc.Quality = 0;
//        sd.Windowed = TRUE;
//        sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
//
//        UINT createDeviceFlags = 0;
//        //createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
//        D3D_FEATURE_LEVEL featureLevel;
//        const D3D_FEATURE_LEVEL featureLevelArray[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, };
//        if (D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &featureLevel, &g_pd3dDeviceContext) != S_OK)
//            return false;
//
//        CreateRenderTarget();
//        return true;
//    }
//
//    void CleanupDeviceD3D()
//    {
//        CleanupRenderTarget();
//        if (g_pSwapChain) { g_pSwapChain->Release(); g_pSwapChain = NULL; }
//        if (g_pd3dDeviceContext) { g_pd3dDeviceContext->Release(); g_pd3dDeviceContext = NULL; }
//        if (g_pd3dDevice) { g_pd3dDevice->Release(); g_pd3dDevice = NULL; }
//    }
//
//    void CreateRenderTarget()
//    {
//        ID3D11Texture2D* pBackBuffer;
//        g_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
//        g_pd3dDevice->CreateRenderTargetView(pBackBuffer, NULL, &g_mainRenderTargetView);
//        pBackBuffer->Release();
//    }
//
//    void CleanupRenderTarget()
//    {
//        if (g_mainRenderTargetView) { g_mainRenderTargetView->Release(); g_mainRenderTargetView = NULL; }
//    }
//
//    bool RHI::Init(void* _wc, void* _hwnd)
//    {
//        //Save a reference for RHI
//        hWnd = _hwnd;
//        wc = _wc;
//
//        //make Scope variable
//        HWND s_hwnd = HWND(_hwnd);
//        WNDCLASSEX s_wc = (WNDCLASSEX&)_wc;
//
//        // Initialize Direct3D
//        if (!CreateDeviceD3D(s_hwnd))
//        {
//            CleanupDeviceD3D();
//            ::UnregisterClass(s_wc.lpszClassName, s_wc.hInstance);
//            return false;
//        }
//
//        // Setup Dear ImGui context
//        IMGUI_CHECKVERSION();
//        ImGui::CreateContext();
//        ImGuiIO& io = ImGui::GetIO(); (void)io;
//        //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
//        //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
//
//        // Setup Dear ImGui style
//        ImGui::StyleColorsDark();
//        //ImGui::StyleColorsClassic();
//
//        // Setup Platform/Renderer bindings
//        ImGui_ImplWin32_Init(hWnd);
//        ImGui_ImplDX11_Init(g_pd3dDevice, g_pd3dDeviceContext);
//
//        // Load Fonts
//        // - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
//        // - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
//        // - If the file cannot be loaded, the function will return NULL. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
//        // - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
//        // - Read 'docs/FONTS.txt' for more instructions and details.
//        // - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
//        //io.Fonts->AddFontDefault();
//        //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
//        //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
//        //io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
//        //io.Fonts->AddFontFromFileTTF("../../misc/fonts/ProggyTiny.ttf", 10.0f);
//        //ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
//        //IM_ASSERT(font != NULL);
//
//        return true;
//    }
//
//    
//
//    void RHI::ResizeSurface(unsigned int _width, unsigned int _height, void* _handle, void* _instance)
//    {
//
//        if (g_pd3dDevice != NULL)
//        {
//            CleanupRenderTarget();
//            g_pSwapChain->ResizeBuffers(0, _width, _height, DXGI_FORMAT_UNKNOWN, 0);
//            CreateRenderTarget();
//        }
//        std::cout << "Ret Surface Info : Width " << _width << " | Height : " << _height << std::endl;
//    }
//
//}
//#endif