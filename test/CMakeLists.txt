cmake_minimum_required(VERSION 3.5)

set(PROJECT_NAME AllocationTracerTestApp1)
set(EXECUTABLE_NAME AllocationTracerTestApp1)

project(${PROJECT_NAME} CXX)

set(CMAKE_CXX_STANDARD 14)

set(SOURCE_FILES
	app.cpp)

add_executable(${EXECUTABLE_NAME} ${SOURCE_FILES})

target_compile_options(${EXECUTABLE_NAME} PRIVATE -Wall -Wextra -pedantic -Werror -pedantic-errors -g -O0)

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    target_compile_options(${EXECUTABLE_NAME} PRIVATE -Wno-invalid-source-encoding)
endif()

install(TARGETS ${EXECUTABLE_NAME} DESTINATION bin)
