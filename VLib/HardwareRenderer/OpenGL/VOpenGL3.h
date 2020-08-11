#pragma once

#include "../../Core/VEngineConfig.h"
#include "imgui.h"

namespace V
{
    V_API bool OpenGL3_Init(const char* glsl_version);
    V_API void OpenGL3_Shutdown();
    V_API void OpenGL3_NewFrame();
    V_API void OpenGL3_RendererDrawData(ImDrawData* draw_data);

    V_API void OpenGL3_CreateFontsTexture();
    V_API void OpenGL3_DestroyFontsTexture();
    V_API void OpenGL3_CreateDeviceObjects();
    V_API void OpenGL3_DestroyDeviceObjects();
}


