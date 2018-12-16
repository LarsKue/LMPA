# source files
file(GLOB_RECURSE SOURCES *.cpp)
add_executable(demo ${SOURCES})
target_link_libraries(demo LMPA)