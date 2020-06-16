cmake_minimum_required(VERSION 3.8)

file(GLOB EditorSource ./**.cpp ./**.c)

# add_executable(Editor WIN32 ${EditorSource} )
add_executable(Editor ${EditorSource} )

target_link_libraries(Editor
	PUBLIC
	ImGui
)
