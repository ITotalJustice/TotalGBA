cmake_minimum_required(VERSION 3.13.4)


project(TotalGBA
    VERSION 0.0.1
    DESCRIPTION "WIP GBA emulator written in c99!"
    HOMEPAGE_URL "https://github.com/ITotalJustice/TotalGBA"
    LANGUAGES C
)

option(GBA_SINGLE_FILE "include all src in single.c" OFF)
option(GBA_DEBUG "enable debug" OFF)
option(GBA_DEV "enables debug and sanitizers" OFF)

option(GBA_EXAMPLE_ALL "builds all examples" OFF)

option(GBA_TEST_ROMRUNNER "" OFF)
# option(GBA_TEST_SANITY "build test that ensures the lib can be built" OFF)
# option(GBA_TEST_BENCH "" OFF)
option(GBA_TEST_ALL "build all tests" OFF)


if (GBA_EXAMPLE_ALL)
endif()

if (GBA_DEV)
    set(GBA_DEBUG ON)
endif()

if (GBA_TEST_ALL)
    set(GBA_TEST_ROMRUNNER ON)
endif()

add_subdirectory(src)
add_subdirectory(tests)
# add_subdirectory(examples)
