
#ifdef _WIN32


#include "Editor.h"


int main(int argc, char** argv)
{

    //================================================================================================
    //Process the commandline argument,
    
    //================================================================================================
    //Editor ---- right now I just hard code it
    bool runEditor = true;
    if (runEditor)
    {
        Editor editor;
        editor.Init();
    }

    //================================================================================================
    return 0;
}

#endif