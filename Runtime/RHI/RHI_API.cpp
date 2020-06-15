#include "RHI_API.h"

namespace V
{

#pragma region Device

	void RHI_Device::RegisterPhysicalDevice(const PhysicalDevice& physical_device)
	{
		m_physical_devices.emplace_back(physical_device);

		std::sort(m_physical_devices.begin(), m_physical_devices.end(), [](const PhysicalDevice& adapter1, const PhysicalDevice& adapter2)
		{
			return adapter1.GetMemory > adapter2.GetMemory();
		});

		LOG_INFO("%s (%d MB)", physical_device.GetName().c_str(), physical_device.GetMemory());
	}

	const PhysicalDevice* RHI_Device::GetPrimaryPhysicalDevice()
	{
		if (m_physical_device_index >= m_physical_devices.size())
			return nullptr;

		return &m_physical_devices[m_physical_device_index];
	}

	void RHI_Device::SetPrimaryPhysicalDevice(const uint32_t index)
	{
		m_physical_device_index = index;

		if (const PhysicalDevice* physical_device = GetPrimaryPhysicalDevice())
		{
			LOG_INFO("%s (%d MB)", physical_device->GetName().c_str(), physical_device->GetMemory());
		}
	}

    void RHI_Device::RegisterDisplayMode(const DisplayMode& display_mode)
    {
        // Early exit if display is already registered
        for (const DisplayMode& display_mode_existing : m_display_modes)
        {
            if (display_mode == display_mode_existing)
                return;
        }

        DisplayMode& mode = m_display_modes.emplace_back(display_mode);

        sort(m_display_modes.begin(), m_display_modes.end(), [](const DisplayMode& display_mode_a, const DisplayMode& display_mode_b)
            {
                return display_mode_a.hz > display_mode_b.hz;
            });

        // Find preferred display mode
        for (uint32_t i = 0; i < static_cast<uint32_t>(m_display_modes.size()); i++)
        {
            const DisplayMode& display_mode = m_display_modes[i];

            // Try to use higher resolution
            if (display_mode.width > m_display_mode_active.width || display_mode.height > m_display_mode_active.height)
            {
                // But not lower hz
                if (display_mode.hz >= m_display_mode_active.hz)
                {
                    m_display_mode_active.width = display_mode.width;
                    m_display_mode_active.height = display_mode.height;
                    m_display_mode_active.hz = display_mode.hz;
                    m_display_mode_active.numerator = display_mode.numerator;
                    m_display_mode_active.denominator = display_mode.denominator;
                }
            }
        }

        // Let the timer know about the refresh rates this monitor is capable of (will result in low latency/smooth ticking)
        m_context->GetSubsystem<Timer>()->SetTargetFps(m_display_modes.front().hz);
    }

    bool RHI_Device::ValidateResolution(const uint32_t width, const uint32_t height) const
    {
        if (!m_rhi_context)
            return false;

        return  width > 0 && width <= m_rhi_context->max_texture_dimension_2d &&
            height > 0 && height <= m_rhi_context->max_texture_dimension_2d;
    }

    bool RHI_Device::Queue_WaitAll() const
    {
        return Queue_Wait(RHI_Queue_Graphics) && Queue_Wait(RHI_Queue_Transfer) && Queue_Wait(RHI_Queue_Compute);
    }

    void* RHI_Device::Queue_Get(const RHI_Queue_Type type) const
    {
        if (type == RHI_Queue_Graphics)
        {
            return m_rhi_context->queue_graphics;
        }
        else if (type == RHI_Queue_Transfer)
        {
            return m_rhi_context->queue_transfer;
        }
        else if (type == RHI_Queue_Compute)
        {
            return m_rhi_context->queue_compute;
        }
        return nullptr;
    }

    uint32_t RHI_Device::Queue_Index(const RHI_Queue_Type type) const
    {
        if (type == RHI_Queue_Graphics)
        {
            return m_rhi_context->queue_graphics_index;
        }
        else if (type == RHI_Queue_Transfer)
        {
            return m_rhi_context->queue_transfer_index;
        }
        else if (type == RHI_Queue_Compute)
        {
            return m_rhi_context->queue_compute_index;
        }

        return 0;
    }

#pragma endregion

#pragma region Viewport
    
    const RHI_Viewport RHI_Viewport::Undefined;

#pragma endregion

#pragma region Sampler

    RHI_Sampler::RHI_Sampler(
        const std::shared_ptr<RHI_Device>& rhi_device,
        const RHI_Filter filter_min                         /* = RHI_Filter_Nearest */,
        const RHI_Filter filter_mag                         /* = RHI_Filter_Nearest */,
        const RHI_Sampler_Mipmap_Mode filter_mipmap         /* = RHI_Sampler_Mipmap_Nearest */,
        const RHI_Sampler_Address_Mode sampler_address_mode /* = RHI_Sampler_Address_Wrap */,
        const RHI_Comparison_Function comparison_function   /* = RHI_Comparison_Always */,
        const bool anisotropy_enabled                       /* = false */,
        const bool comparison_enabled                       /* = false */
    )
    {
        if (!rhi_device || !rhi_device->GetContextRhi()->device)
        {
            LOG_ERROR_INVALID_PARAMETER();
            return;
        }

        m_resource = nullptr;
        m_rhi_device = rhi_device;
        m_filter_min = filter_min;
        m_filter_mag = filter_mag;
        m_filter_mipmap = filter_mipmap;
        m_sampler_address_mode = sampler_address_mode;
        m_comparison_function = comparison_function;
        m_anisotropy_enabled = anisotropy_enabled;
        m_comparison_enabled = comparison_enabled;

        CreateResource();
    }

#pragma endregion

#pragma region Texture

    RHI_Texture::RHI_Texture(Context* context) : IResource(context, Resource_Texture)
    {
        //m_rhi_device = context->GetSubsystem<Renderer>()->GetRhiDevice();

    }

    RHI_Texture::~RHI_Texture()
    {
        m_data.clear();
        m_data.shrink_to_fit();
    }

    bool RHI_Texture::SaveToFile(const string& file_path)
    {
        // Check to see if the file already exists (if so, get the byte count)
        uint32_t byte_count = 0;
        {
            if (FileSystem::Exists(file_path))
            {
                auto file = make_unique<FileStream>(file_path, FileStream_Read);
                if (file->IsOpen())
                {
                    file->Read(&byte_count);
                }
            }
        }

        auto append = true;
        auto file = make_unique<FileStream>(file_path, FileStream_Write | FileStream_Append);
        if (!file->IsOpen())
            return false;

        // If the existing file has a byte count but we 
        // hold no data, don't overwrite the file's bytes.
        if (byte_count != 0 && m_data.empty())
        {
            file->Skip
            (
                sizeof(uint32_t) +	// byte count
                sizeof(uint32_t) +	// mipmap count
                byte_count			// bytes
            );
        }
        else
        {
            byte_count = GetByteCount();

            // Write byte count
            file->Write(byte_count);
            // Write mipmap count
            file->Write(static_cast<uint32_t>(m_data.size()));
            // Write bytes
            for (auto& mip : m_data)
            {
                file->Write(mip);
            }

            // The bytes have been saved, so we can now free some memory
            m_data.clear();
            m_data.shrink_to_fit();
        }

        // Write properties
        file->Write(m_bits_per_channel);
        file->Write(m_width);
        file->Write(m_height);
        file->Write(static_cast<uint32_t>(m_format));
        file->Write(m_channel_count);
        file->Write(m_flags);
        file->Write(GetId());
        file->Write(GetResourceFilePath());

        return true;
    }

    bool RHI_Texture::LoadFromFile(const string& path)
    {
        // Validate file path
        if (!FileSystem::IsFile(path))
        {
            LOG_ERROR("\"%s\" is not a valid file path.", path.c_str());
            return false;
        }

        m_data.clear();
        m_data.shrink_to_fit();
        m_load_state = LoadState_Started;

        // Load from disk
        auto texture_data_loaded = false;
        if (FileSystem::IsEngineTextureFile(path)) // engine format (binary)
        {
            texture_data_loaded = LoadFromFile_NativeFormat(path);
        }
        else if (FileSystem::IsSupportedImageFile(path)) // foreign format (most known image formats)
        {
            texture_data_loaded = LoadFromFile_ForeignFormat(path, m_flags & RHI_Texture_GenerateMipsWhenLoading);
        }

        // Ensure that we have the data
        if (!texture_data_loaded)
        {
            LOG_ERROR("Failed to load \"%s\".", path.c_str());
            m_load_state = LoadState_Failed;
            return false;
        }

        m_mip_levels = static_cast<uint32_t>(m_data.size());

        // Create GPU resource
        if (!m_context->GetSubsystem<Renderer>()->GetRhiDevice()->IsInitialized() || !CreateResourceGpu())
        {
            LOG_ERROR("Failed to create shader resource for \"%s\".", GetResourceFilePathNative().c_str());
            m_load_state = LoadState_Failed;
            return false;
        }

        // Only clear texture bytes if that's an engine texture, if not, it's not serialized yet.
        if (FileSystem::IsEngineTextureFile(path))
        {
            m_data.clear();
            m_data.shrink_to_fit();
        }
        m_load_state = LoadState_Completed;

        // Compute memory usage
        {
            m_size_cpu = 0;
            m_size_gpu = 0;
            for (uint8_t mip_index = 0; mip_index < m_mip_levels; mip_index++)
            {
                const uint32_t mip_width = m_width >> mip_index;
                const uint32_t mip_height = m_height >> mip_index;

                m_size_cpu += mip_index < m_data.size() ? m_data[mip_index].size() * sizeof(std::byte) : 0;
                m_size_gpu += mip_width * mip_height * (m_bits_per_channel / 8);
            }
        }

        return true;
    }

    vector<std::byte>* RHI_Texture::GetData(const uint32_t index)
    {
        if (index >= m_data.size())
        {
            LOG_WARNING("Index out of range");
            return nullptr;
        }

        return &m_data[index];
    }

    vector<std::byte> RHI_Texture::GetMipmap(const uint32_t index)
    {
        vector<std::byte> data;

        // Use existing data, if it's there
        if (index < m_data.size())
        {
            data = m_data[index];
        }
        // Else attempt to load the data
        else
        {
            auto file = make_unique<FileStream>(GetResourceFilePathNative(), FileStream_Read);
            if (file->IsOpen())
            {
                auto byte_count = file->ReadAs<uint32_t>();
                const auto mip_count = file->ReadAs<uint32_t>();

                if (index < mip_count)
                {
                    for (uint32_t i = 0; i <= index; i++)
                    {
                        file->Read(&data);
                    }
                }
                else
                {
                    LOG_ERROR("Invalid index");
                }
                file->Close();
            }
            else
            {
                LOG_ERROR("Unable to retreive data");
            }
        }

        return data;
    }

    bool RHI_Texture::LoadFromFile_ForeignFormat(const string& file_path, const bool generate_mipmaps)
    {
        // Load texture
        ImageImporter* importer = m_context->GetSubsystem<ResourceCache>()->GetImageImporter();
        if (!importer->Load(file_path, this, generate_mipmaps))
            return false;

        // Set resource file path so it can be used by the resource cache
        SetResourceFilePath(file_path);

        return true;
    }

    bool RHI_Texture::LoadFromFile_NativeFormat(const string& file_path)
    {
        auto file = make_unique<FileStream>(file_path, FileStream_Read);
        if (!file->IsOpen())
            return false;

        m_data.clear();
        m_data.shrink_to_fit();

        // Read byte and mipmap count
        auto byte_count = file->ReadAs<uint32_t>();
        const auto mip_count = file->ReadAs<uint32_t>();

        // Read bytes
        m_data.resize(mip_count);
        for (auto& mip : m_data)
        {
            file->Read(&mip);
        }

        // Read properties
        file->Read(&m_bits_per_channel);
        file->Read(&m_width);
        file->Read(&m_height);
        file->Read(reinterpret_cast<uint32_t*>(&m_format));
        file->Read(&m_channel_count);
        file->Read(&m_flags);
        SetId(file->ReadAs<uint32_t>());
        SetResourceFilePath(file->ReadAs<string>());

        return true;
    }

    uint32_t RHI_Texture::GetChannelCountFromFormat(const RHI_Format format)
    {
        switch (format)
        {
            case RHI_Format_R8_Unorm:			    return 1;
            case RHI_Format_R16_Uint:			    return 1;
            case RHI_Format_R16_Float:			    return 1;
            case RHI_Format_R32_Uint:			    return 1;
            case RHI_Format_R32_Float:			    return 1;
            case RHI_Format_R8G8_Unorm:			    return 2;
            case RHI_Format_R16G16_Float:		    return 2;
            case RHI_Format_R32G32_Float:		    return 2;
            case RHI_Format_R32G32B32_Float:	    return 3;
            case RHI_Format_R8G8B8A8_Unorm:		    return 4;
            case RHI_Format_R16G16B16A16_Float:	    return 4;
            case RHI_Format_R32G32B32A32_Float:	    return 4;
            case RHI_Format_D32_Float:			    return 1;
            case RHI_Format_D32_Float_S8X24_Uint:   return 2;
            default:						        return 0;
        }
    }

    uint32_t RHI_Texture::GetByteCount()
    {
        uint32_t byte_count = 0;

        for (auto& mip : m_data)
        {
            byte_count += static_cast<uint32_t>(mip.size());
        }

        return byte_count;
    }


#pragma endregion




}