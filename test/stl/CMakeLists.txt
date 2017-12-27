cmake_minimum_required(VERSION 3.5)

set(PROJECT_NAME STLTest)
set(EXECUTABLE_NAME STLTest)

project(${PROJECT_NAME} CXX)

set(CMAKE_CXX_STANDARD 14)

set(SOURCE_FILES
	stl.cpp)

add_executable(${EXECUTABLE_NAME} ${SOURCE_FILES})

target_compile_options(${EXECUTABLE_NAME} PRIVATE -Wall -Wextra -pedantic -Werror -pedantic-errors -g -O3)

install(TARGETS ${EXECUTABLE_NAME} DESTINATION bin)
