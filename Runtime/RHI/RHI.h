#pragma once

#include <stdint.h>
#include <string>
#include <vector>
#include <memory>
#include <cstddef>
#include <array>

#include "../Core/EngineDefs.h"
#include "../Core/VObject.h"

#include "../Resources/IResources.h"

#include "../Math/Vector4.h"

namespace V
{

	class Context;

#pragma region Definition

	struct RHI_Context;
	class RHI_Device;
	class RHI_CommandList;
	class RHI_PipelineState;
	class RHI_PipelineCache;
	class RHI_Pipeline;
	class RHI_DescriptorSetLayout;
	class RHI_DescriptorCache;
	class RHI_SwapChain;
	class RHI_RasterizerState;
	class RHI_BlendState;
	class RHI_DepthStencilState;
	class RHI_InputLayout;
	class RHI_VertexBuffer;
	class RHI_IndexBuffer;
	class RHI_ConstantBuffer;
	class RHI_Sampler;
	class RHI_Viewport;
	class RHI_Texture;
	class RHI_Texture2D;
	class RHI_TextureCube;
	class RHI_Shader;
	struct RHI_Vertex_Undefined;
	struct RHI_Vertex_PosTex;
	struct RHI_Vertex_PosCol;
	struct RHI_Vertex_PosUvCol;
	struct RHI_Vertex_PosTexNorTan;

	enum RHI_PhysicalDevice_Type
	{
		RHI_PhysicalDevice_Unknown,
		RHI_PhysicalDevice_Integrated,
		RHI_PhysicalDevice_Discrete,
		RHI_PhysicalDevice_Virtual,
		RHI_PhysicalDevice_Cpu
	};

	enum RHI_Api_Type
	{
		RHI_Api_D3d11,
		RHI_Api_D3d12,
		RHI_Api_Vulkan
	};

	enum RHI_Present_Mode : uint32_t
	{
		RHI_Present_Immediate = 1 << 0, // Doesn't wait.                  Frames are not dropped. Tearing.
		RHI_Present_Mailbox = 1 << 1, // Waits for v-blank.             Frames are dropped.     No tearing.
		RHI_Present_Fifo = 1 << 2, // Waits for v-blank, every time. Frames are not dropped. No tearing.
		RHI_Present_FifoRelaxed = 1 << 3, // Waits for v-blank, once.       Frames are not dropped. Tearing.
		RHI_Present_SharedDemandRefresh = 1 << 4,
		RHI_Present_SharedDContinuousRefresh = 1 << 5,

		// D3D11 only flags as match to Vulkan is possible
		RHI_Swap_Discard = 1 << 6,
		RHI_Swap_Sequential = 1 << 7,
		RHI_Swap_Flip_Sequential = 1 << 8,
		RHI_Swap_Flip_Discard = 1 << 9,
		RHI_SwapChain_Allow_Mode_Switch = 1 << 10
	};

	enum RHI_Queue_Type
	{
		RHI_Queue_Graphics,
		RHI_Queue_Transfer,
		RHI_Queue_Compute,
		RHI_Queue_Undefined
	};

	enum RHI_Query_Type
	{
		RHI_Query_Timestamp,
		RHI_Query_Timestamp_Disjoint
	};

	enum RHI_PrimitiveTopology_Mode
	{
		RHI_PrimitiveTopology_TriangleList,
		RHI_PrimitiveTopology_LineList,
		RHI_PrimitiveTopology_Unknown
	};

	enum RHI_Cull_Mode
	{
		RHI_Cull_None,
		RHI_Cull_Front,
		RHI_Cull_Back,
		RHI_Cull_Undefined
	};

	enum RHI_Fill_Mode
	{
		RHI_Fill_Solid,
		RHI_Fill_Wireframe,
		RHI_Fill_Undefined
	};

	enum RHI_Filter
	{
		RHI_Filter_Nearest,
		RHI_Filter_Linear,
	};

	enum RHI_Sampler_Mipmap_Mode
	{
		RHI_Sampler_Mipmap_Nearest,
		RHI_Sampler_Mipmap_Linear,
	};

	enum RHI_Sampler_Address_Mode
	{
		RHI_Sampler_Address_Wrap,
		RHI_Sampler_Address_Mirror,
		RHI_Sampler_Address_Clamp,
		RHI_Sampler_Address_Border,
		RHI_Sampler_Address_MirrorOnce,
	};

	enum RHI_Comparison_Function
	{
		RHI_Comparison_Never,
		RHI_Comparison_Less,
		RHI_Comparison_Equal,
		RHI_Comparison_LessEqual,
		RHI_Comparison_Greater,
		RHI_Comparison_NotEqual,
		RHI_Comparison_GreaterEqual,
		RHI_Comparison_Always
	};

	enum RHI_Stencil_Operation
	{
		RHI_Stencil_Keep,
		RHI_Stencil_Zero,
		RHI_Stencil_Replace,
		RHI_Stencil_Incr_Sat,
		RHI_Stencil_Decr_Sat,
		RHI_Stencil_Invert,
		RHI_Stencil_Incr,
		RHI_Stencil_Decr
	};

	enum RHI_Format : uint32_t // gets serialized so better be explicit
	{
		// R
		RHI_Format_R8_Unorm,
		RHI_Format_R16_Uint,
		RHI_Format_R16_Float,
		RHI_Format_R32_Uint,
		RHI_Format_R32_Float,
		// RG
		RHI_Format_R8G8_Unorm,
		RHI_Format_R16G16_Float,
		RHI_Format_R32G32_Float,
		// RGB
		RHI_Format_R11G11B10_Float,
		RHI_Format_R32G32B32_Float,
		// RGBA
		RHI_Format_R8G8B8A8_Unorm,
		RHI_Format_R10G10B10A2_Unorm,
		RHI_Format_R16G16B16A16_Float,
		RHI_Format_R32G32B32A32_Float,
		// DEPTH
		RHI_Format_D32_Float,
		RHI_Format_D32_Float_S8X24_Uint,

		RHI_Format_Undefined
	};

	enum RHI_Blend
	{
		RHI_Blend_Zero,
		RHI_Blend_One,
		RHI_Blend_Src_Color,
		RHI_Blend_Inv_Src_Color,
		RHI_Blend_Src_Alpha,
		RHI_Blend_Inv_Src_Alpha,
		RHI_Blend_Dest_Alpha,
		RHI_Blend_Inv_Dest_Alpha,
		RHI_Blend_Dest_Color,
		RHI_Blend_Inv_Dest_Color,
		RHI_Blend_Src_Alpha_Sat,
		RHI_Blend_Blend_Factor,
		RHI_Blend_Inv_Blend_Factor,
		RHI_Blend_Src1_Color,
		RHI_Blend_Inv_Src1_Color,
		RHI_Blend_Src1_Alpha,
		RHI_Blend_Inv_Src1_Alpha
	};

	enum RHI_Blend_Operation
	{
		RHI_Blend_Operation_Add,
		RHI_Blend_Operation_Subtract,
		RHI_Blend_Operation_Rev_Subtract,
		RHI_Blend_Operation_Min,
		RHI_Blend_Operation_Max
	};

	enum RHI_Descriptor_Type
	{
		RHI_Descriptor_Sampler,
		RHI_Descriptor_Texture,
		RHI_Descriptor_ConstantBuffer,
		RHI_Descriptor_ConstantBufferDynamic,
		RHI_Descriptor_Undefined
	};

	enum RHI_Image_Layout
	{
		RHI_Image_Undefined,
		RHI_Image_General,
		RHI_Image_Preinitialized,
		RHI_Image_Color_Attachment_Optimal,
		RHI_Image_Depth_Stencil_Attachment_Optimal,
		RHI_Image_Depth_Stencil_Read_Only_Optimal,
		RHI_Image_Shader_Read_Only_Optimal,
		RHI_Image_Transfer_Dst_Optimal,
		RHI_Image_Present_Src
	};

	struct RHI_Descriptor
	{
		RHI_Descriptor() = default;

		RHI_Descriptor(const RHI_Descriptor& descriptor)
		{
			type = descriptor.type;
			slot = descriptor.slot;
			stage = descriptor.stage;
		}

		RHI_Descriptor(RHI_Descriptor_Type type, uint32_t slot, uint32_t stage)
		{
			this->type = type;
			this->slot = slot;
			this->stage = stage;
		}

		uint32_t slot = 0;
		uint32_t stage = 0;
		uint64_t offset = 0;
		uint64_t range = 0;
		RHI_Descriptor_Type type = RHI_Descriptor_Undefined;
		RHI_Image_Layout layout = RHI_Image_Undefined;
		void* resource = nullptr;
	};

	inline const char* rhi_format_to_string(const RHI_Format result)
	{
		switch (result)
		{
		case RHI_Format_R8_Unorm:               return "RHI_Format_R8_Unorm";
		case RHI_Format_R16_Uint:			    return "RHI_Format_R16_Uint";
		case RHI_Format_R16_Float:			    return "RHI_Format_R16_Float";
		case RHI_Format_R32_Uint:			    return "RHI_Format_R32_Uint";
		case RHI_Format_R32_Float:			    return "RHI_Format_R32_Float";
		case RHI_Format_R8G8_Unorm:			    return "RHI_Format_R8G8_Unorm";
		case RHI_Format_R16G16_Float:		    return "RHI_Format_R16G16_Float";
		case RHI_Format_R32G32_Float:		    return "RHI_Format_R32G32_Float";
		case RHI_Format_R32G32B32_Float:	    return "RHI_Format_R32G32B32_Float";
		case RHI_Format_R8G8B8A8_Unorm:		    return "RHI_Format_R8G8B8A8_Unorm";
		case RHI_Format_R16G16B16A16_Float:	    return "RHI_Format_R16G16B16A16_Float";
		case RHI_Format_R32G32B32A32_Float:	    return "RHI_Format_R32G32B32A32_Float";
		case RHI_Format_D32_Float:	            return "RHI_Format_D32_Float";
		case RHI_Format_D32_Float_S8X24_Uint:	return "RHI_Format_D32_Float_S8X24_Uint";
		case RHI_Format_Undefined:              return "RHI_Format_Undefined";
		}

		return "Unknown format";
	}

	// Engine constants 
	static const Math::Vector4  state_color_dont_care = Math::Vector4(-std::numeric_limits<float>::infinity(), 0.0f, 0.0f, 0.0f);
	static const Math::Vector4  state_color_load = Math::Vector4(std::numeric_limits<float>::infinity(), 0.0f, 0.0f, 0.0f);
	static const float          state_depth_dont_care = -std::numeric_limits<float>::infinity();
	static const float          state_depth_load = std::numeric_limits<float>::infinity();
	static const uint32_t       state_stencil_dont_care = (std::numeric_limits<uint32_t>::max)();
	static const uint32_t       state_stencil_load = (std::numeric_limits<uint32_t>::max)() - 1;
	static const uint8_t        state_max_render_target_count = 8;
	static const uint8_t        state_max_constant_buffer_count = 8;
	static const uint32_t       state_dynamic_offset_empty = (std::numeric_limits<uint32_t>::max)();

	enum RHI_Shader_Type : uint8_t
	{
		RHI_Shader_Unknown = 0,
		RHI_Shader_Vertex = 1 << 0,
		RHI_Shader_Pixel = 1 << 1,
		RHI_Shader_Compute = 1 << 2,
	};

	enum Shader_Compilation_State
	{
		Shader_Compilation_Unknown,
		Shader_Compilation_Compiling,
		Shader_Compilation_Succeeded,
		Shader_Compilation_Failed
	};

#pragma endregion

#pragma region Device

	struct DisplayMode
	{
		DisplayMode() = default;
		DisplayMode(const uint32_t width, const uint32_t height, const uint32_t refresh_rate_numerator, const uint32_t refresh_rate_denominator)
		{
			this->width = width;
			this->height = height;
			this->refreshRateNumerator = refresh_rate_numerator;
			this->refreshRateDenominator = refresh_rate_denominator;
			this->refreshRate = static_cast<float>(refresh_rate_numerator) / static_cast<float>(refresh_rate_denominator);
		}
		uint32_t width = 0;
		uint32_t height = 0;
		uint32_t refreshRateNumerator = 0;
		uint32_t refreshRateDenominator = 0;
		float refreshRate = 0;
	};

	struct DisplayAdapter
	{
		DisplayAdapter(const std::string& name, const uint32_t memory, const uint32_t vendor_id, void* data)
		{
			this->name = name;
			this->memory = memory;
			this->vendorID = vendor_id;
			this->data = data;
		}

		bool IsNvidia() const { return vendorID == 0x10DE || name.find("NVIDIA") != std::string::npos; }
		bool IsAmd() const { return vendorID == 0x1002 || vendorID == 0x1022 || name.find("AMD") != std::string::npos; }
		bool IsIntel() const { return vendorID == 0x163C || vendorID == 0x8086 || vendorID == 0x8087 || name.find("Intel") != std::string::npos; }

		std::string name = "Unknown";
		uint32_t vendorID = 0;
		uint32_t memory = 0;
		void* data = nullptr;
	};

	class V_API RHI_Device
	{
		RHI_Device(Context* context);
		~RHI_Device();

		bool ProfilingCreateQuery(void** query, RHI_Query_Type type) const;
		bool ProfilingQueryStart(void* query_object) const;
		bool ProfilingGetTimeStamp(void* query_object) const;
		float ProfilingGetDuration(void* query_disjoint, void* query_start, void* query_end) const;
		void ProfilingReleaseQuery(void* query_object);
		uint32_t ProfilingGetGpuMemory();
		uint32_t ProfilingGetGpuMemoryUsage();

		void AddDisplayMode(uint32_t width, uint32_t height, uint32_t refresh_rate_numerator, uint32_t refresh_rate_denominator);
		bool GetDisplayModeFastest(DisplayMode* display_mode);
		void AddAdapter(const std::string& name, uint32_t memory, uint32_t vendor_id, void* adapter);
		void SetPrimaryAdapter(const DisplayAdapter* primary_adapter);
		const std::vector<DisplayAdapter>& GetAdapters()	const { return m_displayAdapters; }
		const DisplayAdapter* GetPrimaryAdapter()			const { return m_primaryAdapter; }

		auto IsInitialized()            const { return m_initialized; }
		RHI_Context* GetContextRhi()	const { return m_rhi_context.get(); }
		Context* GetContext()           const { return m_context; }

	private:
		std::shared_ptr<RHI_Context> m_rhi_context;
		Context* m_context = nullptr;

		bool m_initialized = false;
		const DisplayAdapter* m_primaryAdapter = nullptr;
		std::vector<DisplayMode> m_displayModes;
		std::vector<DisplayAdapter> m_displayAdapters;

	};

#pragma endregion

#pragma region SwapChain

	class RHI_SwapChain : VObject
	{
	public:
		RHI_SwapChain(
			void* window_handle,
			const std::shared_ptr<RHI_Device>& rhi_device,
			uint32_t width,
			uint32_t height,
			RHI_Format format = RHI_Format_R8G8B8A8_Unorm,
			uint32_t buffer_count = 2,
			uint32_t flags = RHI_Present_Immediate
			);
		~RHI_SwapChain();

		bool Resize(uint32_t width, uint32_t height, const bool force = false);
		bool Present();

		// Misc
		uint32_t GetWidth()                 const { return m_width; }
		uint32_t GetHeight()                const { return m_height; }
		uint32_t GetBufferCount()           const { return m_buffer_count; }
		uint32_t GetFlags()                 const { return m_flags; }
		uint32_t GetCmdIndex()              const { return m_cmd_index; }
		uint32_t GetImageIndex()            const { return m_image_index; }
		bool IsInitialized()                const { return m_initialized; }
		RHI_CommandList* GetCmdList() { return m_cmd_index < static_cast<uint32_t>(m_cmd_lists.size()) ? m_cmd_lists[m_cmd_index].get() : nullptr; }
		void* GetImageAcquireSemaphore()    const { return m_image_acquired_semaphore[m_cmd_index]; }
		bool IsPresenting()                 const { return m_present; }

		// Layout
		const RHI_Image_Layout GetLayout() const { return m_layout; }
		void SetLayout(RHI_Image_Layout layout, RHI_CommandList* command_list = nullptr);

		// GPU Resources
		void* Get_Resource(uint32_t i = 0)          const { return m_resource[i]; }
		void* Get_Resource_View(uint32_t i = 0)     const { return m_resource_view[i]; }
		void* Get_Resource_View_RenderTarget()      const { return m_resource_view_renderTarget; }
		void*& GetCmdPool() { return m_cmd_pool; }

	private:
		bool AcquireNextImage();

		bool m_initialized = false;
		bool m_windowed = false;
		uint32_t m_buffer_count = 0;
		uint32_t m_width = 0;
		uint32_t m_height = 0;
		uint32_t m_flags = 0;
		RHI_Format m_format = RHI_Format_R8G8B8A8_Unorm;

		void* m_swap_chain_view = nullptr;
		void* m_resource_view_renderTarget = nullptr;
		void* m_surface = nullptr;
		void* m_window_handle = nullptr;
		void* m_cmd_pool = nullptr;
		bool m_image_acquired = false;
		bool m_present = true;
		uint32_t m_cmd_index = 0;
		uint32_t m_image_index = 0;
		RHI_Device* m_rhi_device = nullptr;
		RHI_Image_Layout m_layout = RHI_Image_Undefined;
		std::vector<std::shared_ptr<RHI_CommandList>> m_cmd_lists;
		std::array<void*, state_max_render_target_count> m_image_acquired_semaphore = { nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr };
		std::array<void*, state_max_render_target_count> m_resource_view = { nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr };
		std::array<void*, state_max_render_target_count> m_resource = { nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr };
	};

#pragma endregion

#pragma region Viewport

	class V_API RHI_Viewport : public VObject
	{
	public:
		RHI_Viewport(const float x = 0.0f, const float y = 0.0f, const float width = 0.0f, const float height = 0.0f, const float depth_min = 0.0f, const float depth_max = 1.0f)
		{
			this->x = x;
			this->y = y;
			this->width = width;
			this->height = height;
			this->depth_min = depth_min;
			this->depth_max = depth_max;
		}

		RHI_Viewport(const RHI_Viewport& viewport)
		{
			x = viewport.x;
			y = viewport.y;
			width = viewport.width;
			height = viewport.height;
			depth_min = viewport.depth_min;
			depth_max = viewport.depth_max;
		}

		~RHI_Viewport() = default;

		bool operator==(const RHI_Viewport& rhs) const
		{
			return
				x == rhs.x && y == rhs.y &&
				width == rhs.width && height == rhs.height &&
				depth_min == rhs.depth_min && depth_max == rhs.depth_max;
		}

		bool operator!=(const RHI_Viewport& rhs) const
		{
			return !(*this == rhs);
		}

		bool IsDefined() const
		{
			return
				x != 0.0f &&
				y != 0.0f &&
				width != 0.0f &&
				height != 0.0f &&
				depth_min != 0.0f &&
				depth_max != 0.0f;
		}

		float AspectRatio()	const { return width / height; }

		float x = 0.0f;
		float y = 0.0f;
		float width = 0.0f;
		float height = 0.0f;
		float depth_min = 0.0f;
		float depth_max = 0.0f;

	};

#pragma endregion

#pragma region Vertex

	struct RHI_Vertex_Undefined {};

	struct RHI_Vertex_Pos
	{
		RHI_Vertex_Pos(const float& posX,const float& posY,const float& posZ)
		{
			pos[0] = posX;
			pos[1] = posY;
			pos[2] = posZ;
		}
		float pos[3] = {0};
	};

	struct RHI_Vertex_PosTex
	{
		RHI_Vertex_PosTex(const float pos_x, const float pos_y, const float pos_z, const float tex_x, const float tex_y)
		{
			pos[0] = pos_x;
			pos[1] = pos_y;
			pos[2] = pos_z;

			tex[0] = tex_x;
			tex[1] = tex_y;
		}

		float pos[3] = { 0 };
		float tex[2] = { 0 };
	};

	struct RHI_Vertex_PosCol
	{
		RHI_Vertex_PosCol(const float& posX, const float& posY, const float& posZ, const float& colX, const float& colY, const float& colZ, const float& colW)
		{
			this->pos[0] = posX;
			this->pos[1] = posY;
			this->pos[2] = posZ;

			this->col[0] = colX;
			this->col[1] = colY;
			this->col[2] = colZ;
			this->col[3] = colW;
		}

		float pos[3] = { 0 };
		float col[4] = { 0 };
	};

	struct RHI_Vertex_Pos2dTexCol8
	{
		RHI_Vertex_Pos2dTexCol8() = default;

		float pos[2] = { 0 };
		float tex[2] = { 0 };
		uint32_t col = 0;
	};

	struct RHI_Vertex_PosTexNorTan
	{
		RHI_Vertex_PosTexNorTan() = default;
		RHI_Vertex_PosTexNorTan(
			const float& posX, const float& posY, const float& posZ,
			const float& texX, const float& texY,
			const float& norX, const float& norY, const float& norZ,
			const float& tanX, const float& tanY, const float& tanZ)
		{
			this->pos[0] = posX;
			this->pos[1] = posY;
			this->pos[2] = posZ;

			this->tex[0] = texX;
			this->tex[1] = texY;

			this->nor[0] = norX;
			this->nor[1] = norY;
			this->nor[2] = norZ;

			this->tan[0] = tanX;
			this->tan[1] = tanY;
			this->tan[2] = tanZ;
		}

		float pos[3] = { 0 };
		float tex[2] = { 0 };
		float nor[3] = { 0 };
		float tan[3] = { 0 };
	};

	static_assert(std::is_trivially_copyable<RHI_Vertex_Pos>::value, "RHI_Vertex_Pos is not trivially copyable");
	static_assert(std::is_trivially_copyable<RHI_Vertex_PosTex>::value, "RHI_Vertex_PosTex is not trivially copyable");
	static_assert(std::is_trivially_copyable<RHI_Vertex_PosCol>::value, "RHI_Vertex_PosCol is not trivially copyable");
	static_assert(std::is_trivially_copyable<RHI_Vertex_Pos2dTexCol8>::value, "RHI_Vertex_Pos2dTexCol8 is not trivially copyable");
	static_assert(std::is_trivially_copyable<RHI_Vertex_PosTexNorTan>::value, "RHI_Vertex_PosTexNorTan is not trivially copyable");

	enum RHI_Vertex_Type
	{
		RHI_Vertex_Type_Unknown,
		RHI_Vertex_Type_Position,
		RHI_Vertex_Type_PositionColor,
		RHI_Vertex_Type_PositionTexture,
		RHI_Vertex_Type_PositionTextureNormalTangent,
		RHI_Vertex_Type_Position2dTextureColor8
	};

	template <typename T>
	constexpr RHI_Vertex_Type RHI_Vertex_Type_To_Enum() { return RHI_Vertex_Type_Unknown; }

	// Explicit template instantiation
	template<> inline RHI_Vertex_Type RHI_Vertex_Type_To_Enum<RHI_Vertex_Pos>() { return RHI_Vertex_Type_Position; }
	template<> inline RHI_Vertex_Type RHI_Vertex_Type_To_Enum<RHI_Vertex_PosTex>() { return RHI_Vertex_Type_PositionTexture; }
	template<> inline RHI_Vertex_Type RHI_Vertex_Type_To_Enum<RHI_Vertex_PosCol>() { return RHI_Vertex_Type_PositionColor; }
	template<> inline RHI_Vertex_Type RHI_Vertex_Type_To_Enum<RHI_Vertex_Pos2dTexCol8>() { return RHI_Vertex_Type_Position2dTextureColor8; }
	template<> inline RHI_Vertex_Type RHI_Vertex_Type_To_Enum<RHI_Vertex_PosTexNorTan>() { return RHI_Vertex_Type_PositionTextureNormalTangent; }

#pragma endregion

#pragma region VertexBuffer

	class V_API RHI_VertexBuffer : public VObject
	{
	public:
		RHI_VertexBuffer(const std::shared_ptr<RHI_Device>& rhi_device, const uint32_t stride = 0)
		{
			m_rhi_device = rhi_device;
			m_stride = stride;
		}

		~RHI_VertexBuffer()
		{
			_destroy();
		}

		template<typename T>
		bool Create(const std::vector<T>& vertices)
		{
			m_stride = static_cast<uint32_t>(sizeof(T));
			m_vertex_count = static_cast<uint32_t>(vertices.size());
			m_size_gpu = static_cast<uint64_t>(m_stride * m_vertex_count);
			return _create(static_cast<const void*>(vertices.data()));
		}

		template<typename T>
		bool Create(const T* vertices, const uint32_t vertex_count)
		{
			m_stride = static_cast<uint32_t>(sizeof(T));
			m_vertex_count = vertex_count;
			m_size_gpu = static_cast<uint64_t>(m_stride * m_vertex_count);
			return _create(static_cast<const void*>(vertices));
		}

		template<typename T>
		bool CreateDynamic(const uint32_t vertex_count)
		{
			m_stride = static_cast<uint32_t>(sizeof(T));
			m_vertex_count = vertex_count;
			m_size_gpu = static_cast<uint64_t>(m_stride * m_vertex_count);
			return _create(nullptr);
		}

			void* Map();
			bool Unmap();

			void* GetResource()         const { return m_buffer; }
			uint32_t GetStride()        const { return m_stride; }
			uint32_t GetVertexCount()   const { return m_vertex_count; }

		private:
			bool _create(const void* vertices);
			void _destroy();

			bool m_persistent_mapping = true; // only affects Vulkan
			void* m_mapped = nullptr;
			uint32_t m_stride = 0;
			uint32_t m_vertex_count = 0;

			// API
			std::shared_ptr<RHI_Device> m_rhi_device;
			void* m_buffer = nullptr;
			void* m_allocation = nullptr;
			bool m_is_mappable = true;
	};



#pragma endregion

#pragma region IndexBuffer

	class V_API RHI_IndexBuffer : public VObject
	{
	public:
		RHI_IndexBuffer(const std::shared_ptr<RHI_Device>& rhi_device)
		{
			m_rhi_device = rhi_device;
		}

		~RHI_IndexBuffer();

		template<typename T>
		bool Create(const std::vector<T>& indices)
		{
			m_stride = sizeof(T);
			m_index_count = static_cast<uint32_t>(indices.size());
			m_size_gpu = static_cast<uint64_t>(m_stride * m_index_count);
			return _Create(static_cast<const void*>(indices.data()));
		}

		template<typename T>
		bool Create(const T* indices, const uint32_t index_count)
		{
			m_stride = sizeof(T);
			m_index_count = index_count;
			m_size_gpu = static_cast<uint64_t>(m_stride * m_index_count);
			return _Create(static_cast<const void*>(indices));
		}

		template<typename T>
		bool CreateDynamic(const uint32_t index_count)
		{
			m_stride = sizeof(T);
			m_index_count = index_count;
			m_size_gpu = static_cast<uint64_t>(m_stride * m_index_count);
			return _Create(nullptr);
		}

		void* Map() const;
		bool Unmap() const;
		bool Flush() const;

		void* GetResource()		    const { return m_buffer; }
		uint32_t GetIndexCount()	const { return m_index_count; }
		bool Is16Bit()			    const { return sizeof(uint16_t) == m_stride; }
		bool Is32Bit()			    const { return sizeof(uint32_t) == m_stride; }

	protected:
		bool _Create(const void* indices);

		uint32_t m_stride = 0;
		uint32_t m_index_count = 0;
		std::shared_ptr<RHI_Device> m_rhi_device;

		// API
		void* m_buffer = nullptr;
		void* m_buffer_memory = nullptr;
		bool m_mappable = false;

	};

#pragma endregion

#pragma region Sampler

	#define SAMPLER_POINT		RHI_Filter_Nearest, RHI_Filter_Nearest, RHI_Sampler_Mipmap_Nearest
	#define SAMPLER_BILINEAR	RHI_Filter_Linear,  RHI_Filter_Linear,  RHI_Sampler_Mipmap_Nearest
	#define SAMPLER_TRILINEAR	RHI_Filter_Linear,  RHI_Filter_Linear,  RHI_Sampler_Mipmap_Linear

	class RHI_Sampler : public VObject
	{
	public:
		RHI_Sampler(
			const std::shared_ptr<RHI_Device>& rhi_device,
			const RHI_Filter filter_min = RHI_Filter_Nearest,
			const RHI_Filter filter_mag = RHI_Filter_Nearest,
			const RHI_Sampler_Mipmap_Mode filter_mipmap = RHI_Sampler_Mipmap_Nearest,
			const RHI_Sampler_Address_Mode sampler_address_mode = RHI_Sampler_Address_Wrap,
			const RHI_Comparison_Function comparison_function = RHI_Comparison_Always,
			const bool anisotropy_enabled = false,
			const bool comparison_enabled = false
			);

		~RHI_Sampler();

		auto GetFilterMin()				const { return m_filter_min; }
		auto GetFilterMag()				const { return m_filter_mag; }
		auto GetFilterMipmap()			const { return m_filter_mipmap; }
		auto GetAddressMode()			const { return m_sampler_address_mode; }
		auto GetComparisonFunction()	const { return m_comparison_function; }
		auto GetAnisotropyEnabled()		const { return m_anisotropy_enabled; }
		auto GetComparisonEnabled()		const { return m_comparison_enabled; }
		auto GetResource()				const { return m_resource; }

	private:
		RHI_Filter m_filter_min = RHI_Filter_Nearest;
		RHI_Filter m_filter_mag = RHI_Filter_Nearest;
		RHI_Sampler_Mipmap_Mode m_filter_mipmap = RHI_Sampler_Mipmap_Nearest;
		RHI_Sampler_Address_Mode m_sampler_address_mode = RHI_Sampler_Address_Wrap;
		RHI_Comparison_Function m_comparison_function = RHI_Comparison_Always;
		bool m_anisotropy_enabled = false;
		bool m_comparison_enabled = false;

		// API
		void* m_resource = nullptr;

		// Dependencies
		std::shared_ptr<RHI_Device> m_rhi_device;
	};

#pragma endregion

#pragma region Texture

	enum RHI_Texture_Flags : uint16_t
	{
		RHI_Texture_ShaderView = 1 << 0,
		RHI_Texture_UnorderedAccessView = 1 << 1,
		RHI_Texture_RenderTargetView = 1 << 2,
		RHI_Texture_DepthStencilView = 1 << 3,
		RHI_Texture_DepthStencilViewReadOnly = 1 << 4,
		RHI_Texture_Grayscale = 1 << 5,
		RHI_Texture_Transparent = 1 << 6,
		RHI_Texture_GenerateMipsWhenLoading = 1 << 7
	};

	enum RHI_Shader_View_Type : uint8_t
	{
		RHI_Shader_View_ColorDepth,
		RHI_Shader_View_Stencil,
		RHI_Shader_View_Unordered_Access
	};

	class V_API RHI_Texture : public IResource
	{
	public:
		RHI_Texture(Context* context);
		~RHI_Texture();

		//= IResource ===========================================
		bool SaveToFile(const std::string& file_path) override;
		bool LoadFromFile(const std::string& file_path) override;
		//=======================================================

		auto GetWidth() const { return m_width; }
		void SetWidth(const uint32_t width) { m_width = width; }

		auto GetHeight() const { return m_height; }
		void SetHeight(const uint32_t height) { m_height = height; }

		auto GetGrayscale() const { return m_flags & RHI_Texture_Grayscale; }
		void SetGrayscale(const bool is_grayscale) { is_grayscale ? m_flags |= RHI_Texture_Grayscale : m_flags &= ~RHI_Texture_Grayscale; }

		auto GetTransparency() const { return m_flags & RHI_Texture_Transparent; }
		void SetTransparency(const bool is_transparent) { is_transparent ? m_flags |= RHI_Texture_Transparent : m_flags &= ~RHI_Texture_Transparent; }

		uint32_t GetBitsPerChannel() const { return m_bits_per_channel; }
		void SetBitsPerChannel(const uint32_t bits) { m_bits_per_channel = bits; }
		uint32_t GetBytesPerChannel() const { return m_bits_per_channel / 8; }
		uint32_t GetBytesPerPixel() const { return (m_bits_per_channel / 8) * m_channel_count; }

		uint32_t GetChannelCount() const { return m_channel_count; }
		void SetChannelCount(const uint32_t channel_count) { m_channel_count = channel_count; }

		auto GetFormat() const { return m_format; }
		void SetFormat(const RHI_Format format) { m_format = format; }

		// Data
		bool HasData() const { return !m_data.empty(); }
		const auto& GetData() const { return m_data; }
		void SetData(const std::vector<std::vector<std::byte>>& data) { m_data = data; }
		auto AddMipmap() { return &m_data.emplace_back(std::vector<std::byte>()); }
		bool HasMipmaps() const { return !m_data.empty(); }
		uint32_t GetMiplevels() const { return m_mip_levels; }
		std::vector<std::byte>* GetData(uint32_t mipmap_index);
		std::vector<std::byte> GetMipmap(uint32_t index);

		// Binding type
		bool IsSampled()                    const { return m_flags & RHI_Texture_ShaderView; }
		bool IsRenderTargetCompute()        const { return m_flags & RHI_Texture_UnorderedAccessView; }
		bool IsRenderTargetDepthStencil()   const { return m_flags & RHI_Texture_DepthStencilView; }
		bool IsRenderTargetColor()          const { return m_flags & RHI_Texture_RenderTargetView; }

		// Format type
		bool IsDepthFormat()    const { return m_format == RHI_Format_D32_Float || m_format == RHI_Format_D32_Float_S8X24_Uint; }
		bool IsStencilFormat()  const { return m_format == RHI_Format_D32_Float_S8X24_Uint; }
		bool IsDepthStencil()   const { return IsDepthFormat() || IsStencilFormat(); }
		bool IsColorFormat()    const { return !IsDepthStencil(); }

		// Layout
		void SetLayout(const RHI_Image_Layout layout, RHI_CommandList* command_list = nullptr);
		RHI_Image_Layout GetLayout() const { return m_layout; }

		// Misc
		auto GetArraySize()         const { return m_array_size; }
		const auto& GetViewport()   const { return m_viewport; }
		uint16_t GetFlags()         const { return m_flags; }

		// GPU resources
		void* Get_Resource()                                                const { return m_resource; }
		void  Set_Resource(void* resource) { m_resource = resource; }
		void* Get_Resource_View(const uint32_t i = 0)                       const { return m_resource_view[i]; }
		void* Get_Resource_View_UnorderedAccess()	                        const { return m_resource_view_unorderedAccess; }
		void* Get_Resource_View_DepthStencil(const uint32_t i = 0)          const { return i < m_resource_view_depthStencil.size() ? m_resource_view_depthStencil[i] : nullptr; }
		void* Get_Resource_View_DepthStencilReadOnly(const uint32_t i = 0)  const { return i < m_resource_view_depthStencilReadOnly.size() ? m_resource_view_depthStencilReadOnly[i] : nullptr; }
		void* Get_Resource_View_RenderTarget(const uint32_t i = 0)          const { return i < m_resource_view_renderTarget.size() ? m_resource_view_renderTarget[i] : nullptr; }

	protected:
		bool LoadFromFile_NativeFormat(const std::string& file_path);
		bool LoadFromFile_ForeignFormat(const std::string& file_path, bool generate_mipmaps);
		static uint32_t GetChannelCountFromFormat(RHI_Format format);
		virtual bool CreateResourceGpu() { LOG_ERROR("Function not implemented by API"); return false; }

		uint32_t m_bits_per_channel = 8;
		uint32_t m_width = 0;
		uint32_t m_height = 0;
		uint32_t m_channel_count = 4;
		uint32_t m_array_size = 1;
		uint32_t m_mip_levels = 1;
		RHI_Format m_format = RHI_Format_Undefined;
		RHI_Image_Layout m_layout = RHI_Image_Undefined;
		uint16_t m_flags = 0;
		RHI_Viewport m_viewport;
		std::vector<std::vector<std::byte>> m_data;
		std::shared_ptr<RHI_Device> m_rhi_device;

		// API
		void* m_resource_view[2] = { nullptr, nullptr }; // color/depth, stencil
		void* m_resource_view_unorderedAccess = nullptr;
		void* m_resource = nullptr;
		std::array<void*, state_max_render_target_count> m_resource_view_renderTarget = { nullptr };
		std::array<void*, state_max_render_target_count> m_resource_view_depthStencil = { nullptr };
		std::array<void*, state_max_render_target_count> m_resource_view_depthStencilReadOnly = { nullptr };
	private:
		uint32_t GetByteCount();

	};

#pragma endregion


}