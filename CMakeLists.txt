cmake_minimum_required(VERSION 3.5)

set(PROJECT_NAME AllocationTracer)
set(EXECUTABLE_NAME AllocationTracer)

project(${PROJECT_NAME} CXX)

find_package(Libunwind PATHS .)

set(CMAKE_CXX_STANDARD 14)

set(SOURCE_FILES
	src/liballocationtracer.cpp
	src/Originals.cpp
	src/Context.cpp
	src/TraceGuard.cpp
	src/FindAllocatedType.cpp)

add_library(${EXECUTABLE_NAME} SHARED ${SOURCE_FILES})

target_include_directories(${EXECUTABLE_NAME} PRIVATE include)

target_compile_options(${EXECUTABLE_NAME} PRIVATE -Wall -Wextra -pedantic -pedantic-errors -Wno-noexcept-type -g -O0)

target_link_libraries(${EXECUTABLE_NAME} dl unwind)

add_subdirectory(test/base)
add_subdirectory(test/stl)

install(TARGETS ${EXECUTABLE_NAME} DESTINATION bin)
