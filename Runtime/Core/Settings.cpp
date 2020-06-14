#include <fstream>

#include "Settings.h"
#include "FileSystem.h"
#include "Log.h"


using namespace std;


namespace _Settings
{
	ofstream fout;
	ifstream fin;
	string file_name = "V.ini";

	template<class T>
	void write_setting(ofstream& fout, const string& name, T value)
	{
		fout << name << "=" << value << endl;
	}

	template<class T>
	void read_setting(ifstream& fin, const string& name, T& value)
	{
		for (string line; getline(fin, line);)
		{
			const auto first_index = line.find_first_of('=');
			if (name == line.substr(0, first_index))
			{
				const auto lastindex = line.find_last_of('=');
				const auto read_value = line.substr(lastindex + 1, line.length());
				value = static_cast<T>(stof(read_value));
				return;
			}
		}
	}
}

namespace V
{
	Settings::Settings(Context* context) : IModule(context)
	{
		m_context = context;
	}

	Settings::~Settings()
	{
		Reflect();
		Save();
	}

	bool Settings::Initialize()
	{
		Reflect();

		if (FileSystem::FileExists(_Settings::file_name))
		{
			Load();
			Map();
		}
		else
		{
			Save();
		}

		LOGF_INFO("Resolution: %dx%d", static_cast<int>(width), static_cast<int>(height));
		LOGF_INFO("FPS Limit: %f", m_fps_limit);
		LOGF_INFO("Shadow resolution: %d", m_shadow_map_resolution);
		LOGF_INFO("Anisotropy: %d", m_anisotropy);
		LOGF_INFO("Max threads: %d", m_max_thread_count);

		return true;
	}

	void Settings::Save() const
	{
		//Create a settings file
		_Settings::fout.open(_Settings::file_name, ofstream::out);

		//Write the settings
		_Settings::write_setting(_Settings::fout, "bFullScreen", m_is_fullscreen);
		_Settings::write_setting(_Settings::fout, "bIsMouseVisible", m_is_mouse_visible);
		_Settings::write_setting(_Settings::fout, "fResolutionWidth", width);
		_Settings::write_setting(_Settings::fout, "fResolutionHeight",height);
		_Settings::write_setting(_Settings::fout, "iShadowMapResolution", m_shadow_map_resolution);
		_Settings::write_setting(_Settings::fout, "iAnisotropy", m_anisotropy);
		_Settings::write_setting(_Settings::fout, "fFPSLimit", m_fps_limit);
		_Settings::write_setting(_Settings::fout, "iMaxThreadCount", m_max_thread_count);

		_Settings::fout.close();
	}

	void Settings::Load()
	{
		// Create a settings file
		_Settings::fin.open(_Settings::file_name, ifstream::in);

		float resolution_x = 0;
		float resolution_y = 0;

		//Create a settings file
		_Settings::read_setting(_Settings::fin, "bFullScreen", m_is_fullscreen);
		_Settings::read_setting(_Settings::fin, "bIsMouseVisible", m_is_mouse_visible);
		_Settings::read_setting(_Settings::fin, "fResolutionWidth", resolution_x);
		_Settings::read_setting(_Settings::fin, "fResolutionHeight", resolution_y);
		_Settings::read_setting(_Settings::fin, "iShadowMapResolution", m_shadow_map_resolution);
		_Settings::read_setting(_Settings::fin, "iAnisotropy", m_anisotropy);
		_Settings::read_setting(_Settings::fin, "fFPSLimit", m_fps_limit);
		_Settings::read_setting(_Settings::fin, "iMaxThreadCount", m_max_thread_count);
		
		_Settings::fin.close();
	}

	void Settings::Reflect()
	{
		//......
	}

	void Settings::Map()
	{
		//......
	}

}