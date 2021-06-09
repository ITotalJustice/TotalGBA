cmake_minimum_required(VERSION 3.13.4)

if (GBA_TEST_ROMRUNNER)
    add_executable(test_romrunner test_romrunner.c)
    target_link_libraries(test_romrunner LINK_PRIVATE TotalGBA)
endif()
