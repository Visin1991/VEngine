# CMakeList.txt : CMake project for VEngine, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

file(GLOB DX12_Test_Source ./**.cpp ./**.c)

set(STB_INCLUDE_DIRS C:/Users/Visin/vcpkg/installed/x64-windows/include)

# Add source to this project's executable.
add_executable (DX12_Test ${DX12_Test_Source})


target_include_directories(DX12_Test 
	PUBLIC
	${STB_INCLUDE_DIRS}
)

target_link_libraries(DX12_Test PUBLIC
	ImGui
)

