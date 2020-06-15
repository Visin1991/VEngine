#pragma once

#include "../Core/EngineDefs.h"

#include "RHI_API.h"

#if defined(API_GRAPHICS_D3D11)
	#include "D3D11/RHI_D3D11.h"
#endif


namespace V
{
	struct RHI_Context
	{
	#if defined(API_GRAPHICS_D3D11)
			RHI_Api_Type api_type = RHI_Api_D3d11;
			ID3D11Device5* device = nullptr;
			ID3D11DeviceContext4* device_context = nullptr;
			ID3DUserDefinedAnnotation* annotation = nullptr;
	#endif

	#if defined(API_GRAPHICS_D3D12)
				RHI_Api_Type api_type = RHI_Api_D3d12;
				ID3D12Device* device = nullptr;
	#endif

	#if defined(API_GRAPHICS_VULKAN)

	#endif

	#ifdef DEBUG
		bool debug = true;
		bool markers = true;
		bool profiler = true;
	#else
		bool debug = false;
		bool markers = false;
		bool profiler = true;
	#endif

		// Shader resource slot shifts
		static const uint32_t shader_shift_rw_buffer = 000;
		static const uint32_t shader_shift_buffer = 100;
		static const uint32_t shader_shift_texture = 200;
		static const uint32_t shader_shift_sampler = 300;

		static const uint32_t descriptor_max_constant_buffers = 10;
		static const uint32_t descriptor_max_constant_buffers_dynamic = 10;
		static const uint32_t descriptor_max_samplers = 10;
		static const uint32_t descriptor_max_textures = 10;

		uint32_t max_texture_dimension_2d = 16384;
		uint32_t max_msaa_level = 0;

		void* queue_graphics = nullptr;
		void* queue_compute = nullptr;
		void* queue_transfer = nullptr;
		uint32_t queue_graphics_index = 0;
		uint32_t queue_transfer_index = 0;
		uint32_t queue_compute_index = 0;
	};
}