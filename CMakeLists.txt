cmake_minimum_required(VERSION 3.5)

set(PROJECT_NAME AllocationTracer)
set(EXECUTABLE_NAME AllocationTracer)

project(${PROJECT_NAME} CXX)

set(CMAKE_CXX_STANDARD 14)

set(SOURCE_FILES
	liballocationtracer.cpp)

add_library(${EXECUTABLE_NAME} SHARED ${SOURCE_FILES})

target_compile_options(${EXECUTABLE_NAME} PRIVATE -Wall -Wextra -pedantic -Werror -pedantic-errors  -g -O0)

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    target_compile_options(${EXECUTABLE_NAME} PRIVATE -Wno-invalid-source-encoding)
endif()

target_link_libraries(${EXECUTABLE_NAME} dl)

add_subdirectory(test)

install(TARGETS ${EXECUTABLE_NAME} DESTINATION bin)
