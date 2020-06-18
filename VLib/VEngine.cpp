#include "VEngine.h"

#include <assert.h>
#include <iostream>

namespace V
{
    void VEngine::Terminate()
    {
        std::cout << "VEngine Terminate......" << std::endl;
        //Prepare to do the task......

    }

    bool VEngine::Init(void* _wc, void* _hwnd)
    {
        unique_RHI = std::make_unique<RHI>();
        bool result = unique_RHI->Init(_wc, _hwnd);

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

}
