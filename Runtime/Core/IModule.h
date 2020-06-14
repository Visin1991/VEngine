#pragma once

#include "EngineDefs.h"

namespace V
{
	class Context;

	class V_API IModule
	{
	public:
		IModule(Context* context)
		{
			m_context = context;
		}

		virtual ~IModule() = default;
		virtual bool Initialize() { return true; }
		virtual void Tick(float delta_time) {}

	protected:
		Context* m_context;
	};

}