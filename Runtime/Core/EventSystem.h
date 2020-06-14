#pragma once

#include <functional>
#include <map>
#include <vector>

#include "Variant.h"

enum Event_Type
{
	Event_Frame_End,		
	Event_World_Save,		
	Event_World_Saved,		
	Event_World_Load,		
	Event_World_Loaded,		
	Event_World_Unload,		
	Event_World_Resolve,	
	Event_World_Submit,		
	Event_World_Stop,		
	Event_World_Start		
};


namespace V
{
	using subscriber = std::function<void(const Variant&)>;

	class V_API EventSystem
	{
	public:
		static EventSystem& Get()
		{
			static EventSystem instance;
			return instance;
		}

		void Subscribe(const Event_Type event_id, subscriber&& function)
		{
			m_subscribers[event_id].push_back(std::forward<subscriber>(function));
		}

		void Unsubscribe(const Event_Type event_id, subscriber&& function)
		{
			const size_t function_adress = *reinterpret_cast<long*>(reinterpret_cast<char*>(&function));
			auto& subscribers = m_subscribers[event_id];

			for (auto it = subscribers.begin(); it != subscribers.end();)
			{
				const size_t subscriber_address = *reinterpret_cast<long*>(reinterpret_cast<char*>(&(*it)));
				if (subscriber_address == function_adress)
				{
					it = subscribers.erase(it);
					return;
				}
			}
		}

		void Fire(const Event_Type event_id, const Variant& data = 0)
		{
			//if find at the end, then it means the event is not exist at all
			if (m_subscribers.find(event_id) == m_subscribers.end())
				return;
			
			for (const auto& subscriber : m_subscribers[event_id])
			{
				subscriber(data);
			}
		}

		void Clear()
		{
			m_subscribers.clear();
		}


	private:
		std::map<Event_Type, std::vector<subscriber>> m_subscribers;
	};

}