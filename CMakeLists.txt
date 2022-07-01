project (freq)
cmake_minimum_required (VERSION 3.1)

if (NOT CMAKE_BUILD_TYPE)
    message(STATUS "Use default cmake build type: Release")
    set(CMAKE_BUILD_TYPE Release)
endif()

option(FREQ_BUILD_TESTS "Build tests" OFF)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wpedantic -Wextra -Werror")

set(EXECUTABLE_OUTPUT_PATH ${OUTPUT_PATH})

file(GLOB SRC_LIST
    ./src/*.h ./src/*.cpp
    ./src/file-reader/*
    ./src/file-writer/*
    ./src/file-chunks-producer/*
    ./src/file-chunks-processor/*
    ./src/common/*
)

set(OUTPUT_PATH "${CMAKE_CURRENT_SOURCE_DIR}/lib")

set(MODULE_NAME ${PROJECT_NAME})

add_executable(${MODULE_NAME} ${SRC_LIST})

if (FREQ_BUILD_TESTS)
    add_subdirectory(tests)
    add_library(${MODULE_NAME}_lib ${SRC_LIST})
endif()

target_link_libraries(
    ${PROJECT_NAME}
)
