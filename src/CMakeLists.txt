cmake_minimum_required(VERSION 3.13.4)

if (GBA_SINGLE_FILE)
    add_library(TotalGBA single.c)

    target_compile_definitions(TotalGBA PRIVATE GBA_SINGLE_FILE=1)
else()
    add_library(TotalGBA
        gba.c
        # mem
        mem/mmio.c
        mem/reads/external_memory.c
        mem/reads/general_internal_memory.c
        mem/reads/internal_display_memory.c
        mem/writes/external_memory.c
        mem/writes/general_internal_memory.c
        mem/writes/internal_display_memory.c
        # util
        util/string.c
        util/bit.c
        # arm
        arm7tdmi/arm7tdmi.c
        arm7tdmi/barrel_shifter.c
        arm7tdmi/arm/arm.c
        arm7tdmi/arm/block_data.c
        arm7tdmi/arm/branch.c
        arm7tdmi/arm/data_processing.c
        arm7tdmi/arm/halfword_and_signed_data.c
        arm7tdmi/arm/psr_transfer.c
        arm7tdmi/arm/single_data.c
        arm7tdmi/thumb/thumb.c
        arm7tdmi/thumb/instructions.c
    )

    target_compile_definitions(TotalGBA PRIVATE GBA_SINGLE_FILE=0)
endif()

target_include_directories(TotalGBA PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_features(TotalGBA PRIVATE c_std_99)

if (GBA_DEV)
    list(APPEND _flags
        -Wall
        -Wextra
        -Werror
        -Wfatal-errors
        # will enable near cpu being finished, as then ill change
        # all `case 0 ... 10:` to expand out.
        # -Wpedantic
        -Wshadow
        -Wdouble-promotion
        -Wformat=2
        -Wformat-overflow=2
        -Wformat-truncation=1
        -Wundef
        -Wmissing-include-dirs
        -Wstrict-aliasing=2
        -Walloca
        -Wduplicated-branches
        -Wduplicated-cond
        -Wwrite-strings
        -Wdate-time
        -Wsizeof-pointer-memaccess
        -Wlogical-op
        -Wpacked
        -Wredundant-decls
        -Wnested-externs
        # -Winline
        -ftrapv
    )

    list(APPEND _sanitizers
        -fsanitize=address
        -fsanitize=leak
        -fsanitize=undefined
        -fsanitize=pointer-compare
        -fsanitize=pointer-subtract
        -fstack-protector-all
    )

    # include(CheckCCompilerFlag)

    # TODO: check_c_compiler_flag() seems to not work on all flags
    # for some reason. example -Werror fails, even though its valid for gcc...
    
    foreach (flag IN LISTS _flags)
        # check_c_compiler_flag(${flag} CHECK_FLAG_${flag})
        # if (CHECK_FLAG_${flag})
            target_compile_options(TotalGBA PRIVATE ${flag})
        # endif()
    endforeach()

    foreach (flag IN LISTS _sanitizers)
        # check_c_compiler_flag(${flag} CHECK_FLAG_${flag})
        # if (CHECK_FLAG_${flag})
            target_compile_options(TotalGBA PRIVATE ${flag})
            target_link_options(TotalGBA INTERFACE ${flag})
        # endif()
    endforeach()

endif()

if (GBA_DEBUG)
    target_compile_definitions(TotalGBA PRIVATE GBA_DEBUG=1)
endif()
