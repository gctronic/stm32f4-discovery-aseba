cmake_minimum_required(VERSION 2.6)
project(xemreader)
set(CMAKE_BUILD_TYPE Debug)


include_directories(
    ./tests/
    )
list( APPEND CMAKE_CXX_FLAGS "-DTESTS -std=c++0x -Wall -Wextra")
list( APPEND CMAKE_C_FLAGS "-DTESTS -Wall -Wextra -std=gnu99")

file(GLOB_RECURSE tests tests/*.cpp)

add_executable(
    tests
    ${tests}
    # ${crc}
    )

target_link_libraries(
    tests
    m
    CppUTest
    CppUTestExt
    )

# Run unit tests
add_custom_target(check ./tests -c DEPENDS tests)
