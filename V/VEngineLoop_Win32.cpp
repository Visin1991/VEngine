#ifdef _WIN32

// dear imgui - standalone example application for DirectX 11
// If you are new to dear imgui, see examples/README.txt and documentation at the top of imgui.cpp.

#include <memory>
#include <string>

#include <Windows.h>
#include <tchar.h>


#include "VEngineLoop.h"
#include <ImGui/imgui.h>
#include <VEngine.h>

using namespace V;
std::unique_ptr<V::VEngine> g_engine;


// Forward declare message handler from imgui_impl_win32.cpp.
// Right now, I just use IMGUI win32 implmentation of processing Input and event.
// Later I will replace with my own implementation
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// Win32 message handler------Callback function will recive the window system event {Resize window, Input,Close window......}
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
        return true;

    switch (msg)
    {
    case WM_SIZE:
        if (wParam != SIZE_MINIMIZED)
        {
            g_engine->ResizeSurface((UINT)LOWORD(lParam), (UINT)HIWORD(lParam), hWnd, nullptr);
        }
        return 0;
    case WM_SYSCOMMAND:
        if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
            return 0;
        break;
    case WM_DESTROY:
        ::PostQuitMessage(0);
        return 0;
    }
    return ::DefWindowProc(hWnd, msg, wParam, lParam);
}


void VEngineLoop::Run()
{
    //Create Engine
    g_engine = std::make_unique<V::VEngine>();

    // Create application window-------Register WindowPro callback function
    WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(NULL), NULL, NULL, NULL, NULL, _T("VEngine"), NULL };
    ::RegisterClassEx(&wc);

    const int windowWidth = GetSystemMetrics(SM_CXSCREEN);
    const int windowHeight = GetSystemMetrics(SM_CYSCREEN);
    HWND hwnd = ::CreateWindow(wc.lpszClassName, _T("VEngine"), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, windowWidth, windowHeight, NULL, NULL, wc.hInstance, NULL);

    // Show the window
    ::ShowWindow(hwnd, SW_MAXIMIZE);
    ::UpdateWindow(hwnd);
    ::SetFocus(hwnd);

    if (g_engine->Init(&wc, hwnd))
    {
        g_engine->Update();
    }

    //Exit();
    ::DestroyWindow(hwnd);
    ::UnregisterClass(wc.lpszClassName, wc.hInstance);
}

#endif

