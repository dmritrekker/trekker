cmake_minimum_required(VERSION 3.15.0)

set(trekker_VERSION_MAJOR 1)
set(trekker_VERSION_MINOR 0)
set(trekker_VERSION_PATCH 0)

project(trekker VERSION ${trekker_VERSION_MAJOR}.${trekker_VERSION_MINOR}.${trekker_VERSION_PATCH} LANGUAGES CXX C)

set(CMDNAME trekker_v${trekker_VERSION}-rc4)
message(STATUS ">> Building ${CMDNAME}")

set(CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}/install")

option(BUILD_SHARED_LIBS    "Build shared libraries"                ON)
option(USE_SYSTEM_NIBRARY   "Use system nibrary if exists"          OFF)
option(BUILD_NATIVE         "Build with -march=native optimization" ON)

# Using C++17 standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_OSX_DEPLOYMENT_TARGET "11.0" CACHE STRING "Minimum OS X deployment version")

include_directories(${CMAKE_SOURCE_DIR}/src)
include_directories(${CMAKE_INCLUDE_PATH})
link_directories(${CMAKE_LIBRARY_PATH})

# Include external dependencies
add_subdirectory(external)
include_directories(${NIBRARY_INCLUDE_DIR})
link_directories(${NIBRARY_LIBRARY_DIR})

# List source files
file(GLOB_RECURSE SRCS CONFIGURE_DEPENDS RELATIVE ${CMAKE_SOURCE_DIR} 
    "src/*.c*")

list(REMOVE_ITEM SRCS "main.cpp")

# Set executable
add_executable(${CMDNAME} ${CMAKE_SOURCE_DIR}/src/main.cpp ${SRCS})

# Add version information
target_compile_definitions(${CMDNAME} PRIVATE
    TREKKER_VERSION_MAJOR=${trekker_VERSION_MAJOR}
    TREKKER_VERSION_MINOR=${trekker_VERSION_MINOR}
    TREKKER_VERSION_PATCH=${trekker_VERSION_PATCH}
)

find_package(OpenMP REQUIRED)

if(OpenMP_FOUND)
    target_compile_options(${CMDNAME} PUBLIC ${OpenMP_CXX_FLAGS})
    target_link_libraries(${CMDNAME} OpenMP::OpenMP_CXX)
    target_include_directories(${CMDNAME} PUBLIC ${OpenMP_CXX_INCLUDE_DIRS})
endif()


if ((CMAKE_CXX_COMPILER_ID MATCHES "GNU") OR (CMAKE_CXX_COMPILER_ID MATCHES "Clang") )
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -W -Wno-long-long -Wno-stringop-truncation -Wno-dev -Wno-unknown-pragmas -Wno-unused-result -Wall -Wextra -pedantic -pedantic-errors" CACHE STRING "C++ Compiler Flags" FORCE)
endif()

if ((CMAKE_CXX_COMPILER_ID MATCHES "GNU") AND (CMAKE_CXX_COMPILER_VERSION VERSION_LESS "9"))
    target_link_libraries(${CMDNAME} stdc++fs)
endif()

if (UNIX)

    # Optimize for native architecture
    if (BUILD_NATIVE)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=native")
    endif()

    # Debug messages
    if(CMAKE_BUILD_TYPE MATCHES Debug)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=address")
    endif()
    
endif()

if (UNIX)
    # Thread support
    find_package(Threads)
    if(Threads_FOUND)
        target_link_libraries(${CMDNAME} Threads::Threads)
    else()
        message(WARNING "pthreads not found")
    endif()

    # Debug messages
    if(CMAKE_BUILD_TYPE MATCHES Debug)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=address")
    endif()
endif()

if (CMAKE_SYSTEM_NAME STREQUAL "Linux")

    message(STATUS ">> Trekker::Building for Linux")

elseif (CMAKE_SYSTEM_NAME STREQUAL "Darwin")

    message(STATUS ">> Trekker::Building for MacOs")
    add_compile_definitions(APPLE __APPLE__ MACH __MACH__)

elseif (CMAKE_SYSTEM_NAME STREQUAL "Windows")

    message(STATUS ">> Trekker::Building for Windows")

    # if (BUILD_SHARED_LIBS)
    #     set_target_properties(${CMDNAME} PROPERTIES MSVC_RUNTIME_LIBRARY "MultiThreadedDLL")
    # else()
        set_target_properties(${CMDNAME} PROPERTIES MSVC_RUNTIME_LIBRARY "MultiThreaded")
    # endif()

    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++17 /permissive- /bigobj")

    # For definitions
    add_compile_definitions(BUILD_FOR_WINDOWS _USE_MATH_DEFINES _WIN32 WIN32 _WIN64 WIN64)

    # Disable some compiler warnings
    set(DISABLED_WARNINGS "/wd4244 /wd4267 /wd4996 /wd4305 /wd4101 /wd4068 /wd4661 /wd4477 /wd4804 /wd4700")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${DISABLED_WARNINGS}")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${DISABLED_WARNINGS}")

    set_target_properties(${CMDNAME} PROPERTIES WINDOWS_EXPORT_ALL_SYMBOLS TRUE)

else()
    message(FATAL_ERROR ">> Trekker::This operating system is not supported")
endif()


if(BUILDING_NIBRARY_FROM_SOURCE)
    message(STATUS ">> NIBRARY_LIBRARY_DIR: ${NIBRARY_LIBRARY_DIR}")
    message(STATUS ">> NIBRARY_INCLUDE_DIR: ${NIBRARY_INCLUDE_DIR}")
    add_dependencies(${CMDNAME} build_nibrary)
endif()

target_link_libraries(${CMDNAME} ${NIBRARY_LIBRARIES})

install(TARGETS ${CMDNAME} RUNTIME DESTINATION bin)

if(BUILDING_NIBRARY_FROM_SOURCE)
    install(DIRECTORY ${NIBRARY_INCLUDE_DIR} DESTINATION include)
endif()

add_custom_target(uninstall
    COMMAND ${CMAKE_COMMAND} -E echo "Uninstalling ${NIBRARY_LIBRARY_DIR}"
    COMMAND ${CMAKE_COMMAND} -E remove_directory -v "${NIBRARY_LIBRARY_DIR}"
    COMMAND ${CMAKE_COMMAND} -E echo "Uninstalling ${NIBRARY_INCLUDE_DIR}"
    COMMAND ${CMAKE_COMMAND} -E remove_directory -v "${NIBRARY_INCLUDE_DIR}"
    COMMAND ${CMAKE_COMMAND} -E echo "Uninstalling ${CMAKE_BINARY_DIR}/install/bin/${CMDNAME}"
    COMMAND ${CMAKE_COMMAND} -E remove -f "${CMAKE_BINARY_DIR}/install/bin/${CMDNAME}"
)
