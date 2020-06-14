#pragma once

#include "EngineDefs.h"
#include "IModule.h"
#include "Log.h"


namespace V
{
	class Engine;

	template<typename T>
	constexpr void validate_subsystem_type() {
		static_assert(std::is_base_of<IModule, T>::value, "The Type is not a Module");
	}

	enum Tick_Group
	{
		Tick_Variable,
		Tick_Smoothed
	};

	struct _subsystem
	{	
		_subsystem(const std::shared_ptr<IModule>& subsystem, Tick_Group tick_group)
		{
			ptr = subsystem;
			this->tick_group = tick_group;
		}

		std::shared_ptr<IModule> ptr;
		Tick_Group tick_group;
	};

	class V_API Context
	{
	public:
		Context() = default;

		~Context()
		{
			for (size_t i = m_subsystems.size() - 1; i > 0; i--)
			{
				m_subsystems[i].ptr.reset();
			}
			m_subsystems.clear();
		}

		template<class T>
		void RegisterSubsystem(Tick_Group tick_group = Tick_Variable)
		{
			validate_subsystem_type<T>();
			m_subsystems.emplace_back(std::make_shared<T>(this), tick_group);
		}

		bool Initialize()
		{
			auto result = true;
			for (const auto& subsystem : m_subsystems)
			{
				if (!subsystem.ptr->Initialize())
				{
					LOGF_ERROR("Failed to initialize %s", typeid(*subsystem.ptr).name());
					result = false;
				}
			}
			return result;
		}

		//Tick
		void Tick(Tick_Group tick_group, float delta_time = 0.0f)
		{
			for (const auto& subsystem : m_subsystems)
			{
				if (subsystem.tick_group != tick_group)
				{
					continue;
				}
				subsystem.ptr->Tick(delta_time);
			}
		}

		template<class T>
		std::shared_ptr<T> GetSubsystem()
		{
			validate_subsystem_type<T>();

			for (const auto& subsystem : m_subsystems)
			{
				if (typeid(T) == typeid(*subsystem.ptr))
					return std::static_pointer_cast<T>(subsystem.ptr);
			}
			return nullptr;
		}

		Engine* m_engine = nullptr;


	private:
		std::vector<_subsystem> m_subsystems;
	};


}