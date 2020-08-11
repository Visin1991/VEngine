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
#-----------------
# For Linux platform, we will use OpenGL and GLEW,GLFW3
if(LINUX OR FORCEGL)

  target_link_libraries(ImGui
    PUBLIC
    GLEW::GLEW
    ${SDL2_LIBRARIES}
    ${OPENGL_LIBRARY} 
  )
  if(WIN32)
    target_link_libraries(ImGui
    PUBLIC
    SDL2::SDL2 
    SDL2::SDL2main
    )
  endif()

endif()
#============================================================================
#Build VLib Library
file(GLOB VLib_Source
    ./Core/**.cpp
	  ./RHI/**.cpp
    ./RHI/RHI_Platform/**.cpp
    ./HardwareRenderer/OpenGL/**.cpp
)

add_library (VLib
	SHARED
	${VLib_Source}
)

target_link_libraries(VLib
  PUBLIC
  ImGui
)
#======================================================
#For Windows platform we need ......

if(WIN32)
  set_target_properties(ImGui
    PROPERTIES
      POSITION_INDEPENDENT_CODE 1
      CXX_VISIBILITY_PRESET hidden
      VISIBILITY_INLINES_HIDDEN 1
      OUTPUT_NAME "ImGui"
      DEBUG_POSTFIX "_d"
      MACOSX_RPATH ON
      WINDOWS_EXPORT_ALL_SYMBOLS ON
    )

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
endif(WIN32)
#==================================================