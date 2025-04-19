cmake_minimum_required(VERSION 3.25)
project(archiver LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(~/vcpkg/installed/x64-linux-dynamic/include)
link_directories(~/vcpkg/installed/x64-linux-dynamic/lib)

add_executable(archiver main.cpp ./src/App.cpp)

target_link_libraries(archiver PRIVATE archive)