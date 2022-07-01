project (freqTests)
cmake_minimum_required (VERSION 3.1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wpedantic -Wextra -Werror")

find_package(GTest REQUIRED)

set(EXECUTABLE_OUTPUT_PATH ${OUTPUT_PATH})

add_definitions(-DGTEST_HAS_PTHREAD=1)
add_definitions(-D_TURN_OFF_PLATFORM_STRING)

file(GLOB SRC_LIST
    *.h *.cpp
)

link_directories(
    ./../lib
)

add_executable(${PROJECT_NAME} ${SRC_LIST})

target_link_libraries(
    ${PROJECT_NAME}
    ${GTEST_BOTH_LIBRARIES}
    freq_lib
)