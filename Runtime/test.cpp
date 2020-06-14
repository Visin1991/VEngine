#include <memory>

#include "Core/Log.h"
#include "Core/FileSystem.h"
#include "Core/Engine.h"


using namespace V;

int main()
{
	//Test For Log System

	//Test For FileSystem

	auto engine = std::make_unique<Engine>(nullptr, nullptr,nullptr, 512.0, 512.0);

	auto context = engine->GetContext();


	for (int i = 0; i < 1000000; i++)
	{
		engine->Tick();
	}



	return 0;
}