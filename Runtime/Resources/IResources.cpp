#include "IResources.h"

#include "../RHI/RHI.h"

namespace V
{
	IResource::IResource(Context* context, const Resource_Type type)
	{
		m_context = context;
		m_resource_type = type;
		m_load_state = LoadState_Idle;
	}

	template <typename T>
	inline constexpr Resource_Type IResource::TypeToEnum() { return Resource_Unknown; }

	template<typename T>
	inline constexpr void validate_resource_type() { static_assert(std::is_base_of<IResource, T>::value, "Provided type does not implement IResource"); }

	// Explicit template instantiation
	#define INSTANTIATE_TO_RESOURCE_TYPE(T, enumT) template<> V_API Resource_Type IResource::TypeToEnum<T>() { validate_resource_type<T>(); return enumT; }

	// To add a new resource to the engine, simply register it here
	INSTANTIATE_TO_RESOURCE_TYPE(RHI_Texture, Resource_Texture)
	INSTANTIATE_TO_RESOURCE_TYPE(RHI_Texture2D, Resource_Texture2d)
	INSTANTIATE_TO_RESOURCE_TYPE(RHI_TextureCube, Resource_TextureCube)
	//INSTANTIATE_TO_RESOURCE_TYPE(AudioClip, Resource_Audio)
	//INSTANTIATE_TO_RESOURCE_TYPE(Material, Resource_Material)
	//INSTANTIATE_TO_RESOURCE_TYPE(Model, Resource_Model)
	//INSTANTIATE_TO_RESOURCE_TYPE(Animation, Resource_Animation)
	//INSTANTIATE_TO_RESOURCE_TYPE(Font, Resource_Font)


}