cmake_minimum_required(VERSION 3.13)
project(LMPA_LIBRARY)

# for 32-bit compilers, use -m32 instead of -m64 (search and replace)
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -std=c++11 -m64 -Wall -Wextra -Wshadow -pedantic -Weffc++")
    set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -std=c++11 -m64")
    set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++")
endif()

add_subdirectory(demo)
add_subdirectory(LMPA)