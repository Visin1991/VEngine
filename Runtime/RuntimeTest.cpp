#include <memory>
#include <iostream>
#include <assert.h>

#include "Math/Vector2.h"
#include "Math/Vector3.h"
#include "Math/Vector4.h"

#include "Core/Log.h"
#include "Core/FileSystem.h"
#include "Core/Engine.h"
#include "Core/Timer.h"


#include "Resources/IResources.h"

#include "RHI/RHI.h"



using namespace V;
using namespace V::Math;

int main()
{
	//Test For Math Library
	Vector2 pos1 = Vector2(0.5f, 0.6f);
	Vector2 pos2 = Vector2(0.3f, 0.6f);
	assert(pos1 * pos2 == Vector2(0.15f, 0.36f));
	assert(pos1 + pos2 == Vector2(0.8f, 1.2f));

	pos1 += pos2;
	assert(pos1 == Vector2(0.8f, 1.2f));






	//Test For Log System

	//Test For FileSystem

	auto engine = std::make_unique<Engine>(nullptr, nullptr,nullptr, 512.0, 512.0);

	auto context = engine->GetContext();


	for (int i = 0; i < 10; i++)
	{
		engine->Tick();
		std::cout << context->GetSubsystem<Timer>()->GetDeltaTimeSec() << std::endl;
	}


	system("pause");

	return 0;
}