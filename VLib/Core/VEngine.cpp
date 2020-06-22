#include "VEngine.h"

#include <assert.h>
#include <iostream>

namespace V
{

    VEngine::VEngine()
    {
        assert(g_Engine==nullptr);
        g_Engine = this;
        unique_IOHI = std::make_unique<VIOHI>();
        unique_RHI = std::make_unique<RHI>();
    }

    void VEngine::Terminate()
    {
        std::cout << "VEngine Terminate......" << std::endl;
        //Prepare to do the task......

    }

    bool VEngine::Init()
    {
        bool result = unique_RHI->Init();
        return result;
    }

    void VEngine::Update()
    {
        unique_RHI->Update();
    }

	void VEngine::ResizeWindow(unsigned int _width, unsigned int _height, void* _handle, void* _instance)
	{
        unique_RHI->ResizeSurface(_width,_height,_handle,_instance);
	}

    VEngine* VEngine::GetEngine()
    {
        return g_Engine;
    }

}
