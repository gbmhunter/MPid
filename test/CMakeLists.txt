enable_testing()
find_package (Threads)

# The main header files do not actually have to be added here, but
# this helps CLion recognize the header files as being part of a
# project and allows auto-complete to work correctly.
file(GLOB_RECURSE MPid_HEADERS
        "${CMAKE_SOURCE_DIR}/include/*.hpp")

file(GLOB_RECURSE MPidTests_SRC
        "*.cpp"
        "*.hpp"
        )

add_executable (MPidTests ${MPid_HEADERS} ${MPidTests_SRC})
add_dependencies (MPidTests MUnitTest_Project)

if(COVERAGE)
    set_target_properties(MPidTests PROPERTIES COMPILE_FLAGS "-g -O0 -fprofile-arcs -ftest-coverage")
    set_target_properties(MPidTests PROPERTIES LINK_FLAGS "-coverage -lgcov")
endif ()

target_link_libraries(MPidTests LINK_PUBLIC MUnitTest ${CMAKE_THREAD_LIBS_INIT})

add_custom_target(
    run_unit_tests ALL
    DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/MPidTests.touch MPidTests)

add_custom_command(         
	OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/MPidTests.touch
    COMMAND ${CMAKE_CURRENT_BINARY_DIR}/MPidTests)

# Target name, executable that runs unit tests, output directory
if(COVERAGE)
    setup_target_for_coverage(${PROJECT_NAME}_coverage MPidTests coverage)
endif()