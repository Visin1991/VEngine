#pragma once

#include <stdint.h>
#include <string>

#include "EngineDefs.h"


namespace V
{
	class Context;

	static uint32_t g_id = 0;

	class V_API VObject
	{
	public:
		VObject(Context* context = nullptr)
		{
			m_context = context;
			m_id = GenerateId();
		}


		const std::string& GetName()    const { return m_name; }

		// Id
		const uint32_t GetId()          const { return m_id; }
		void SetId(const uint32_t id) { m_id = id; }
		static uint32_t GenerateId() { return ++g_id; }

		// CPU & GPU sizes
		const uint64_t GetSizeCpu()     const { return m_size_cpu; }
		const uint64_t GetSizeGpu()     const { return m_size_gpu; }

		// Execution context.
		Context* GetContext()           const { return m_context; }


	protected:
		// Execution context
		Context* m_context = nullptr;

		std::string m_name;
		uint32_t m_id = 0;
		uint64_t m_size_cpu = 0;
		uint64_t m_size_gpu = 0;
	};

}