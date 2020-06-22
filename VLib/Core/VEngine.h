#pragma once

#include <memory>

#include "VEngineConfig.h"
#include "VIOHI.h"
#include "../RHI/RHI.h"


namespace V
{
	class V_API VEngine
	{
	public:
		VEngine();
		bool Init();
		void Update();
		void ResizeWindow(unsigned int width, unsigned int height, void* _handle = nullptr, void* instance = nullptr);

		void Terminate();

	public:
		std::unique_ptr<VIOHI> unique_IOHI;
		std::unique_ptr<RHI> unique_RHI;

	private:
		static inline VEngine* g_Engine;
	public:
		static VEngine* GetEngine();
	};
}