# source files
file(GLOB_RECURSE SOURCES *.cpp)
add_executable(demo ${SOURCES})

# random library
file(GLOB ${PROJECT_SOURCE_DIR}/demo RANDLIB libRandom.a)

target_link_libraries(demo LMPA
                            ${RANDLIB}
        )