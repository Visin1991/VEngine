# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.8)

project ("VEngine")

# <<< General set up >>>

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

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

