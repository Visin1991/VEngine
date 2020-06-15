#pragma once


#include <memory>

#include "../Core/Context.h"
#include "../Core/Log.h"
#include "../Core/FileSystem.h"
#include "../Core/VObject.h"


namespace V
{
	enum Resource_Type
	{
		Resource_Unknown,
		Resource_Texture,
		Resource_Texture2d,
		Resource_TextureCube,
		Resource_Audio,
		Resource_Material,
		Resource_Mesh,
		Resource_Model,
		Resource_Cubemap,
		Resource_Animation,
		Resource_Font,
		Resource_Shader
	};

	enum LoadState
	{
		LoadState_Idle,
		LoadState_Started,
		LoadState_Completed,
		LoadState_Failed
	};

	class V_API IResource : VObject
	{
	public:
		IResource(Context* context, Resource_Type type);
		virtual ~IResource() = default;

		void SetResourceFilePath(const std::string & path)
		{
			const bool is_native_file = FileSystem::IsEngineMaterialFile(path) || FileSystem::IsEngineModelFile(path);

			// If this is an native engine file, don't do a file check as no actual foreign material exists (it was created on the fly)
			if (!is_native_file)
			{
				if (!FileSystem::IsFile(path))
				{
					LOG_ERROR("\"%s\" is not a valid file path", path.c_str());
					return;
				}
			}

			const std::string file_path_relative = FileSystem::GetRelativePath(path);

			// Foreign file
			if (!FileSystem::IsEngineFile(path))
			{
				m_resource_file_path_foreign = file_path_relative;
				m_resource_file_path_native = FileSystem::NativizeFilePath(file_path_relative);
			}
			// Native file
			else
			{
				m_resource_file_path_foreign.clear();
				m_resource_file_path_native = file_path_relative;
			}
			m_resource_name = FileSystem::GetFileNameNoExtensionFromFilePath(file_path_relative);
			m_resource_directory = FileSystem::GetDirectoryFromFilePath(file_path_relative);
		}

		Resource_Type GetResourceType()                 const { return m_resource_type; }
		const char* GetResourceTypeCstr()               const { return typeid(*this).name(); }
		bool HasFilePathNative()                        const { return !m_resource_file_path_native.empty(); }
		const std::string& GetResourceFilePath()        const { return m_resource_file_path_foreign; }
		const std::string& GetResourceFilePathNative()  const { return m_resource_file_path_native; }
		const std::string& GetResourceName()            const { return m_resource_name; }
		const std::string& GetResourceFileName()        const { return m_resource_name; }
		const std::string& GetResourceDirectory()       const { return m_resource_directory; }


		// Misc
		LoadState GetLoadState() const { return m_load_state; }

		// IO
		virtual bool SaveToFile(const std::string & file_path) { return true; }
		virtual bool LoadFromFile(const std::string & file_path) { return true; }

		// Type
		template <typename T>
		static constexpr Resource_Type TypeToEnum();

	protected:
		Resource_Type m_resource_type = Resource_Unknown;
		LoadState m_load_state = LoadState_Idle;

	private:
		std::string m_resource_name;
		std::string m_resource_directory;
		std::string m_resource_file_path_native;
		std::string m_resource_file_path_foreign;
	};


}