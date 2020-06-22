cmake_minimum_required(VERSION 3.8)


add_definitions(-DV_EXPORT=1)

#============================================================================
# Build Library for Dear ImGUI

file(GLOB ImGui_Source
    ./ImGui/**.cpp
    ./ImGui/ImGui_Platform/**.cpp
)

add_library (ImGui
	SHARED
	${ImGui_Source}
)

#============================================================================
#Build VLib Library
file(GLOB VLib_Source
    ./Core/**.cpp
	  ./RHI/**.cpp
    ./RHI/RHI_Platform/**.cpp
)

add_library (VLib
	SHARED
	${VLib_Source}
)

#-----------------
# For windows platform, we will use DX and Native window
if(WIN32)
  target_link_libraries(VLib
    PUBLIC
    ImGui
  )
endif()

#-----------------
# For Linux platform, we will use OpenGL and GLEW,GLFW3
if(LINUX)
  find_package(OpenGL REQUIRED)
  find_package(GLEW REQUIRED)
  find_package(glfw3 REQUIRED)

  target_link_libraries(VLib
    PUBLIC
    GLEW::GLEW
    glfw
    ${OPENGL_LIBRARY}
    ImGui
  )
endif()

#======================================================
#For Windows platform we need ......



set_target_properties(VLib
  PROPERTIES
    POSITION_INDEPENDENT_CODE 1
    CXX_VISIBILITY_PRESET hidden
    VISIBILITY_INLINES_HIDDEN 1
    OUTPUT_NAME "VLib"
    DEBUG_POSTFIX "_d"
    MACOSX_RPATH ON
    WINDOWS_EXPORT_ALL_SYMBOLS ON
  )
#==================================================