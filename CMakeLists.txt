# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.8)

project ("VEngine")

# cmake toolchain
if(CMAKE_TOOLCHAIN_FILE)
    include(${CMAKE_TOOLCHAIN_FILE})
    message("-------------------VCPKG Path---------------------")
    message(${CMAKE_TOOLCHAIN_FILE})
endif(CMAKE_TOOLCHAIN_FILE)

option(FORCEGL "Force to use OpenGL" OFF)

if(FORCEGL)
    message("Force to Use OpenGL Add Definitions")
    add_definitions(-DFORCEGL=1)
endif()


# <<< General set up >>>
set (CMAKE_CXX_STANDARD 17)

if (MSVC_VERSION GREATER_EQUAL "1900")
    include(CheckCXXCompilerFlag)
    CHECK_CXX_COMPILER_FLAG("/std:c++latest" _cpp_latest_flag_supported)
    if (_cpp_latest_flag_supported)
        add_compile_options("/std:c++latest")
    endif()
endif()

add_definitions(-DUNICODE -D_UNICODE)


if(UNIX AND NOT APPLE)
    set(LINUX TRUE)
endif()


# For Linux platform, or Force OpenGL we need to find Those Library
# Because we will both use those lib in VLib and VEngine. So We set those library as global variable.
if(LINUX OR FORCEGL)
  find_package(OpenGL REQUIRED)
  find_package(GLEW REQUIRED)
  find_package(SDL2 CONFIG REQUIRED)
endif()

# Just simply copy all Library and Runtime Out put to CMAKE_BINARY_DIR
set (CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

add_subdirectory("VLib")
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/VLib)
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/VLib)


add_subdirectory("V")
