#pragma once

#include "VRenderer.h"

namespace V
{
    class V_API VRenderer_ImGui : public VRenderer
    {
    public:
        virtual void Init();
        virtual void NewFrame();
        virtual void Update();
        virtual void Rendering();
    };

}