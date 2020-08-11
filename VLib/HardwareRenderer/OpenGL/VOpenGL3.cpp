#include "VOpenGL3.h"

#include <GL/glew.h>

namespace V
{

    // OpenGL Data
    static GLuint       g_GlVersion = 0;                // Extracted at runtime using GL_MAJOR_VERSION, GL_MINOR_VERSION queries (e.g. 320 for GL 3.2)
    static char         g_GlslVersionString[32] = "";   // Specified by user or detected based on compile time GL settings.
    static GLuint       g_FontTexture = 0;
    static GLuint       g_ShaderHandle = 0, g_VertHandle = 0, g_FragHandle = 0;
    static GLint        g_AttribLocationTex = 0, g_AttribLocationProjMtx = 0;                                // Uniforms location
    static GLuint       g_AttribLocationVtxPos = 0, g_AttribLocationVtxUV = 0, g_AttribLocationVtxColor = 0; // Vertex attributes location
    static unsigned int g_VboHandle = 0, g_ElementsHandle = 0;


    bool OpenGL3_Init(const char* glsl_version)
    {
        // Query for GL version (e.g. 320 for GL 3.2)
        GLint major, minor;
        glGetIntegerv(GL_MAJOR_VERSION, &major);
        glGetIntegerv(GL_MINOR_VERSION, &minor);
        g_GlVersion = (GLuint)(major * 100 + minor * 10);

        // Setup back-end capabilities flags
        ImGuiIO& io = ImGui::GetIO();
        io.BackendRendererName = "imgui_impl_opengl3";

        if (glsl_version == NULL)
        glsl_version = "#version 130";

        IM_ASSERT((int)strlen(glsl_version) + 2 < IM_ARRAYSIZE(g_GlslVersionString));
        strcpy(g_GlslVersionString, glsl_version);
        strcat(g_GlslVersionString, "\n");


        const char* gl_loader = "GLEW";

        // Make a dummy GL call (we don't actually need the result)
        // IF YOU GET A CRASH HERE: it probably means that you haven't initialized the OpenGL function loader used by this code.
        // Desktop OpenGL 3/4 need a function loader. See the IMGUI_IMPL_OPENGL_LOADER_xxx explanation above.
        GLint current_texture;
        glGetIntegerv(GL_TEXTURE_BINDING_2D, &current_texture);

        return true;
    }

    //..............More

}