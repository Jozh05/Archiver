cmake_minimum_required(VERSION 3.25)
project(archiver LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(PATH_TO_INCLUDE_DIR ~/vcpkg/installed/x64-linux-dynamic/include)
set(PATH_TO_LINK_DIR ~/vcpkg/installed/x64-linux-dynamic/lib)

include_directories(${PATH_TO_INCLUDE_DIR})
link_directories(${PATH_TO_LINK_DIR})

add_executable(archiver main.cpp ./src/Execution.cpp ./src/Logic.cpp ./src/Validation.cpp)

target_link_libraries(archiver PRIVATE archive)