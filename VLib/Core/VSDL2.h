#pragma once

#include "VEngineConfig.h"


namespace V
{
    class V_API VSDL2
    {
    public:
        VSDL2()=default;
        int Init();
    private:
        void Init_ImGui();
    };
} // namespace V


