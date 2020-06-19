#pragma once

#include <memory>

#include "VEngineConfig.h"
#include "../RHI/RHI.h"


namespace V
{
	class V_API VEngine
	{
	public:
		VEngine() = default;

		bool Init(void* wc, void* hwnd);
		void Update();
		void ResizeWindow(unsigned int width, unsigned int height, void* _handle = nullptr, void* instance = nullptr);

		void Terminate();

	public:
		std::unique_ptr<RHI> unique_RHI;
	};
}