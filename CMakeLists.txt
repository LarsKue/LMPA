cmake_minimum_required(VERSION 3.13)
project(LMPA_LIBRARY)

if (CMAKE_BUILD_TYPE EQUAL "DEBUG")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -std=c++11 -m64 -Wall -Wextra -pedantic -Wshadow")
    set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++")
elseif(CMAKE_BUILD_TYPE EQUAL "RELEASE")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -std=c++11 -m64")
    set(CMAKE_EXE_LINKER_FLAGS "static-libgcc -static-libstdc++")
endif(CMAKE_BUILD_TYPE EQUAL "DEBUG")

add_subdirectory(demo)
add_subdirectory(LMPA)