#include <string>
#include "VEngineLoop.h"

int main(int argc, char** argv)
{
    //================================================================================================
    //Process the commandline argument,
    #ifdef FORCEGL
        printf("Force OpenGL Graphics");
    #else
        printf("Not Force OpenGL");
    #endif

    //================================================================================================
    VEngineLoop engineLoop;
    engineLoop.Run();
    //================================================================================================
    return 0;
}
