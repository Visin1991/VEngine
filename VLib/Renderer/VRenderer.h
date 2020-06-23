#pragma once

#include "../Core/VEngineConfig.h"

namespace V
{
    class V_API VRenderer
    {
    public:
        virtual void Init()=0;
        virtual void NewFrame()=0;
        virtual void Update()=0;
        virtual void Rendering()=0;
    };
}