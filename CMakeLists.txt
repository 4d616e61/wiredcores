cmake_minimum_required(VERSION 3.5.0)
project(game-server VERSION 0.1.0 LANGUAGES C CXX)



include_directories("${CMAKE_CURRENT_SOURCE_DIR}/include")
file(GLOB_RECURSE SOURCES src/*.cpp)

add_executable(game-server ${SOURCES})

