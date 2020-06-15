
#include "../RHI.h"

#if defined(API_GRAPHICS_D3D11)
//===================================================================================================
	#pragma comment(lib, "d3d11.lib")
	#pragma comment(lib, "dxgi.lib")
	#pragma comment(lib, "d3dcompiler.lib")
	#pragma warning(push, 0) // Hide warnings which belong DirectX
	#include <d3d11_4.h>
	#pragma warning(pop)

	#include "D3D11_Utility.h"

	using namespace std;

	namespace V
	{
		void RHI_VertexBuffer::_destroy()
		{
			d3d11_utility::release(*reinterpret_cast<ID3D11Buffer**>(&m_buffer));
		}

		bool RHI_VertexBuffer::_create(const void* vertices)
		{
			if (!m_rhi_device || !m_rhi_device->GetContextRhi()->device_context)
			{

			}
		}
	}
//===================================================================================================
#endif