cmake_minimum_required(VERSION 3.15.0)

PROJECT(trekker LANGUAGES CXX C)

# Using C++17 standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(BUILD_SHARED_LIBS "Build shared libraries" ON)

set(CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}/install")

include_directories(${CMAKE_SOURCE_DIR}/src)
include_directories(${CMAKE_INCLUDE_PATH})
link_directories(${CMAKE_LIBRARY_PATH})

# List source files
file(GLOB_RECURSE SRCS CONFIGURE_DEPENDS RELATIVE ${CMAKE_SOURCE_DIR} "src/*.c*")
list(REMOVE_ITEM SRCS "main.cpp")

# Set executable
set(CMDNAME trekker)
add_executable(${CMDNAME} ${CMAKE_SOURCE_DIR}/src/main.cpp ${SRCS})
target_link_libraries(${CMDNAME} Nibrary geogram z)

find_package(OpenMP REQUIRED)
if(OPENMP_FOUND)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif()

if(UNIX)

    message(STATUS "Building for Unix")

    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -march=native")

    if(CMAKE_BUILD_TYPE MATCHES Debug)
        add_compile_options(-fsanitize=address)
        add_link_options(-fsanitize=address)
    endif()

    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")

        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -W -Wno-long-long -Wno-stringop-truncation -Wno-dev -Wno-unknown-pragmas -Wno-unused-result -Wall -Wextra -pedantic -pedantic-errors")

        # Thread support
        find_package (Threads)
        target_link_libraries(${CMDNAME} Threads::Threads)

    elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang")

        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -w")
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -w")

    endif()

    if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS "9")
        target_link_libraries(${CMDNAME} stdc++fs)
    endif()

elseif(MSVC)

    message(STATUS "Building for Windows")

    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++17 /permissive- /bigobj")

    # For definitions
    add_compile_definitions(BUILD_FOR_WINDOWS _USE_MATH_DEFINES _WIN32 WIN32 _WIN64 WIN64)

    # Disable some compiler warnings
    set(DISABLED_WARNINGS "/wd4244 /wd4267 /wd4996 /wd4305 /wd4101 /wd4068 /wd4661 /wd4477 /wd4804 /wd4700")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${DISABLED_WARNINGS}")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${DISABLED_WARNINGS}")

else()
    message(FATAL_ERROR "This operating system is not supported")
endif()

INSTALL(TARGETS ${CMDNAME} DESTINATION bin)
