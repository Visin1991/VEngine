# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.8)

project ("VEngine")

# <<< General set up >>>

if (MSVC_VERSION GREATER_EQUAL "1900")
    include(CheckCXXCompilerFlag)
    CHECK_CXX_COMPILER_FLAG("/std:c++latest" _cpp_latest_flag_supported)
    if (_cpp_latest_flag_supported)
        add_compile_options("/std:c++latest")
    endif()
endif()

#CMAKE_INSTALL_PREFIX   : by default :/usr/local on Unix and c:\Program Files on Windows
message(STATUS "Project will be installed to ${CMAKE_INSTALL_PREFIX}")

add_definitions(-DUNICODE -D_UNICODE)

# Just simply copy all Library and Runtime Out put to CMAKE_BINARY_DIR
set (CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})


add_subdirectory("ImGui")

#Set the Header and libraries files for our project
include_directories(${VEngine_SOURCE_DIR}/ImGui)
link_directories(${VEngine_SOURCE_DIR}/ImGui)

add_subdirectory ("ImGui_Test")
add_subdirectory("DX12_Test")
add_subdirectory("DX11_Test")

add_subdirectory("VLib")
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/VLib)
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/VLib)


add_subdirectory("V")



