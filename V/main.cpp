#include <string>

#include "VEngineLoop.h"
#include <VLibGlobal.h>



int main(int argc, char** argv)
{
    std::string info = "Hi ! call from the Engine Programing";
    V::SetGlobalString(info);

    //================================================================================================
    //Process the commandline argument,
    
    //================================================================================================
    //Editor ---- right now I just hard code it
    bool runEditor = true;
    if (runEditor)
    {
        VEngineLoop editor;
        editor.Init();
    }

    //================================================================================================
    return 0;
}
