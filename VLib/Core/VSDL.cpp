#include "VSDL2.h"

#include <SDL2/SDL.h>
#include <GL/glew.h>


namespace V
{
    int VSDL2::Init()
    {
        if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER)!=0)
        {
            printf("Error: s%\n",SDL_GetError());
            return -1;
        }

        //GL 3.0 + GLSL 130
        const char* glsl_version = "#version 130";
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS,0);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION,3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

        //Create window with graphcs context
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER,1);
        SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
        SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
        SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
        SDL_Window* window = SDL_CreateWindow("VEngine",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,1280,720,window_flags);
        SDL_GLContext gl_context = SDL_GL_CreateContext(window);


        SDL_GL_MakeCurrent(window,gl_context);
        SDL_GL_SetSwapInterval(1);

        bool err = glewInit() != GLEW_OK;

        if(err)
        {
            fprintf(stderr, "Failed to initialize OpenGL loader!\n");
            return -1;
        }

        return 1;
    }
    
} // namespace V

