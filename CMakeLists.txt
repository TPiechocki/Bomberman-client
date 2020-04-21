cmake_minimum_required(VERSION 3.16)
project(Bomberman_client C)

set(CMAKE_C_STANDARD 11)

file(GLOB_RECURSE SOURCES src/*.c)

add_executable(${PROJECT_NAME} ${SOURCES} src/board.h src/board.c)

SET(GCC_COVERAGE_COMPILE_FLAGS "-I/usr/include/ -pthread")
SET(GCC_COVERAGE_LINK_FLAGS    "-L/usr/lib -lSDL2 -lSDL2_image")

SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}")
SET(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} ${GCC_COVERAGE_LINK_FLAGS}")