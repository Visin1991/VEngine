cmake_minimum_required(VERSION 3.8)

file(GLOB VEngineSource 
	./Windows/**.cpp
	./**.cpp 
	)

# add_executable(Editor WIN32 ${EditorSource} )
add_executable(V ${VEngineSource})

target_link_libraries(V
	PUBLIC
	VLib
)
