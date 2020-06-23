#if defined(__linux__) || defined(FORCEGL)

#include "../RHI.h"

namespace V
{
    bool RHI::Platform_Create_Context(RHI_SwapChain& _swapChian)
    {
       return true;
    }

    void RHI::Platform_Update_ImGui_NewFrame()
    {
       
    }

    void RHI::Platform_Rendering_ImGUI()
    {
    
    }

    void RHI::Platform_Destroy()
    {

        
    }

    void RHI::ResizeSurface(unsigned int _width, unsigned int _height, void* _handle, void* _instance)
    {

       
    }

}

#endif