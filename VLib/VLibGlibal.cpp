#include "VLibGlobal.h"
#include <iostream>

namespace V
{
	void SetGlobalString(std::string& info)
	{
		std::cout << info << std::endl;
	}
}