cmake_minimum_required(VERSION 3.8)

file(GLOB EditorSource ./**.cpp ./**.c)

add_executable(Editor ${EditorSource} "imgui_impl_win32.cpp")
target_link_libraries(Editor PUBLIC  Runtime)