#pragma once

#include <stdint.h>
#include <string>
#include <vector>
#include <memory>

#include "../Core/EngineDefs.h"
#include "../Core/VObject.h"

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

	enum RHI_Present_Mode : uint32_t
	{
		Present_Immediate = 1 << 0,
		Present_Mailbox = 1 << 1,
		Present_Fifo = 1 << 2,
		Present_Relaxed = 1 << 3,
		Present_SharedDemandRefresh = 1 << 4,
		Present_SharedDContinuousRefresh = 1 << 5,

		// Find a way to remove those legacy D3D11 only flags
		Swap_Discard = 1 << 6,
		Swap_Sequential = 1 << 7,
		Swap_Flip_Sequential = 1 << 8,
		Swap_Flip_Discard = 1 << 9,
		SwapChain_Allow_Mode_Switch = 1 << 10
	};

	enum RHI_Query_Type
	{
		Query_Timestamp,
		Query_Timestamp_Disjoint
	};

	enum RHI_Clear_Buffer
	{
		Clear_Depth = 1 << 0,
		Clear_Stencil = 1 << 1
	};

	enum RHI_Buffer_Scope
	{
		Buffer_VertexShader,
		Buffer_PixelShader,
		Buffer_Global,
		Buffer_NotAssigned
	};

	enum RHI_PrimitiveTopology_Mode
	{
		PrimitiveTopology_TriangleList,
		PrimitiveTopology_LineList,
		PrimitiveTopology_NotAssigned
	};

	enum RHI_Cull_Mode
	{
		Cull_None,
		Cull_Front,
		Cull_Back
	};

	enum RHI_Fill_Mode
	{
		Fill_Solid,
		Fill_Wireframe
	};

	enum RHI_Filter
	{
		Filter_Nearest,
		Filter_Linear,
	};

	enum RHI_Sampler_Mipmap_Mode
	{
		Sampler_Mipmap_Nearest,
		Sampler_Mipmap_Linear,
	};

	enum RHI_Sampler_Address_Mode
	{
		Sampler_Address_Wrap,
		Sampler_Address_Mirror,
		Sampler_Address_Clamp,
		Sampler_Address_Border,
		Sampler_Address_MirrorOnce,
	};

	enum RHI_Comparison_Function
	{
		Comparison_Never,
		Comparison_Less,
		Comparison_Equal,
		Comparison_LessEqual,
		Comparison_Greater,
		Comparison_NotEqual,
		Comparison_GreaterEqual,
		Comparison_Always
	};

	enum RHI_Format
	{
		// R
		Format_R8_UNORM,
		Format_R16_UINT,
		Format_R16_FLOAT,
		Format_R32_UINT,
		Format_R32_FLOAT,
		Format_D32_FLOAT,
		Format_R32_FLOAT_TYPELESS,
		// RG
		Format_R8G8_UNORM,
		Format_R16G16_FLOAT,
		Format_R32G32_FLOAT,
		// RGB
		Format_R32G32B32_FLOAT,
		// RGBA
		Format_R8G8B8A8_UNORM,
		Format_R16G16B16A16_FLOAT,
		Format_R32G32B32A32_FLOAT
	};

	enum RHI_Blend
	{
		Blend_Zero,
		Blend_One,
		Blend_Src_Color,
		Blend_Inv_Src_Color,
		Blend_Src_Alpha,
		Blend_Inv_Src_Alpha
	};

	enum RHI_Blend_Operation
	{
		Blend_Operation_Add,
		Blend_Operation_Subtract,
		Blend_Operation_Rev_Subtract,
		Blend_Operation_Min,
		Blend_Operation_Max
	};

	enum RHI_Descriptor_Type
	{
		Descriptor_Sampler,
		Descriptor_Texture,
		Descriptor_ConstantBuffer
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
			RHI_Format format = Format_R8G8B8A8_UNORM,
			uint32_t buffer_count = 1,
			uint32_t flags = Present_Immediate
		);
		~RHI_SwapChain();

		bool Resize(uint32_t width, uint32_t height);
		bool AcquireNextImage();
		bool Present() const;

		auto GetWidth()				const { return m_width; }
		auto GetHeight()			const { return m_height; }
		auto IsInitialized()		const { return m_initialized; }
		auto GetSwapChainView()		const { return m_swap_chain_view; }
		auto GetRenderTargetView()	const { return m_render_target_view; }
		auto GetRenderPass()		const { return m_render_pass; }
		auto GetBufferCount()		const { return m_buffer_count; }
		auto& GetFrameBuffer() { return m_frame_buffers[m_image_index]; }
		auto& GetSemaphoreImageAcquired() { return m_semaphores_image_acquired[m_image_index]; }
		auto& GetImageIndex() { return m_image_index; }
		void SetSemaphoreRenderFinished(void* semaphore_cmd_list_consumed) { m_semaphore_cmd_list_consumed = semaphore_cmd_list_consumed; }

	private:
		bool CreateRenderPass();

		bool m_initialized = false;
		bool m_windowed = false;
		uint32_t m_buffer_count = 0;
		uint32_t m_max_resolution = 16384;
		uint32_t m_width = 0;
		uint32_t m_height = 0;
		uint32_t m_flags = 0;
		RHI_Format m_format = Format_R8G8B8A8_UNORM;

		std::shared_ptr<RHI_Device> m_rhi_device;
		void* m_swap_chain_view = nullptr;
		void* m_render_target_view = nullptr;
		void* m_surface = nullptr;
		void* m_render_pass = nullptr;
		void* m_window_handle = nullptr;
		void* m_semaphore_cmd_list_consumed = nullptr;
		uint32_t m_image_index = 0;
		std::vector<void*> m_semaphores_image_acquired;
		std::vector<void*> m_image_views;
		std::vector<void*> m_frame_buffers;
		bool image_acquired = false;


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

	class V_API RHI_VertexBuffer : VObject
	{
		RHI_VertexBuffer(const std::shared_ptr<RHI_Device>& rhi_device, const uint32_t stride = 0)
		{
			m_rhi_device = rhi_device;
			m_stride = stride;
		}

		~RHI_VertexBuffer();

		template<typename T>
		bool Create(const std::vector<T>& vertices)
		{
			m_is_dynamic = false;
			m_stride = static_cast<uint32_t>(sizeof(T));
			m_vertex_count = static_cast<uint32_t>(vertices.size());
			m_size = static_cast<uint64_t>(m_stride * m_vertex_count);
			return _Create(static_cast<const void*>(vertices.data()));
		}

		template<typename T>
		bool Create(const T* vertices, const uint32_t vertex_count)
		{
			m_is_dynamic = false;
			m_stride = static_cast<uint32_t>(sizeof(T));
			m_vertex_count = vertex_count;
			m_size = static_cast<uint64_t>(m_stride * m_vertex_count);
			return _Create(static_cast<const void*>(vertices));
		}

		template<typename T>
		bool CreateDynamic(const uint32_t vertex_count)
		{
			m_is_dynamic = true;
			m_stride = static_cast<uint32_t>(sizeof(T));
			m_vertex_count = vertex_count;
			m_size = static_cast<uint64_t>(m_stride * m_vertex_count);
			return _Create(nullptr);
		}

		void* Map() const;
		bool Unmap() const;

		auto GetResource()      const { return m_buffer; }
		auto& GetSize()         const { return m_size; }
		auto GetStride()        const { return m_stride; }
		auto GetVertexCount()   const { return m_vertex_count; }

	private:
		bool _Create(const void* vertices);

		uint32_t m_stride = 0;
		uint32_t m_vertex_count = 0;
		bool m_is_dynamic = false;

		// API
		std::shared_ptr<RHI_Device> m_rhi_device;
		void* m_buffer = nullptr;
		void* m_buffer_memory = nullptr;
	};

#pragma endregion


}