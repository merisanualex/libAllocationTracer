cmake_minimum_required(VERSION 3.5)

set(PROJECT_NAME BaseTest)
set(EXECUTABLE_NAME BaseTest)

project(${PROJECT_NAME} CXX)

set(CMAKE_CXX_STANDARD 14)

set(SOURCE_FILES
	base.cpp)

add_executable(${EXECUTABLE_NAME} ${SOURCE_FILES})

target_compile_options(${EXECUTABLE_NAME} PRIVATE -Wall -Wextra -pedantic -Werror -pedantic-errors -g -O0)

install(TARGETS ${EXECUTABLE_NAME} DESTINATION bin)
