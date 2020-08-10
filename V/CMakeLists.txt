cmake_minimum_required(VERSION 3.8)

include_directories(${SDL2_INCLUDE_DIRS})


file(GLOB VEngineSource 
	./Windows/**.cpp
	./**.cpp 
	)


# add_executable(Editor WIN32 ${EditorSource} )
add_executable(VEngine ${VEngineSource})
target_link_libraries(VEngine
	PUBLIC	
	VLib
)
