#include "VEngine.h"

namespace V
{

    bool VEngine::Init(void* _wc, void* _hwnd)
    {
        bool result = m_rhi.Init(_wc, _hwnd);
        return result;
    }

    void VEngine::Update()
    {
        m_rhi.Update();
    }

	void VEngine::ResizeSurface(unsigned int _width, unsigned int _height, void* _handle, void* _instance)
	{
        m_rhi.ResizeSurface(_width,_height,_handle,_instance);
	}

}
