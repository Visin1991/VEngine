
add_executable(VLibTest "VLibTest.cpp")
target_link_libraries(VLibTest VLib)

enable_testing()
add_test( NAME VLibTest COMMAND $<TARGET_FILE:VLibTest> ) 