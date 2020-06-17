#include <iostream>
#include <string>
#include <VLibGlobal.h>

#include <VEngine.h>
std::unique_ptr<V::VEngine> g_engine;

int main()
{
	g_engine = std::make_unique<V::VEngine>();
	g_engine->InitRHI();


	std::string info = "Haha this is a test program";
	V::SetGlobalString(info);

	system("pause");
	return 0;
}