cmake_minimum_required(VERSION 3.8)

file(GLOB Runtime_Source ${CMAKE_CURRENT_SOURCE_DIR}/Core/**.cpp ${CMAKE_CURRENT_SOURCE_DIR}/Core/**.c)

add_definitions(-DV_RUNTIME_EXPORT=1)

add_library (Runtime
	SHARED
	${Runtime_Source}
 )

set_target_properties(Runtime
  PROPERTIES
    POSITION_INDEPENDENT_CODE 1
    CXX_VISIBILITY_PRESET hidden
    VISIBILITY_INLINES_HIDDEN 1
    OUTPUT_NAME "Runtime"
    DEBUG_POSTFIX "_d"
    MACOSX_RPATH ON
    WINDOWS_EXPORT_ALL_SYMBOLS ON
  )