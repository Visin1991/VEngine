cmake_minimum_required(VERSION 3.8)

file(GLOB VLib_Source
	./**.cpp
	./RHI/**.cpp
    ./RHI/RHI_Platform/**.cpp
    ./ImGui/**.cpp
    ./ImGui/ImGui_Platform/**.cpp
)

add_definitions(-DV_EXPORT=1)


add_library (VLib
	SHARED
	${VLib_Source}
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


#add current directory; So our TestDir can find the header of VLib
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
add_subdirectory("TestDir")