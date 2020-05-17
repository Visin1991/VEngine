cmake_minimum_required (VERSION 3.8)

file(GLOB DX11_TEST_SOURCE ./**.cpp ./**.c)

add_executable(DX11_Test ${DX11_TEST_SOURCE})

target_link_libraries(DX11_Test
	PUBLIC
	ImGui
)