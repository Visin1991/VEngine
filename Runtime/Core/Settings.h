#pragma once

#include <string>
#include <vector>


#include "EngineDefs.h"
#include "IModule.h"

namespace V
{

	class Context;

	struct ThirdPartyLib
	{
		ThirdPartyLib(const std::string& name, const std::string& version, const std::string& url)
		{
			this->name = name;
			this->version = version;
			this->url = url;
		}

		std::string name;
		std::string version;
		std::string url;
	};

	class V_API Settings : public IModule
	{
	public:
		Settings(Context* context);
		~Settings();

		//= Subsystem =============
		bool Initialize() override;

		bool GetIsFullScreen()      const { return m_is_fullscreen; }
		bool GetIsMouseVisible()    const { return m_is_mouse_visible; }
		bool Loaded()            const { return m_loaded; }

		void RegisterThirdPartyLib(const std::string& name, const std::string& version, const std::string& url);
		const auto& GetThirdPartyLibs() const { return m_third_party_libs; }

	private:
		void Save() const;
		void Load();

		void Reflect();
		void Map() const;

		bool m_is_fullscreen = false;
		bool m_is_mouse_visible = true;
		uint32_t m_shadow_map_resolution = 0;
		uint64_t m_renderer_flags = 0;
		float width = 0;
		float height = 0;
		uint32_t m_anisotropy = 0;
		uint32_t m_max_thread_count = 0;
		double m_fps_limit = 0;
		bool m_loaded = false;
		Context* m_context = nullptr;
		std::vector<ThirdPartyLib> m_third_party_libs;

	};

}