#pragma once

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma warning(push, 0) 
#include <d3d11_4.h>
#pragma warning(pop)


#include <wrl/client.h>

#include "../../Core/Log.h"

#include "../RHI_API.h"


static const D3D11_CULL_MODE d3d11_cull_mode[] =
{
	D3D11_CULL_NONE,
	D3D11_CULL_FRONT,
	D3D11_CULL_BACK
};

static const D3D11_FILL_MODE d3d11_polygon_mode[] =
{
	D3D11_FILL_SOLID,
	D3D11_FILL_WIREFRAME
};

static const D3D11_PRIMITIVE_TOPOLOGY d3d11_primitive_topology[] =
{
	D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
	D3D11_PRIMITIVE_TOPOLOGY_LINELIST
};

static const DXGI_FORMAT d3d11_format[] =
{
	// R
	DXGI_FORMAT_R8_UNORM,
	DXGI_FORMAT_R16_UINT,
	DXGI_FORMAT_R16_FLOAT,
	DXGI_FORMAT_R32_UINT,
	DXGI_FORMAT_R32_FLOAT,
	// RG
	DXGI_FORMAT_R8G8_UNORM,
	DXGI_FORMAT_R16G16_FLOAT,
	DXGI_FORMAT_R32G32_FLOAT,
	// RGB
	DXGI_FORMAT_R11G11B10_FLOAT,
	DXGI_FORMAT_R32G32B32_FLOAT,
	// RGBA
	DXGI_FORMAT_R8G8B8A8_UNORM,
	DXGI_FORMAT_R10G10B10A2_UNORM,
	DXGI_FORMAT_R16G16B16A16_FLOAT,
	DXGI_FORMAT_R32G32B32A32_FLOAT,
	// Depth
	DXGI_FORMAT_D32_FLOAT,
	DXGI_FORMAT_D32_FLOAT_S8X24_UINT,

	DXGI_FORMAT_UNKNOWN
};

static const D3D11_TEXTURE_ADDRESS_MODE d3d11_sampler_address_mode[] =
{
	D3D11_TEXTURE_ADDRESS_WRAP,
	D3D11_TEXTURE_ADDRESS_MIRROR,
	D3D11_TEXTURE_ADDRESS_CLAMP,
	D3D11_TEXTURE_ADDRESS_BORDER,
	D3D11_TEXTURE_ADDRESS_MIRROR_ONCE
};

static const D3D11_COMPARISON_FUNC d3d11_comparison_function[] =
{
	D3D11_COMPARISON_NEVER,
	D3D11_COMPARISON_LESS,
	D3D11_COMPARISON_EQUAL,
	D3D11_COMPARISON_LESS_EQUAL,
	D3D11_COMPARISON_GREATER,
	D3D11_COMPARISON_NOT_EQUAL,
	D3D11_COMPARISON_GREATER_EQUAL,
	D3D11_COMPARISON_ALWAYS
};

static const D3D11_STENCIL_OP d3d11_stencil_operation[] =
{
	D3D11_STENCIL_OP_KEEP,
	D3D11_STENCIL_OP_ZERO,
	D3D11_STENCIL_OP_REPLACE,
	D3D11_STENCIL_OP_INCR_SAT,
	D3D11_STENCIL_OP_DECR_SAT,
	D3D11_STENCIL_OP_INVERT,
	D3D11_STENCIL_OP_INCR,
	D3D11_STENCIL_OP_DECR
};

static const D3D11_BLEND d3d11_blend_factor[] =
{
	D3D11_BLEND_ZERO,
	D3D11_BLEND_ONE,
	D3D11_BLEND_SRC_COLOR,
	D3D11_BLEND_INV_SRC_COLOR,
	D3D11_BLEND_SRC_ALPHA,
	D3D11_BLEND_INV_SRC_ALPHA,
	D3D11_BLEND_DEST_ALPHA,
	D3D11_BLEND_INV_DEST_ALPHA,
	D3D11_BLEND_DEST_COLOR,
	D3D11_BLEND_INV_DEST_COLOR,
	D3D11_BLEND_SRC_ALPHA_SAT,
	D3D11_BLEND_BLEND_FACTOR,
	D3D11_BLEND_INV_BLEND_FACTOR,
	D3D11_BLEND_SRC1_COLOR,
	D3D11_BLEND_INV_SRC1_COLOR,
	D3D11_BLEND_SRC1_ALPHA,
	D3D11_BLEND_INV_SRC1_ALPHA
};

static const D3D11_BLEND_OP d3d11_blend_operation[] =
{
	D3D11_BLEND_OP_ADD,
	D3D11_BLEND_OP_SUBTRACT,
	D3D11_BLEND_OP_REV_SUBTRACT,
	D3D11_BLEND_OP_MIN,
	D3D11_BLEND_OP_MAX
};

namespace V::d3d11_utility
{
	struct globals
	{
		static inline RHI_Device* rhi_device;
		static inline RHI_Context* rhi_context;
	};

	inline const char* dxgi_error_to_string(const HRESULT error_code)
	{
		switch (error_code)
		{
		case DXGI_ERROR_DEVICE_HUNG:                    return "DXGI_ERROR_DEVICE_HUNG";					// The application's device failed due to badly formed commands sent by the application. This is an design-time issue that should be investigated and fixed.
		case DXGI_ERROR_DEVICE_REMOVED:                 return "DXGI_ERROR_DEVICE_REMOVED";					// The video card has been physically removed from the system, or a driver upgrade for the video card has occurred. The application should destroy and recreate the device. For help debugging the problem, call ID3D10Device::GetDeviceRemovedReason.
		case DXGI_ERROR_DEVICE_RESET:                   return "DXGI_ERROR_DEVICE_RESET";					// The device failed due to a badly formed command. This is a run-time issue; The application should destroy and recreate the device.
		case DXGI_ERROR_DRIVER_INTERNAL_ERROR:          return "DXGI_ERROR_DRIVER_INTERNAL_ERROR";			// The driver encountered a problem and was put into the device removed state.
		case DXGI_ERROR_FRAME_STATISTICS_DISJOINT:      return "DXGI_ERROR_FRAME_STATISTICS_DISJOINT";		// An event (for example, a power cycle) interrupted the gathering of presentation statistics.
		case DXGI_ERROR_GRAPHICS_VIDPN_SOURCE_IN_USE:   return "DXGI_ERROR_GRAPHICS_VIDPN_SOURCE_IN_USE";	// The application attempted to acquire exclusive ownership of an output, but failed because some other application (or device within the application) already acquired ownership.
		case DXGI_ERROR_INVALID_CALL:                   return "DXGI_ERROR_INVALID_CALL";					// The application provided invalid parameter data; this must be debugged and fixed before the application is released.
		case DXGI_ERROR_MORE_DATA:                      return "DXGI_ERROR_MORE_DATA";						// The buffer supplied by the application is not big enough to hold the requested data.
		case DXGI_ERROR_NONEXCLUSIVE:                   return "DXGI_ERROR_NONEXCLUSIVE";					// A global counter resource is in use, and the Direct3D device can't currently use the counter resource.
		case DXGI_ERROR_NOT_CURRENTLY_AVAILABLE:        return "DXGI_ERROR_NOT_CURRENTLY_AVAILABLE";		// The resource or request is not currently available, but it might become available later.
		case DXGI_ERROR_NOT_FOUND:                      return "DXGI_ERROR_NOT_FOUND";						// When calling IDXGIObject::GetPrivateData, the GUID passed in is not recognized as one previously passed to IDXGIObject::SetPrivateData or IDXGIObject::SetPrivateDataInterface. When calling IDXGIFentityy::EnumAdapters or IDXGIAdapter::EnumOutputs, the enumerated ordinal is out of range.
		case DXGI_ERROR_REMOTE_CLIENT_DISCONNECTED:     return "DXGI_ERROR_REMOTE_CLIENT_DISCONNECTED";		// Reserved
		case DXGI_ERROR_REMOTE_OUTOFMEMORY:             return "DXGI_ERROR_REMOTE_OUTOFMEMORY";				// Reserved
		case DXGI_ERROR_WAS_STILL_DRAWING:              return "DXGI_ERROR_WAS_STILL_DRAWING";				// The GPU was busy at the moment when a call was made to perform an operation, and did not execute or schedule the operation.
		case DXGI_ERROR_UNSUPPORTED:                    return "DXGI_ERROR_UNSUPPORTED";					// The requested functionality is not supported by the device or the driver.
		case DXGI_ERROR_ACCESS_LOST:                    return "DXGI_ERROR_ACCESS_LOST";					// The desktop duplication interface is invalid. The desktop duplication interface typically becomes invalid when a different type of image is displayed on the desktop.
		case DXGI_ERROR_WAIT_TIMEOUT:                   return "DXGI_ERROR_WAIT_TIMEOUT";					// The time-out interval elapsed before the next desktop frame was available.
		case DXGI_ERROR_SESSION_DISCONNECTED:           return "DXGI_ERROR_SESSION_DISCONNECTED";			// The Remote Desktop Services session is currently disconnected.
		case DXGI_ERROR_RESTRICT_TO_OUTPUT_STALE:       return "DXGI_ERROR_RESTRICT_TO_OUTPUT_STALE";		// The DXGI output (monitor) to which the swap chain content was restricted is now disconnected or changed.
		case DXGI_ERROR_CANNOT_PROTECT_CONTENT:         return "DXGI_ERROR_CANNOT_PROTECT_CONTENT";			// DXGI can't provide content protection on the swap chain. This error is typically caused by an older driver, or when you use a swap chain that is incompatible with content protection.
		case DXGI_ERROR_ACCESS_DENIED:                  return "DXGI_ERROR_ACCESS_DENIED";					// You tried to use a resource to which you did not have the required access privileges. This error is most typically caused when you write to a shared resource with read-only access.
		case DXGI_ERROR_NAME_ALREADY_EXISTS:            return "DXGI_ERROR_NAME_ALREADY_EXISTS";			// The supplied name of a resource in a call to IDXGIResource1::CreateSharedHandle is already associated with some other resource.
		case DXGI_ERROR_SDK_COMPONENT_MISSING:          return "DXGI_ERROR_SDK_COMPONENT_MISSING";			// The application requested an operation that depends on an SDK component that is missing or mismatched.
		case DXGI_ERROR_NOT_CURRENT:                    return "DXGI_ERROR_NOT_CURRENT";                    // The DXGI objects that the application has created are no longer current & need to be recreated for this operation to be performed.
		case DXGI_ERROR_HW_PROTECTION_OUTOFMEMORY:      return "DXGI_ERROR_HW_PROTECTION_OUTOFMEMORY";      // Insufficient HW protected memory exits for proper function.
		case DXGI_ERROR_DYNAMIC_CODE_POLICY_VIOLATION:  return "DXGI_ERROR_DYNAMIC_CODE_POLICY_VIOLATION";  // Creating this device would violate the process's dynamic code policy.
		case DXGI_ERROR_NON_COMPOSITED_UI:              return "DXGI_ERROR_NON_COMPOSITED_UI";              // The operation failed because the compositor is not in control of the output.
		case E_INVALIDARG:								return "E_INVALIDARG";								// One or more arguments are invalid.
		}

		return "Unknown error code";
	}

	constexpr bool error_check(const HRESULT result)
	{
		if (FAILED(result))
		{
			LOG_ERROR("%s", dxgi_error_to_string(result));
			return false;
		}

		return true;
	}

	template <typename T>
	constexpr void release(T*& ptr)
	{
		if (ptr)
		{
			ptr->Release();
			ptr = nullptr;
		}
	}

	inline void DetectAdapters()
	{
		// Create DirectX graphics interface factory
		IDXGIFactory1* factory;
		const auto result = CreateDXGIFactory1(IID_PPV_ARGS(&factory));
		if (FAILED(result))
		{
			LOG_ERROR("Failed to create a DirectX graphics interface factory, %s.", dxgi_error_to_string(result));
			return;
		}

		const auto get_available_adapters = [](IDXGIFactory1* factory)
		{
			uint32_t i = 0;
			IDXGIAdapter* adapter;
			std::vector<IDXGIAdapter*> adapters;
			while (factory->EnumAdapters(i, &adapter) != DXGI_ERROR_NOT_FOUND)
			{
				adapters.emplace_back(adapter);
				++i;
			}

			return adapters;
		};

		// Get all available adapters
		auto adapters = get_available_adapters(factory);
		release(factory);
		if (adapters.empty())
		{
			LOG_ERROR("Couldn't find any adapters");
			return;
		}

		// Save all available adapters
		DXGI_ADAPTER_DESC adapter_desc;
		for (auto display_adapter : adapters)
		{
			if (FAILED(display_adapter->GetDesc(&adapter_desc)))
			{
				LOG_ERROR("Failed to get adapter description");
				continue;
			}

			// Of course it wouldn't be simple, lets convert the device name
			char name[128];
			auto def_char = ' ';
			WideCharToMultiByte(CP_ACP, 0, adapter_desc.Description, -1, name, 128, &def_char, nullptr);

			globals::rhi_device->RegisterPhysicalDevice(PhysicalDevice
			(
				11 << 22,                                                   // api version
				0,                                                          // driver version
				adapter_desc.VendorId,                                      // vendor id
				RHI_PhysicalDevice_Unknown,                                 // type
				&name[0],                                                   // name
				static_cast<uint32_t>(adapter_desc.DedicatedVideoMemory),   // memory
				static_cast<void*>(display_adapter))                        // data
				);
		}

		// DISPLAY MODES
		const auto get_display_modes = [](IDXGIAdapter* adapter, RHI_Format format)
		{
			// Enumerate the primary adapter output (monitor).
			IDXGIOutput* adapter_output = nullptr;
			bool result = SUCCEEDED(adapter->EnumOutputs(0, &adapter_output));
			if (result)
			{
				// Get supported display mode count
				UINT display_mode_count = 0;
				result = SUCCEEDED(adapter_output->GetDisplayModeList(d3d11_format[format], DXGI_ENUM_MODES_INTERLACED, &display_mode_count, nullptr));
				if (result)
				{
					// Get display modes
					std::vector<DXGI_MODE_DESC> display_modes;
					display_modes.resize(display_mode_count);
					result = SUCCEEDED(adapter_output->GetDisplayModeList(d3d11_format[format], DXGI_ENUM_MODES_INTERLACED, &display_mode_count, &display_modes[0]));
					if (result)
					{
						// Save all the display modes
						for (const DXGI_MODE_DESC& mode : display_modes)
						{
							globals::rhi_device->RegisterDisplayMode(DisplayMode(mode.Width, mode.Height, mode.RefreshRate.Numerator, mode.RefreshRate.Denominator));
						}
					}
				}
				adapter_output->Release();
			}

			return result;
		};

		// Get display modes and set primary adapter
		for (uint32_t device_index = 0; device_index < globals::rhi_device->GetPhysicalDevices().size(); device_index++)
		{
			const PhysicalDevice& physical_device = globals::rhi_device->GetPhysicalDevices()[device_index];
			const auto dx_adapter = static_cast<IDXGIAdapter*>(physical_device.GetData());

			// Adapters are ordered by memory (descending), so stop on the first success
			const auto format = RHI_Format_R8G8B8A8_Unorm; // TODO: This must come from the swapchain
			if (get_display_modes(dx_adapter, format))
			{
				globals::rhi_device->SetPrimaryPhysicalDevice(device_index);
				return;
			}
			else
			{
				LOG_ERROR("Failed to get display modes for \"%s\".", physical_device.GetName().c_str());
			}
		}

		// If we failed to detect any display modes but we have at least one adapter, use it.
		if (globals::rhi_device->GetPhysicalDevices().size() != 0)
		{
			LOG_ERROR("Failed to detect display modes for all physical devices, falling back to first available.");
			globals::rhi_device->SetPrimaryPhysicalDevice(0);
		}
	}

	// Determines whether tearing support is available for fullscreen borderless windows.
	inline bool CheckTearingSupport()
	{
		// Rather than create the 1.5 factory interface directly, we create the 1.4
		// interface and query for the 1.5 interface. This will enable the graphics
		// debugging tools which might not support the 1.5 factory interface
		Microsoft::WRL::ComPtr<IDXGIFactory4> factory4;
		HRESULT resut = CreateDXGIFactory1(IID_PPV_ARGS(&factory4));
		BOOL allowTearing = FALSE;
		if (SUCCEEDED(resut))
		{
			Microsoft::WRL::ComPtr<IDXGIFactory5> factory5;
			resut = factory4.As(&factory5);
			if (SUCCEEDED(resut))
			{
				resut = factory5->CheckFeatureSupport(DXGI_FEATURE_PRESENT_ALLOW_TEARING, &allowTearing, sizeof(allowTearing));
			}
		}

		const bool fullscreen_borderless_support = SUCCEEDED(resut) && allowTearing;
		const bool vendor_support = !globals::rhi_device->GetPrimaryPhysicalDevice()->IsIntel(); // Intel, bad

		return fullscreen_borderless_support && vendor_support;
	}

	namespace swap_chain
	{
		inline uint32_t validate_flags(uint32_t flags)
		{
			// If SwapChain_Allow_Tearing was requested
			if (flags & RHI_Present_Immediate)
			{
				// Check if the adapter supports it, if not, disable it (tends to fail with Intel adapters)
				if (!CheckTearingSupport())
				{
					flags &= ~RHI_Present_Immediate;
					LOG_WARNING("Present_Immediate was requested but it's not supported by the adapter.");
				}
			}

			return flags;
		}

		inline UINT get_flags(uint32_t flags)
		{
			UINT d3d11_flags = 0;

			d3d11_flags |= flags & RHI_SwapChain_Allow_Mode_Switch ? DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH : 0;
			d3d11_flags |= flags & RHI_Present_Immediate ? DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING : 0;

			return d3d11_flags;
		}

		inline DXGI_SWAP_EFFECT get_swap_effect(uint32_t flags)
		{
#if !defined(_WIN32_WINNT_WIN10)
			if (flags & RHI_Swap_Flip_Discard)
			{
				LOG_WARNING("Swap_Flip_Discard was requested but it's only supported in Windows 10, using Swap_Discard instead.");
				flags &= ~RHI_Swap_Flip_Discard;
				flags |= RHI_Swap_Discard;
			}
#endif

			if (flags & RHI_Swap_Flip_Discard && globals::rhi_device->GetPrimaryPhysicalDevice()->IsIntel())
			{
				LOG_WARNING("Swap_Flip_Discard was requested but it's not supported by Intel adapters, using Swap_Discard instead.");
				flags &= ~RHI_Swap_Flip_Discard;
				flags |= RHI_Swap_Discard;
			}

			if (flags & RHI_Swap_Discard)           return DXGI_SWAP_EFFECT_DISCARD;
			if (flags & RHI_Swap_Sequential)        return DXGI_SWAP_EFFECT_SEQUENTIAL;
			if (flags & RHI_Swap_Flip_Discard)      return DXGI_SWAP_EFFECT_FLIP_DISCARD;
			if (flags & RHI_Swap_Flip_Sequential)   return DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;

			LOG_ERROR("Unable to determine the requested swap effect, opting for DXGI_SWAP_EFFECT_DISCARD");
			return DXGI_SWAP_EFFECT_DISCARD;
		}
	}

	namespace sampler
	{
		inline D3D11_FILTER get_filter(const RHI_Filter filter_min, const RHI_Filter filter_mag, const RHI_Sampler_Mipmap_Mode filter_mipmap, bool anisotropy_enabled, bool comparison_enabled)
		{
			if (anisotropy_enabled)
				return !comparison_enabled ? D3D11_FILTER_ANISOTROPIC : D3D11_FILTER_COMPARISON_ANISOTROPIC;

			if ((filter_min == RHI_Filter_Nearest) && (filter_mag == RHI_Filter_Nearest) && (filter_mipmap == RHI_Filter_Nearest))			return !comparison_enabled ? D3D11_FILTER_MIN_MAG_MIP_POINT : D3D11_FILTER_COMPARISON_MIN_MAG_MIP_POINT;
			if ((filter_min == RHI_Filter_Nearest) && (filter_mag == RHI_Filter_Nearest) && (filter_mipmap == RHI_Sampler_Mipmap_Linear))	return !comparison_enabled ? D3D11_FILTER_MIN_MAG_POINT_MIP_LINEAR : D3D11_FILTER_COMPARISON_MIN_MAG_POINT_MIP_LINEAR;
			if ((filter_min == RHI_Filter_Nearest) && (filter_mag == RHI_Sampler_Mipmap_Linear) && (filter_mipmap == RHI_Filter_Nearest))			return !comparison_enabled ? D3D11_FILTER_MIN_POINT_MAG_LINEAR_MIP_POINT : D3D11_FILTER_COMPARISON_MIN_POINT_MAG_LINEAR_MIP_POINT;
			if ((filter_min == RHI_Filter_Nearest) && (filter_mag == RHI_Sampler_Mipmap_Linear) && (filter_mipmap == RHI_Sampler_Mipmap_Linear))	return !comparison_enabled ? D3D11_FILTER_MIN_POINT_MAG_MIP_LINEAR : D3D11_FILTER_COMPARISON_MIN_POINT_MAG_MIP_LINEAR;
			if ((filter_min == RHI_Sampler_Mipmap_Linear) && (filter_mag == RHI_Filter_Nearest) && (filter_mipmap == RHI_Filter_Nearest))			return !comparison_enabled ? D3D11_FILTER_MIN_LINEAR_MAG_MIP_POINT : D3D11_FILTER_COMPARISON_MIN_LINEAR_MAG_MIP_POINT;
			if ((filter_min == RHI_Sampler_Mipmap_Linear) && (filter_mag == RHI_Filter_Nearest) && (filter_mipmap == RHI_Sampler_Mipmap_Linear))	return !comparison_enabled ? D3D11_FILTER_MIN_LINEAR_MAG_POINT_MIP_LINEAR : D3D11_FILTER_COMPARISON_MIN_LINEAR_MAG_POINT_MIP_LINEAR;
			if ((filter_min == RHI_Sampler_Mipmap_Linear) && (filter_mag == RHI_Sampler_Mipmap_Linear) && (filter_mipmap == RHI_Filter_Nearest))			return !comparison_enabled ? D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT : D3D11_FILTER_COMPARISON_MIN_MAG_LINEAR_MIP_POINT;
			if ((filter_min == RHI_Sampler_Mipmap_Linear) && (filter_mag == RHI_Sampler_Mipmap_Linear) && (filter_mipmap == RHI_Sampler_Mipmap_Linear))	return !comparison_enabled ? D3D11_FILTER_MIN_MAG_MIP_LINEAR : D3D11_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR;

			assert(false && "D3D11_Sampler filter not supported.");
			return D3D11_FILTER_MIN_MAG_MIP_POINT;
		}
	}

}
