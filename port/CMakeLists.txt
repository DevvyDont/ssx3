cmake_minimum_required(VERSION 3.24)

# The decompiled code reads struct fields at fixed 32-bit offsets and keeps
# EE addresses as pointers, so the port is a 32-bit program. Set the flags
# before project() so SDL and every test are built the same way.
if(NOT WIN32)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -m32" CACHE STRING "" FORCE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -m32" CACHE STRING "" FORCE)
endif()

project(ssx3pc C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

if(NOT CMAKE_SIZEOF_VOID_P EQUAL 4)
    message(FATAL_ERROR "The SSX 3 port must be built as 32-bit. On Windows, "
                        "configure with -A Win32.")
endif()

option(SSX3PC_SYSTEM_SDL "Use an installed 32-bit SDL3 instead of fetching it" OFF)
if(SSX3PC_SYSTEM_SDL)
    find_package(SDL3 REQUIRED CONFIG)
else()
    include(FetchContent)
    set(SDL_SHARED OFF CACHE BOOL "" FORCE)
    set(SDL_STATIC ON CACHE BOOL "" FORCE)
    set(SDL_TEST_LIBRARY OFF CACHE BOOL "" FORCE)
    FetchContent_Declare(SDL3
        GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
        GIT_TAG release-3.2.8
        GIT_SHALLOW TRUE)
    FetchContent_MakeAvailable(SDL3)
endif()

if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    set(SSX3PC_WARNINGS -Wall -Wextra)
endif()

add_library(ps2os STATIC
    src/ps2os/eemem.c
    src/ps2os/kernel.c
    src/platform/ds2pad.c)
target_include_directories(ps2os PUBLIC src)
target_compile_options(ps2os PRIVATE ${SSX3PC_WARNINGS})
target_link_libraries(ps2os PUBLIC SDL3::SDL3)

add_executable(ssx3pc
    src/main.c
    src/platform/sdlpad.c)
target_compile_options(ssx3pc PRIVATE ${SSX3PC_WARNINGS})
target_link_libraries(ssx3pc PRIVATE ps2os)

include(CTest)
if(BUILD_TESTING)
    foreach(t eemem kernel ds2pad)
        add_executable(test_${t} tests/test_${t}.c)
        target_compile_options(test_${t} PRIVATE ${SSX3PC_WARNINGS})
        target_link_libraries(test_${t} PRIVATE ps2os)
        add_test(NAME ${t} COMMAND test_${t})
    endforeach()

    add_test(NAME host_headless COMMAND ssx3pc --headless --frames 120)

    # Loads the retail executable when one is supplied:
    #   cmake -DSSX3PC_ELF=/path/to/SLUS_207.72
    set(SSX3PC_ELF "" CACHE FILEPATH "Path to the retail SLUS_207.72, for the load test")
    if(SSX3PC_ELF)
        add_test(NAME load_retail_elf COMMAND test_eemem ${SSX3PC_ELF})
    endif()
endif()
