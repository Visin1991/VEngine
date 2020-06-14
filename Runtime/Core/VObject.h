#include "EngineDefs.h"

#include <stdint.h>

namespace V
{
	static uint32_t g_id = 0;

	class V_API VObject
	{
	public:
		VObject() { m_id = GenerateID(); }


		const uint32_t GetID() const { return m_id; }
		void SetId(const uint32_t id) { m_id = id; }

		static uint32_t GenerateID() { return ++g_id; }


	protected:
		uint64_t m_size = 0;
		uint32_t m_id = 0;
	};

}