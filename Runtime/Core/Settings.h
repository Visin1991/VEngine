#pragma once

#include <string>

#include "EngineDefs.h"
#include "IModule.h"

namespace V
{

	class Context;

	class V_API Settings : public IModule
	{
	public:
		Settings(Context* context);
		~Settings();

		bool Initialize();

		auto GetIsFullScreen() const { return m_is_fullscreen; }
		auto GetIsMouseVisible() const { return m_is_mouse_visible; }

		// Third party lib versions
		std::string m_versionAngelScript;
		std::string m_versionAssimp;
		std::string m_versionBullet;
		std::string m_versionFMOD;
		std::string m_versionFreeImage;
		std::string m_versionFreeType;
		std::string m_versionImGui;
		std::string m_versionPugiXML = "1.90";
		std::string m_versionGraphicsAPI;

	private:
		void Save() const;
		void Load();

		void Reflect();
		void Map();

		bool m_is_fullscreen = false;
		bool m_is_mouse_visible = true;
		uint32_t m_shadow_map_resolution = 0;
		float width = 0;
		float height = 0;
		uint32_t m_anisotropy = 0;
		uint32_t m_max_thread_count = 0;
		double m_fps_limit = 0;
		Context* m_context = nullptr;

	};

}