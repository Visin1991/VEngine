
#include "../../Core/Settings.h"

#include "../RHI.h"
#include "RHI_D3D11.h"

using namespace std;

namespace V
{

#pragma region Device

	RHI_Device::RHI_Device(Context* context)
	{
		m_context = context;
		m_rhi_context = make_shared<RHI_Context>();
		d3d11_utility::globals::rhi_context = m_rhi_context.get();
		d3d11_utility::globals::rhi_device = this;
		const bool multithread_protection = true;

		d3d11_utility::DetectAdapters();

		m_rhi_context->max_texture_dimension_2d = D3D11_REQ_TEXTURE2D_U_OR_V_DIMENSION;

		const PhysicalDevice* physical_device = GetPrimaryPhysicalDevice();

		if (!physical_device)
		{
			LOG_ERROR("Failed to detect any devices");
			return;
		}

		{
			UINT device_flags = 0;

			if (m_rhi_context->debug)
			{
				device_flags |= D3D11_CREATE_DEVICE_DEBUG;
			}

			vector<D3D_FEATURE_LEVEL> feature_levels =
			{
				D3D_FEATURE_LEVEL_11_1,
				D3D_FEATURE_LEVEL_11_0,
				D3D_FEATURE_LEVEL_10_1,
				D3D_FEATURE_LEVEL_10_0,
				D3D_FEATURE_LEVEL_9_3,
				D3D_FEATURE_LEVEL_9_2,
				D3D_FEATURE_LEVEL_9_1
			};

			auto adapter = static_cast<IDXGIAdapter*>(physical_device->GetData());
			auto driver_type = adapter ? D3D_DRIVER_TYPE_UNKNOWN : D3D_DRIVER_TYPE_HARDWARE;

			auto create_device = [this, &adapter, &driver_type, &device_flags, &feature_levels]()
			{
				ID3D11Device* temp_device = nullptr;
				ID3D11DeviceContext* temp_context = nullptr;

				HRESULT result = D3D11CreateDevice(
					adapter,									// pAdapter: If nullptr, the default adapter will be used
					driver_type,								// DriverType
					nullptr,									// HMODULE: nullptr because DriverType = D3D_DRIVER_TYPE_HARDWARE
					device_flags,								// Flags
					feature_levels.data(),						// pFeatureLevels
					static_cast<UINT>(feature_levels.size()),	// FeatureLevels
					D3D11_SDK_VERSION,							// SDKVersion
					&temp_device,						        // ppDevice
					nullptr,									// pFeatureLevel
					&temp_context				                // ppImmediateContext
				);

				if (SUCCEEDED(result))
				{
					// Query old device for newer interface.
					if (!d3d11_utility::error_check(temp_device->QueryInterface(__uuidof(ID3D11Device5), (void**)&m_rhi_context->device)))
						return E_FAIL;

					// Release old device.
					d3d11_utility::release(temp_device);

					// Query old device context for newer interface.
					if (!d3d11_utility::error_check(temp_context->QueryInterface(__uuidof(ID3D11DeviceContext4), (void**)&m_rhi_context->device_context)))
						return E_FAIL;

					// Release old context.
					d3d11_utility::release(temp_context);
				}

				return result;
			};

			auto result = create_device();

			if (result == DXGI_ERROR_SDK_COMPONENT_MISSING)
			{
				LOG_WARNING("Failed to create device with D3D11_CREATE_DEVICE_DEBUG flags as it requires the DirectX SDK to be installed. Attempting to create a device without it.");
				device_flags &= ~D3D11_CREATE_DEVICE_DEBUG;
				result = create_device();
			}

			if (FAILED(result))
			{
				LOG_ERROR("Failed to create device, %s.", d3d11_utility::dxgi_error_to_string(result));
				return;
			}
		}

		{
			auto log_feature_level = [this](const std::string& level)
			{
				auto settings = m_context->GetSubsystem<Settings>();
				settings->RegisterThirdPartyLib("DirectX", level, "https://www.microsoft.com/en-us/download/details.aspx?id=17431");
				LOG_INFO("DirectX %s", level.c_str());
			};

			switch (m_rhi_context->device->GetFeatureLevel())
			{
			case D3D_FEATURE_LEVEL_9_1:
				log_feature_level("9.1");
				break;

			case D3D_FEATURE_LEVEL_9_2:
				log_feature_level("9.2");
				break;

			case D3D_FEATURE_LEVEL_9_3:
				log_feature_level("9.3");
				break;

			case D3D_FEATURE_LEVEL_10_0:
				log_feature_level("10.0");
				break;

			case D3D_FEATURE_LEVEL_10_1:
				log_feature_level("10.1");
				break;

			case D3D_FEATURE_LEVEL_11_0:
				log_feature_level("11.0");
				break;

			case D3D_FEATURE_LEVEL_11_1:
				log_feature_level("11.1");
				break;
			case D3D_FEATURE_LEVEL_12_0: break;
			case D3D_FEATURE_LEVEL_12_1: break;
			default:;
			}
		}

		if (multithread_protection)
		{
			ID3D11Multithread* multithread = nullptr;
			if (SUCCEEDED(m_rhi_context->device_context->QueryInterface(__uuidof(ID3D11Multithread), reinterpret_cast<void**>(&multithread))))
			{
				multithread->SetMultithreadProtected(TRUE);
				multithread->Release();
			}
			else
			{
				LOG_ERROR("Failed to enable multi-threaded protection");
			}
		}

		if (m_rhi_context->debug)
		{
			const auto result = m_rhi_context->device_context->QueryInterface(IID_PPV_ARGS(&m_rhi_context->annotation));
			if (FAILED(result))
			{
				LOG_ERROR("Failed to create ID3DUserDefinedAnnotation for event reporting, %s.", d3d11_utility::dxgi_error_to_string(result));
				return;
			}
		}

		m_initialized = true;
	}

	RHI_Device::~RHI_Device()
	{
		d3d11_utility::release(m_rhi_context->device_context);
		d3d11_utility::release(m_rhi_context->device);
		d3d11_utility::release(m_rhi_context->annotation);
	}

	bool RHI_Device::Queue_Submit(const RHI_Queue_Type type, void* cmd_buffer, void* wait_semaphore /*= nullptr*/, void* signal_semaphore /*= nullptr*/, void* wait_fence /*= nullptr*/, uint32_t wait_flags /*= 0*/) const
	{
		return true;
	}

	bool RHI_Device::Queue_Wait(const RHI_Queue_Type type) const
	{
		m_rhi_context->device_context->Flush();
		return true;
	}

#pragma endregion

#pragma region SwapChain

	RHI_SwapChain::RHI_SwapChain(
		void* window_handle,
		const shared_ptr<RHI_Device>& rhi_device,
		const uint32_t width,
		const uint32_t height,
		const RHI_Format format	    /*= Format_R8G8B8A8_UNORM*/,
		const uint32_t buffer_count	/*= 2 */,
		const uint32_t flags	    /*= Present_Immediate */
	)
	{
		// Validate device
		if (!rhi_device || !rhi_device->GetContextRhi()->device)
		{
			LOG_ERROR("Invalid device.");
			return;
		}

		// Validate window handle
		const auto hwnd = static_cast<HWND>(window_handle);
		if (!hwnd || !IsWindow(hwnd))
		{
			LOG_ERROR_INVALID_PARAMETER();
			return;
		}

		// Validate resolution
		if (!rhi_device->ValidateResolution(width, height))
		{
			LOG_WARNING("%dx%d is an invalid resolution", width, height);
			return;
		}

		// Get factory
		IDXGIFactory* dxgi_factory = nullptr;
		if (const auto& adapter = rhi_device->GetPrimaryPhysicalDevice())
		{
			auto dxgi_adapter = static_cast<IDXGIAdapter*>(adapter->GetData());
			if (dxgi_adapter->GetParent(IID_PPV_ARGS(&dxgi_factory)) != S_OK)
			{
				LOG_ERROR("Failed to get adapter's factory");
				return;
			}
		}
		else
		{
			LOG_ERROR("Invalid primary adapter");
			return;
		}

		// Save parameters
		m_format = format;
		m_rhi_device = rhi_device.get();
		m_buffer_count = buffer_count;
		m_windowed = true;
		m_width = width;
		m_height = height;
		m_flags = d3d11_utility::swap_chain::validate_flags(flags);

		// Create swap chain
		{
			DXGI_SWAP_CHAIN_DESC desc = {};
			desc.BufferCount = static_cast<UINT>(buffer_count);
			desc.BufferDesc.Width = static_cast<UINT>(width);
			desc.BufferDesc.Height = static_cast<UINT>(height);
			desc.BufferDesc.Format = d3d11_format[format];
			desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
			desc.OutputWindow = hwnd;
			desc.SampleDesc.Count = 1;
			desc.SampleDesc.Quality = 0;
			desc.Windowed = m_windowed ? TRUE : FALSE;
			desc.SwapEffect = d3d11_utility::swap_chain::get_swap_effect(m_flags);
			desc.Flags = d3d11_utility::swap_chain::get_flags(m_flags);

			if (!d3d11_utility::error_check(dxgi_factory->CreateSwapChain(m_rhi_device->GetContextRhi()->device, &desc, reinterpret_cast<IDXGISwapChain**>(&m_swap_chain_view))))
			{
				LOG_ERROR("Failed to create swapchain");
				return;
			}
		}

		// Create the render target
		if (auto swap_chain = static_cast<IDXGISwapChain*>(m_swap_chain_view))
		{
			ID3D11Texture2D* backbuffer = nullptr;
			auto result = swap_chain->GetBuffer(0, IID_PPV_ARGS(&backbuffer));
			if (FAILED(result))
			{
				LOG_ERROR("%s", d3d11_utility::dxgi_error_to_string(result));
				return;
			}

			auto render_target_view = static_cast<ID3D11RenderTargetView*>(m_resource_view_renderTarget);
			result = rhi_device->GetContextRhi()->device->CreateRenderTargetView(backbuffer, nullptr, &render_target_view);
			backbuffer->Release();
			if (FAILED(result))
			{
				LOG_ERROR("%s", d3d11_utility::dxgi_error_to_string(result));
				return;
			}
			m_resource_view_renderTarget = static_cast<void*>(render_target_view);
		}

		// Create command lists
		for (uint32_t i = 0; i < m_buffer_count; i++)
		{
			m_cmd_lists.emplace_back(make_shared<RHI_CommandList>(i, this, rhi_device->GetContext()));
		}

		m_initialized = true;
	}

	RHI_SwapChain::~RHI_SwapChain()
	{
		auto swap_chain = static_cast<IDXGISwapChain*>(m_swap_chain_view);

		// Before shutting down set to windowed mode to avoid swap chain exception
		if (swap_chain && !m_windowed)
		{
			swap_chain->SetFullscreenState(false, nullptr);
		}

		m_cmd_lists.clear();

		d3d11_utility::release(swap_chain);
		d3d11_utility::release(*reinterpret_cast<ID3D11RenderTargetView**>(&m_resource_view_renderTarget));
	}

	bool RHI_SwapChain::Resize(const uint32_t width, const uint32_t height, const bool force /*= false*/)
	{
		if (!m_swap_chain_view)
		{
			LOG_ERROR_INVALID_INTERNALS();
			return false;
		}

		// Validate resolution
		m_present = m_rhi_device->ValidateResolution(width, height);
		if (!m_present)
		{
			// Return true as when minimizing, a resolution
			// of 0,0 can be passed in, and this is fine.
			return true;
		}

		// Only resize if needed
		if (!force)
		{
			if (m_width == width && m_height == height)
				return true;
		}

		auto swap_chain = static_cast<IDXGISwapChain*>(m_swap_chain_view);
		auto render_target_view = static_cast<ID3D11RenderTargetView*>(m_resource_view_renderTarget);

		// Release previous stuff
		d3d11_utility::release(render_target_view);

		// Set this flag to enable an application to switch modes by calling IDXGISwapChain::ResizeTarget.
		// When switching from windowed to full-screen mode, the display mode (or monitor resolution)
		// will be changed to match the dimensions of the application window.
		if (m_flags & RHI_SwapChain_Allow_Mode_Switch)
		{
			const DisplayMode& display_mode = m_rhi_device->GetActiveDisplayMode();

			// Resize swapchain target
			DXGI_MODE_DESC dxgi_mode_desc = {};
			dxgi_mode_desc.Width = static_cast<UINT>(width);
			dxgi_mode_desc.Height = static_cast<UINT>(height);
			dxgi_mode_desc.Format = d3d11_format[m_format];
			dxgi_mode_desc.RefreshRate = DXGI_RATIONAL{ display_mode.numerator, display_mode.denominator };
			dxgi_mode_desc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
			dxgi_mode_desc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;

			// Resize swapchain target
			const auto result = swap_chain->ResizeTarget(&dxgi_mode_desc);
			if (FAILED(result))
			{
				LOG_ERROR("Failed to resize swapchain target, %s.", d3d11_utility::dxgi_error_to_string(result));
				return false;
			}
		}

		// Resize swapchain buffers
		const UINT d3d11_flags = d3d11_utility::swap_chain::get_flags(d3d11_utility::swap_chain::validate_flags(m_flags));
		auto result = swap_chain->ResizeBuffers(m_buffer_count, static_cast<UINT>(width), static_cast<UINT>(height), d3d11_format[m_format], d3d11_flags);
		if (FAILED(result))
		{
			LOG_ERROR("Failed to resize swapchain buffers, %s.", d3d11_utility::dxgi_error_to_string(result));
			return false;
		}

		// Get swapchain back-buffer
		ID3D11Texture2D* backbuffer = nullptr;
		result = swap_chain->GetBuffer(0, IID_PPV_ARGS(&backbuffer));
		if (FAILED(result))
		{
			LOG_ERROR("Failed to get swapchain buffer, %s.", d3d11_utility::dxgi_error_to_string(result));
			return false;
		}

		// Create render target view
		result = m_rhi_device->GetContextRhi()->device->CreateRenderTargetView(backbuffer, nullptr, &render_target_view);
		d3d11_utility::release(backbuffer);
		if (FAILED(result))
		{
			LOG_ERROR("Failed to create render target view, %s.", d3d11_utility::dxgi_error_to_string(result));
			return false;
		}
		m_resource_view_renderTarget = static_cast<void*>(render_target_view);

		return true;
	}

	bool RHI_SwapChain::AcquireNextImage()
	{
		return true;
	}

	bool RHI_SwapChain::Present()
	{
		if (!m_present)
			return true;

		if (!m_swap_chain_view)
		{
			LOG_ERROR("Can't present, swapchain failed to initialise");
			return false;
		}

		// Build flags
		const bool tearing_allowed = m_flags & RHI_Present_Immediate;
		const UINT sync_interval = tearing_allowed ? 0 : 1; // sync interval can go up to 4, so this could be improved
		const UINT flags = (tearing_allowed && m_windowed) ? DXGI_PRESENT_ALLOW_TEARING : 0;

		// Present
		auto ptr_swap_chain = static_cast<IDXGISwapChain*>(m_swap_chain_view);
		const auto result = ptr_swap_chain->Present(sync_interval, flags);
		if (FAILED(result))
		{
			LOG_ERROR("Failed to present, %s.", d3d11_utility::dxgi_error_to_string(result));
			return false;
		}

		return true;
	}


#pragma endregion


#pragma region Sampler

	void RHI_Sampler::CreateResource()
	{
		D3D11_SAMPLER_DESC sampler_desc = {};
		sampler_desc.Filter = d3d11_utility::sampler::get_filter(m_filter_min, m_filter_mag, m_filter_mipmap, m_anisotropy_enabled, m_comparison_enabled);
		sampler_desc.AddressU = d3d11_sampler_address_mode[m_sampler_address_mode];
		sampler_desc.AddressV = d3d11_sampler_address_mode[m_sampler_address_mode];
		sampler_desc.AddressW = d3d11_sampler_address_mode[m_sampler_address_mode];
		sampler_desc.MipLODBias = 0.0f;
		sampler_desc.MaxAnisotropy = 1;//m_rhi_device->GetContext()->GetSubsystem<Renderer>()->GetOptionValue<UINT>(Option_Value_Anisotropy);
		sampler_desc.ComparisonFunc = d3d11_comparison_function[m_comparison_function];
		sampler_desc.BorderColor[0] = 0;
		sampler_desc.BorderColor[1] = 0;
		sampler_desc.BorderColor[2] = 0;
		sampler_desc.BorderColor[3] = 0;
		sampler_desc.MinLOD = 0;
		sampler_desc.MaxLOD = FLT_MAX;

		// Create sampler state.
		if (FAILED(m_rhi_device->GetContextRhi()->device->CreateSamplerState(&sampler_desc, reinterpret_cast<ID3D11SamplerState**>(&m_resource))))
		{
			LOG_ERROR("Failed to create sampler state");
		}
	}

	RHI_Sampler::~RHI_Sampler()
	{
		d3d11_utility::release(*reinterpret_cast<ID3D11SamplerState**>(&m_resource));
	}

#pragma endregion

#pragma region Texture

	inline UINT GetBindFlags(uint16_t flags)
	{
		UINT flags_d3d11 = 0;

		flags_d3d11 |= (flags & RHI_Texture_ShaderView) ? D3D11_BIND_SHADER_RESOURCE : 0;
		flags_d3d11 |= (flags & RHI_Texture_UnorderedAccessView) ? D3D11_BIND_UNORDERED_ACCESS : 0;
		flags_d3d11 |= (flags & RHI_Texture_DepthStencilView) ? D3D11_BIND_DEPTH_STENCIL : 0;
		flags_d3d11 |= (flags & RHI_Texture_RenderTargetView) ? D3D11_BIND_RENDER_TARGET : 0;

		return flags_d3d11;
	}

	inline DXGI_FORMAT GetDepthFormat(RHI_Format format)
	{
		if (format == RHI_Format_D32_Float_S8X24_Uint)
		{
			return DXGI_FORMAT_R32G8X24_TYPELESS;
		}
		else if (format == RHI_Format_D32_Float)
		{
			return DXGI_FORMAT_R32_TYPELESS;
		}
		return d3d11_format[format];
	}

	inline DXGI_FORMAT GetDepthFormatDsv(RHI_Format format)
	{
		if (format == RHI_Format_D32_Float_S8X24_Uint)
		{
			return DXGI_FORMAT_D32_FLOAT_S8X24_UINT;
		}
		else if (format == RHI_Format_D32_Float)
		{
			return DXGI_FORMAT_D32_FLOAT;
		}

		return d3d11_format[format];
	}

	inline DXGI_FORMAT GetDepthFormatSrv(RHI_Format format)
	{
		if (format == RHI_Format_D32_Float_S8X24_Uint)
		{
			return DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS;
		}
		else if (format == RHI_Format_D32_Float)
		{
			return DXGI_FORMAT_R32_FLOAT;
		}

		return d3d11_format[format];
	}

	//Texture 2D
	inline bool CreateTexture2d(
		void*& texture,
		const uint32_t width,
		const uint32_t height,
		const uint32_t channels,
		const uint32_t bits_per_channel,
		const uint32_t array_size,
		const DXGI_FORMAT format,
		const UINT bind_flags,
		vector<vector<std::byte>>& data,
		const shared_ptr<RHI_Device>& rhi_device
	)
	{
		// Describe
		D3D11_TEXTURE2D_DESC texture_desc = {};
		texture_desc.Width = static_cast<UINT>(width);
		texture_desc.Height = static_cast<UINT>(height);
		texture_desc.MipLevels = data.empty() ? 1 : static_cast<UINT>(data.size());
		texture_desc.ArraySize = static_cast<UINT>(array_size);
		texture_desc.Format = format;
		texture_desc.SampleDesc.Count = 1;
		texture_desc.SampleDesc.Quality = 0;
		texture_desc.Usage = (bind_flags & D3D11_BIND_RENDER_TARGET) || (bind_flags & D3D11_BIND_DEPTH_STENCIL) ? D3D11_USAGE_DEFAULT : D3D11_USAGE_IMMUTABLE;
		texture_desc.BindFlags = bind_flags;
		texture_desc.MiscFlags = 0;
		texture_desc.CPUAccessFlags = 0;

		// Fill subresource data
		vector<D3D11_SUBRESOURCE_DATA> vec_subresource_data;
		for (uint32_t mip_level = 0; mip_level < static_cast<uint32_t>(data.size()); mip_level++)
		{
			if (data[mip_level].empty())
			{
				LOG_ERROR("Mipmap %d has invalid data.", mip_level);
				return false;
			}

			auto& subresource_data = vec_subresource_data.emplace_back(D3D11_SUBRESOURCE_DATA{});
			subresource_data.pSysMem = data[mip_level].data();					                // Data pointer		
			subresource_data.SysMemPitch = (width >> mip_level) * channels * (bits_per_channel / 8);	// Line width in bytes
			subresource_data.SysMemSlicePitch = 0;								                        // This is only used for 3D textures
		}

		// Create
		const auto result = rhi_device->GetContextRhi()->device->CreateTexture2D(&texture_desc, vec_subresource_data.data(), reinterpret_cast<ID3D11Texture2D**>(&texture));
		if (FAILED(result))
		{
			LOG_ERROR("Failed, %s.", d3d11_utility::dxgi_error_to_string(result));
			return false;
		}

		return true;
	}

	inline bool CreateRenderTargetView2d(void* texture, array<void*, state_max_render_target_count>& views, const DXGI_FORMAT format, const unsigned array_size, const shared_ptr<RHI_Device>& rhi_device)
	{
		// Describe
		D3D11_RENDER_TARGET_VIEW_DESC view_desc = {};
		view_desc.Format = format;
		view_desc.ViewDimension = (array_size == 1) ? D3D11_RTV_DIMENSION_TEXTURE2D : D3D11_RTV_DIMENSION_TEXTURE2DARRAY;
		view_desc.Texture2DArray.MipSlice = 0;
		view_desc.Texture2DArray.ArraySize = 1;
		view_desc.Texture2DArray.FirstArraySlice = 0;

		// Create
		for (uint32_t i = 0; i < array_size; i++)
		{
			view_desc.Texture2DArray.FirstArraySlice = i;
			const auto result = rhi_device->GetContextRhi()->device->CreateRenderTargetView(static_cast<ID3D11Resource*>(texture), &view_desc, reinterpret_cast<ID3D11RenderTargetView**>(&views[i]));
			if (FAILED(result))
			{
				LOG_ERROR("Failed, %s.", d3d11_utility::dxgi_error_to_string(result));
				return false;
			}
		}
		return true;
	}

	inline bool CreateDepthStencilView2d(void* texture, array<void*, state_max_render_target_count>& views, const uint32_t array_size, const DXGI_FORMAT format, bool read_only, const shared_ptr<RHI_Device>& rhi_device)
	{
		// Describe
		D3D11_DEPTH_STENCIL_VIEW_DESC dsv_desc = {};
		dsv_desc.Format = format;
		dsv_desc.ViewDimension = (array_size == 1) ? D3D11_DSV_DIMENSION_TEXTURE2D : D3D11_DSV_DIMENSION_TEXTURE2DARRAY;
		dsv_desc.Texture2DArray.MipSlice = 0;
		dsv_desc.Texture2DArray.ArraySize = 1;
		dsv_desc.Texture2DArray.FirstArraySlice = 0;
		dsv_desc.Flags = read_only ? D3D11_DSV_READ_ONLY_DEPTH | D3D11_DSV_READ_ONLY_STENCIL : 0;

		// Create
		for (uint32_t i = 0; i < array_size; i++)
		{
			dsv_desc.Texture2DArray.FirstArraySlice = i;
			const auto result = rhi_device->GetContextRhi()->device->CreateDepthStencilView(static_cast<ID3D11Resource*>(texture), &dsv_desc, reinterpret_cast<ID3D11DepthStencilView**>(&views[i]));
			if (FAILED(result))
			{
				LOG_ERROR("Failed, %s.", d3d11_utility::dxgi_error_to_string(result));
				return false;
			}
		}

		return true;
	}

	inline bool CreateShaderResourceView2d(void* texture, void*& view, DXGI_FORMAT format, uint32_t array_size, vector<vector<std::byte>>& data, const shared_ptr<RHI_Device>& rhi_device)
	{
		// Describe
		D3D11_SHADER_RESOURCE_VIEW_DESC shader_resource_view_desc = {};
		shader_resource_view_desc.Format = format;
		shader_resource_view_desc.ViewDimension = (array_size == 1) ? D3D11_SRV_DIMENSION_TEXTURE2D : D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
		shader_resource_view_desc.Texture2DArray.FirstArraySlice = 0;
		shader_resource_view_desc.Texture2DArray.MostDetailedMip = 0;
		shader_resource_view_desc.Texture2DArray.MipLevels = data.empty() ? 1 : static_cast<UINT>(data.size());
		shader_resource_view_desc.Texture2DArray.ArraySize = array_size;

		// Create
		auto result = rhi_device->GetContextRhi()->device->CreateShaderResourceView(static_cast<ID3D11Resource*>(texture), &shader_resource_view_desc, reinterpret_cast<ID3D11ShaderResourceView**>(&view));
		if (FAILED(result))
		{
			LOG_ERROR("Failed, %s.", d3d11_utility::dxgi_error_to_string(result));
			return false;
		}

		return true;
	}

	inline bool CreateUnorderedAccessView2d(void* texture, void*& view, DXGI_FORMAT format, const shared_ptr<RHI_Device>& rhi_device)
	{
		// Describe
		D3D11_UNORDERED_ACCESS_VIEW_DESC unorderd_access_view_desc = {};
		unorderd_access_view_desc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;
		unorderd_access_view_desc.Format = format;

		// Create
		const auto result = rhi_device->GetContextRhi()->device->CreateUnorderedAccessView(static_cast<ID3D11Resource*>(texture), &unorderd_access_view_desc, reinterpret_cast<ID3D11UnorderedAccessView**>(&view));
		if (FAILED(result))
		{
			LOG_ERROR("Failed, %s.", d3d11_utility::dxgi_error_to_string(result));
			return false;
		}

		return true;
	}

	RHI_Texture2D::~RHI_Texture2D()
	{
		d3d11_utility::release(*reinterpret_cast<ID3D11ShaderResourceView**>(&m_resource_view[0]));
		d3d11_utility::release(*reinterpret_cast<ID3D11UnorderedAccessView**>(&m_resource_view_unorderedAccess));
		d3d11_utility::release(*reinterpret_cast<ID3D11Texture2D**>(&m_resource));
		for (void*& render_target : m_resource_view_renderTarget)
		{
			d3d11_utility::release(*reinterpret_cast<ID3D11RenderTargetView**>(&render_target));
		}
		for (void*& depth_stencil : m_resource_view_depthStencil)
		{
			d3d11_utility::release(*reinterpret_cast<ID3D11DepthStencilView**>(&depth_stencil));
		}
		for (void*& depth_stencil : m_resource_view_depthStencilReadOnly)
		{
			d3d11_utility::release(*reinterpret_cast<ID3D11DepthStencilView**>(&depth_stencil));
		}
	}

	void RHI_Texture::SetLayout(const RHI_Image_Layout new_layout, RHI_CommandList* command_list /*= nullptr*/)
	{
		m_layout = new_layout;
	}

	bool RHI_Texture2D::CreateResourceGpu()
	{
		if (!m_rhi_device || !m_rhi_device->GetContextRhi()->device)
		{
			LOG_ERROR_INVALID_PARAMETER();
			return false;
		}

		bool result_tex = true;
		bool result_srv = true;
		bool result_uav = true;
		bool result_rt = true;
		bool result_ds = true;

		// Get texture flags
		const UINT flags = GetBindFlags(m_flags);

		// Resolve formats
		const DXGI_FORMAT format = GetDepthFormat(m_format);
		const DXGI_FORMAT format_dsv = GetDepthFormatDsv(m_format);
		const DXGI_FORMAT format_srv = GetDepthFormatSrv(m_format);

		// TEXTURE
		result_tex = CreateTexture2d
		(
			m_resource,
			m_width,
			m_height,
			m_channel_count,
			m_bits_per_channel,
			m_array_size,
			format,
			flags,
			m_data,
			m_rhi_device
		);

		// RESOURCE VIEW
		if (m_flags & RHI_Texture_ShaderView)
		{
			result_srv = CreateShaderResourceView2d(
				m_resource,
				m_resource_view[0],
				format_srv,
				m_array_size,
				m_data,
				m_rhi_device
			);
		}

		// UNORDERED ACCESS VIEW
		if (m_flags & RHI_Texture_UnorderedAccessView)
		{
			result_uav = CreateUnorderedAccessView2d(m_resource, m_resource_view_unorderedAccess, format, m_rhi_device);
		}

		// DEPTH-STENCIL VIEW
		if (m_flags & RHI_Texture_DepthStencilView)
		{
			result_ds = CreateDepthStencilView2d
			(
				m_resource,
				m_resource_view_depthStencil,
				m_array_size,
				format_dsv,
				false,
				m_rhi_device
			);

			if (m_flags & RHI_Texture_DepthStencilViewReadOnly)
			{
				result_ds = CreateDepthStencilView2d
				(
					m_resource,
					m_resource_view_depthStencilReadOnly,
					m_array_size,
					format_dsv,
					true,
					m_rhi_device
				);
			}
		}

		// RENDER TARGET VIEW
		if (m_flags & RHI_Texture_RenderTargetView)
		{
			result_rt = CreateRenderTargetView2d
			(
				m_resource,
				m_resource_view_renderTarget,
				format,
				m_array_size,
				m_rhi_device
			);
		}

		d3d11_utility::release(*reinterpret_cast<ID3D11Texture2D**>(&m_resource));

		return result_tex && result_srv && result_uav && result_rt && result_ds;
	}

	inline bool CreateTextureCube(
		void*& texture,
		const uint32_t width,
		const uint32_t height,
		uint32_t channels,
		uint32_t array_size,
		uint32_t bits_per_channel,
		DXGI_FORMAT format,
		const UINT flags,
		vector<vector<vector<std::byte>>>& data,
		const shared_ptr<RHI_Device>& rhi_device
	)
	{
		// Describe
		D3D11_TEXTURE2D_DESC texture_desc;
		texture_desc.Width = static_cast<UINT>(width);
		texture_desc.Height = static_cast<UINT>(height);
		texture_desc.MipLevels = static_cast<UINT>(data.empty() || data[0].empty() ? 1 : data[0].size());
		texture_desc.ArraySize = array_size;
		texture_desc.Format = format;
		texture_desc.SampleDesc.Count = 1;
		texture_desc.SampleDesc.Quality = 0;
		texture_desc.Usage = (flags & D3D11_BIND_RENDER_TARGET) || (flags & D3D11_BIND_DEPTH_STENCIL) ? D3D11_USAGE_DEFAULT : D3D11_USAGE_IMMUTABLE;
		texture_desc.BindFlags = flags;
		texture_desc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;
		texture_desc.CPUAccessFlags = 0;

		vector<D3D11_SUBRESOURCE_DATA> vec_subresource_data;
		vector<D3D11_TEXTURE2D_DESC> vec_texture_desc;

		if (!data.empty())
		{
			for (uint32_t face_index = 0; face_index < array_size; face_index++)
			{
				const auto& face_data = data[face_index];

				if (face_data.empty())
				{
					LOG_ERROR("A side contains no data");
					continue;
				}

				for (uint32_t mip_level = 0; mip_level < static_cast<uint32_t>(face_data.size()); mip_level++)
				{
					const auto& mip_data = face_data[mip_level];

					if (mip_data.empty())
					{
						LOG_ERROR("A mip-map contains invalid data.");
						continue;
					}

					// D3D11_SUBRESOURCE_DATA
					auto& subresource_data = vec_subresource_data.emplace_back(D3D11_SUBRESOURCE_DATA{});
					subresource_data.pSysMem = mip_data.data();                                          // Data pointer		
					subresource_data.SysMemPitch = (width >> mip_level) * channels * (bits_per_channel / 8);	// Line width in bytes
					subresource_data.SysMemSlicePitch = 0;			                                             // This is only used for 3D textures
				}

				vec_texture_desc.emplace_back(texture_desc);
			}
		}
		else
		{
			vec_texture_desc.emplace_back(texture_desc);
		}

		// Create
		const auto result = rhi_device->GetContextRhi()->device->CreateTexture2D(vec_texture_desc.data(), vec_subresource_data.data(), reinterpret_cast<ID3D11Texture2D**>(&texture));
		if (FAILED(result))
		{
			LOG_ERROR("Failed, %s.", d3d11_utility::dxgi_error_to_string(result));
			return false;
		}

		return true;
	}

	inline bool CreateShaderResourceViewCube(void* texture, void*& view, DXGI_FORMAT format, uint32_t array_size, vector<vector<vector<std::byte>>>& data, const shared_ptr<RHI_Device>& rhi_device)
	{
		// Describe
		D3D11_SHADER_RESOURCE_VIEW_DESC srv_desc = {};
		srv_desc.Format = format;
		srv_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
		srv_desc.TextureCube.MipLevels = static_cast<UINT>(data.empty() || data[0].empty() ? 1 : data[0].size());
		srv_desc.TextureCube.MostDetailedMip = 0;
		srv_desc.Texture2DArray.FirstArraySlice = 0;
		srv_desc.Texture2DArray.MostDetailedMip = 0;
		srv_desc.Texture2DArray.MipLevels = 1;
		srv_desc.Texture2DArray.ArraySize = array_size;

		// Create
		const auto result = rhi_device->GetContextRhi()->device->CreateShaderResourceView(static_cast<ID3D11Resource*>(texture), &srv_desc, reinterpret_cast<ID3D11ShaderResourceView**>(&view));
		if (FAILED(result))
		{
			LOG_ERROR("Failed, %s.", d3d11_utility::dxgi_error_to_string(result));
			return false;
		}
		return true;
	}

	inline bool CreateDepthStencilViewCube(void* texture, array<void*, state_max_render_target_count>& views, const uint32_t array_size, const DXGI_FORMAT format, bool read_only, const shared_ptr<RHI_Device>& rhi_device)
	{
		// DSV
		D3D11_DEPTH_STENCIL_VIEW_DESC dsv_desc = {};
		dsv_desc.Format = format;
		dsv_desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DARRAY;
		dsv_desc.Texture2DArray.MipSlice = 0;
		dsv_desc.Texture2DArray.ArraySize = 1;
		dsv_desc.Texture2DArray.FirstArraySlice = 0;

		// Create
		for (uint32_t i = 0; i < array_size; i++)
		{
			dsv_desc.Texture2DArray.FirstArraySlice = i;
			const auto result = rhi_device->GetContextRhi()->device->CreateDepthStencilView(static_cast<ID3D11Resource*>(texture), &dsv_desc, reinterpret_cast<ID3D11DepthStencilView**>(&views[i]));
			if (FAILED(result))
			{
				LOG_ERROR("Failed, %s.", d3d11_utility::dxgi_error_to_string(result));
				return false;
			}
		}

		return true;
	}

	inline bool CreateRenderTargetViewCube(void* texture, array<void*, state_max_render_target_count>& views, const DXGI_FORMAT format, const unsigned array_size, const shared_ptr<RHI_Device>& rhi_device)
	{
		// Describe
		D3D11_RENDER_TARGET_VIEW_DESC view_desc = {};
		view_desc.Format = format;
		view_desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DARRAY;
		view_desc.Texture2DArray.MipSlice = 0;
		view_desc.Texture2DArray.ArraySize = 1;
		view_desc.Texture2DArray.FirstArraySlice = 0;

		// Create
		for (uint32_t i = 0; i < array_size; i++)
		{
			view_desc.Texture2DArray.FirstArraySlice = i;
			const auto result = rhi_device->GetContextRhi()->device->CreateRenderTargetView(static_cast<ID3D11Resource*>(texture), &view_desc, reinterpret_cast<ID3D11RenderTargetView**>(&views[i]));
			if (FAILED(result))
			{
				LOG_ERROR("Failed, %s.", d3d11_utility::dxgi_error_to_string(result));
				return false;
			}
		}

		return true;
	}

	inline bool CreateUnorderedAccessViewCube(void* texture, void*& view, DXGI_FORMAT format, const shared_ptr<RHI_Device>& rhi_device)
	{
		// Describe
		D3D11_UNORDERED_ACCESS_VIEW_DESC unorderd_access_view_desc = {};
		unorderd_access_view_desc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2DARRAY;
		unorderd_access_view_desc.Format = format;

		// Create
		const auto result = rhi_device->GetContextRhi()->device->CreateUnorderedAccessView(static_cast<ID3D11Resource*>(texture), &unorderd_access_view_desc, reinterpret_cast<ID3D11UnorderedAccessView**>(&view));
		if (FAILED(result))
		{
			LOG_ERROR("Failed, %s.", d3d11_utility::dxgi_error_to_string(result));
			return false;
		}

		return true;
	}

	RHI_TextureCube::~RHI_TextureCube()
	{
		d3d11_utility::release(*reinterpret_cast<ID3D11ShaderResourceView**>(&m_resource_view));
		d3d11_utility::release(*reinterpret_cast<ID3D11UnorderedAccessView**>(&m_resource_view_unorderedAccess));
		d3d11_utility::release(*reinterpret_cast<ID3D11Texture2D**>(&m_resource));
		for (void*& render_target : m_resource_view_renderTarget)
		{
			d3d11_utility::release(*reinterpret_cast<ID3D11RenderTargetView**>(&render_target));
		}
		for (void*& depth_stencil : m_resource_view_depthStencil)
		{
			d3d11_utility::release(*reinterpret_cast<ID3D11DepthStencilView**>(&depth_stencil));
		}
		for (void*& depth_stencil : m_resource_view_depthStencilReadOnly)
		{
			d3d11_utility::release(*reinterpret_cast<ID3D11DepthStencilView**>(&depth_stencil));
		}
	}

	bool RHI_TextureCube::CreateResourceGpu()
	{
		if (!m_rhi_device || !m_rhi_device->GetContextRhi()->device)
		{
			LOG_ERROR_INVALID_PARAMETER();
			return false;
		}

		bool result_tex = true;
		bool result_srv = true;
		bool result_uav = true;
		bool result_rt = true;
		bool result_ds = true;

		// Get texture flags
		const UINT flags = GetBindFlags(m_flags);

		// Resolve formats
		const DXGI_FORMAT format = GetDepthFormat(m_format);
		const DXGI_FORMAT format_dsv = GetDepthFormatDsv(m_format);
		const DXGI_FORMAT format_srv = GetDepthFormatSrv(m_format);

		// TEXTURE
		result_tex = CreateTextureCube
		(
			m_resource,
			m_width,
			m_height,
			m_channel_count,
			m_bits_per_channel,
			m_array_size,
			format,
			flags,
			m_data_cube,
			m_rhi_device
		);

		// RESOURCE VIEW
		if (m_flags & RHI_Texture_ShaderView)
		{
			result_srv = CreateShaderResourceViewCube(
				m_resource,
				m_resource_view[0],
				format_srv,
				m_array_size,
				m_data_cube,
				m_rhi_device
			);
		}

		// UNORDERED ACCESS VIEW
		if (m_flags & RHI_Texture_UnorderedAccessView)
		{
			result_uav = CreateUnorderedAccessViewCube(m_resource, m_resource_view_unorderedAccess, format, m_rhi_device);
		}

		// DEPTH-STENCIL VIEW
		if (m_flags & RHI_Texture_DepthStencilView)
		{
			result_ds = CreateDepthStencilViewCube
			(
				m_resource,
				m_resource_view_depthStencil,
				m_array_size,
				format_dsv,
				false,
				m_rhi_device
			);

			if (m_flags & RHI_Texture_DepthStencilViewReadOnly)
			{
				result_ds = CreateDepthStencilViewCube
				(
					m_resource,
					m_resource_view_depthStencilReadOnly,
					m_array_size,
					format_dsv,
					true,
					m_rhi_device
				);
			}
		}

		// RENDER TARGET VIEW
		if (m_flags & RHI_Texture_RenderTargetView)
		{
			result_rt = CreateRenderTargetViewCube
			(
				m_resource,
				m_resource_view_renderTarget,
				format,
				m_array_size,
				m_rhi_device
			);
		}

		d3d11_utility::release(*reinterpret_cast<ID3D11Texture2D**>(&m_resource));

		return result_tex && result_srv && result_uav && result_rt && result_ds;
	}

#pragma endregion

#pragma region VertexBuffer

	void RHI_VertexBuffer::_destroy()
	{
		d3d11_utility::release(*reinterpret_cast<ID3D11Buffer**>(&m_buffer));
	}

	bool RHI_VertexBuffer::_create(const void* vertices)
	{
		if (!m_rhi_device || !m_rhi_device->GetContextRhi()->device_context)
		{
			LOG_ERROR_INVALID_INTERNALS();
			return false;
		}

		const bool is_dynamic = vertices == nullptr;

		// Destroy previous buffer
		_destroy();

		// fill in a buffer description.
		D3D11_BUFFER_DESC buffer_desc = {};
		buffer_desc.ByteWidth = static_cast<UINT>(m_size_gpu);
		buffer_desc.Usage = is_dynamic ? D3D11_USAGE_DYNAMIC : D3D11_USAGE_IMMUTABLE;
		buffer_desc.CPUAccessFlags = is_dynamic ? D3D11_CPU_ACCESS_WRITE : 0;
		buffer_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		buffer_desc.MiscFlags = 0;
		buffer_desc.StructureByteStride = 0;

		// fill in the subresource data.
		D3D11_SUBRESOURCE_DATA init_data = {};
		init_data.pSysMem = vertices;
		init_data.SysMemPitch = 0;
		init_data.SysMemSlicePitch = 0;

		const auto ptr = reinterpret_cast<ID3D11Buffer**>(&m_buffer);
		const auto result = m_rhi_device->GetContextRhi()->device->CreateBuffer(&buffer_desc, is_dynamic ? nullptr : &init_data, ptr);
		if (FAILED(result))
		{
			LOG_ERROR("Failed to create vertex buffer");
			return false;
		}

		return true;
	}

	void* RHI_VertexBuffer::Map()
	{
		if (!m_rhi_device || !m_rhi_device->GetContextRhi()->device_context || !m_buffer)
		{
			LOG_ERROR_INVALID_INTERNALS();
			return nullptr;
		}

		// Disable GPU access to the vertex buffer data.
		D3D11_MAPPED_SUBRESOURCE mapped_resource;
		const auto result = m_rhi_device->GetContextRhi()->device_context->Map(static_cast<ID3D11Resource*>(m_buffer), 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_resource);
		if (FAILED(result))
		{
			LOG_ERROR("Failed to map vertex buffer");
			return nullptr;
		}

		return mapped_resource.pData;
	}

	bool RHI_VertexBuffer::Unmap()
	{
		if (!m_rhi_device || !m_rhi_device->GetContextRhi()->device_context || !m_buffer)
		{
			LOG_ERROR_INVALID_INTERNALS();
			return false;
		}

		// Re-enable GPU access to the vertex buffer data.
		m_rhi_device->GetContextRhi()->device_context->Unmap(static_cast<ID3D11Resource*>(m_buffer), 0);
		return true;
	}

#pragma endregion

#pragma region IndexBuffer

	void RHI_IndexBuffer::_destroy()
	{
		d3d11_utility::release(*reinterpret_cast<ID3D11Buffer**>(&m_buffer));
		return;
	}

	bool RHI_IndexBuffer::_create(const void* indices)
	{
		if (!m_rhi_device || !m_rhi_device->GetContextRhi()->device)
		{
			LOG_ERROR_INVALID_INTERNALS();
			return false;
		}

		const bool is_dynamic = indices == nullptr;

		// Destroy previous buffer
		_destroy();

		D3D11_BUFFER_DESC buffer_desc;
		ZeroMemory(&buffer_desc, sizeof(buffer_desc));
		buffer_desc.ByteWidth = m_stride * m_index_count;
		buffer_desc.Usage = is_dynamic ? D3D11_USAGE_DYNAMIC : D3D11_USAGE_IMMUTABLE;
		buffer_desc.CPUAccessFlags = is_dynamic ? D3D11_CPU_ACCESS_WRITE : 0;
		buffer_desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		buffer_desc.MiscFlags = 0;
		buffer_desc.StructureByteStride = 0;

		D3D11_SUBRESOURCE_DATA init_data;
		init_data.pSysMem = indices;
		init_data.SysMemPitch = 0;
		init_data.SysMemSlicePitch = 0;

		const auto ptr = reinterpret_cast<ID3D11Buffer**>(&m_buffer);
		const auto result = m_rhi_device->GetContextRhi()->device->CreateBuffer(&buffer_desc, is_dynamic ? nullptr : &init_data, ptr);
		if FAILED(result)
		{
			LOG_ERROR(" Failed to create index buffer");
			return false;
		}

		return true;
	}

	void* RHI_IndexBuffer::Map()
	{
		if (!m_rhi_device || !m_rhi_device->GetContextRhi()->device_context || !m_buffer)
		{
			LOG_ERROR_INVALID_INTERNALS();
			return nullptr;
		}

		D3D11_MAPPED_SUBRESOURCE mapped_resource;
		const auto result = m_rhi_device->GetContextRhi()->device_context->Map(static_cast<ID3D11Resource*>(m_buffer), 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_resource);
		if (FAILED(result))
		{
			LOG_ERROR("Failed to map index buffer.");
			return nullptr;
		}

		return mapped_resource.pData;
	}

	bool RHI_IndexBuffer::Unmap()
	{
		if (!m_rhi_device || !m_rhi_device->GetContextRhi()->device_context || !m_buffer)
		{
			LOG_ERROR_INVALID_INTERNALS();
			return false;
		}

		m_rhi_device->GetContextRhi()->device_context->Unmap(static_cast<ID3D11Resource*>(m_buffer), 0);
		return true;
	}

#pragma endregion



}
//===================================================================================================
