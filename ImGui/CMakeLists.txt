cmake_minimum_required(VERSION 3.8)

file(GLOB ImGui_Source ./**.cpp ./**.c)

add_library (ImGui
	SHARED
	${ImGui_Source}
 )

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