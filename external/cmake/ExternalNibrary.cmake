# NIBRARY

SET(NIBRARY_MIN_VERSION "0.3.0" CACHE STRING "Minimum nibrary version") 

include("${CMAKE_CURRENT_LIST_DIR}/utils.cmake")

if (CMAKE_SYSTEM_NAME STREQUAL "Windows")
    if (BUILD_SHARED_LIBS)
        set(NIBRARY_LIBRARIES Nibrary geogram zlib CACHE INTERNAL "Nibrary libraries to link against")
    else()
        set(NIBRARY_LIBRARIES Nibrary geogram zlibstatic dcm2niixfs CACHE INTERNAL "Nibrary libraries to link against")
    endif()
else()
    if (BUILD_SHARED_LIBS)
        set(NIBRARY_LIBRARIES Nibrary geogram z CACHE INTERNAL "Nibrary libraries to link against")
    else()
        set(NIBRARY_LIBRARIES Nibrary geogram z dcm2niixfs CACHE INTERNAL "Nibrary libraries to link against")
    endif()
endif()

if (NOT USE_SYSTEM_NIBRARY)

    set(NIBRARY_LIBRARY_DIR ${CMAKE_INSTALL_PREFIX}/lib/nibrary_v${NIBRARY_MIN_VERSION} CACHE INTERNAL "")
    set(NIBRARY_INCLUDE_DIR ${CMAKE_INSTALL_PREFIX}/include/nibrary_v${NIBRARY_MIN_VERSION} CACHE INTERNAL "")

    if (EXISTS "${CMAKE_SOURCE_DIR}/external/nibrary/CMakeLists.txt")
        
        message(STATUS "Nibrary will be built from local source")

        set(NIBRARY_SOURCE_DIR "${CMAKE_SOURCE_DIR}/external/nibrary")

        set(BUILDING_NIBRARY_FROM_SOURCE TRUE CACHE INTERNAL "Nibrary will be built from local source")

    elseif (EXISTS "${CMAKE_SOURCE_DIR}/external/nibrary_v${NIBRARY_MIN_VERSION}/CMakeLists.txt")

        message(STATUS "Nibrary will be built from local source")

        set(NIBRARY_SOURCE_DIR "${CMAKE_SOURCE_DIR}/external/nibrary_v${NIBRARY_MIN_VERSION}")

        set(BUILDING_NIBRARY_FROM_SOURCE TRUE CACHE INTERNAL "Nibrary will be built from local source")

    else()    

        set(BUILDING_NIBRARY_FROM_SOURCE TRUE CACHE INTERNAL "Nibrary will be downloaded and built from source")

        message(STATUS "Nibrary will be downloaded and built from source")

        set(NIBRARY_SOURCE_DIR "${CMAKE_BINARY_DIR}/external/nibrary_v${NIBRARY_MIN_VERSION}")

        set(DOWNLOAD_FNAME "nibrary_v${NIBRARY_MIN_VERSION}.zip")
        set(DOWNLOAD_URL   "https://github.com/nibrary/nibrary/releases/download/v${NIBRARY_MIN_VERSION}/${DOWNLOAD_FNAME}")
        set(DOWNLOAD_PATH  "${CMAKE_BINARY_DIR}/external/download/${DOWNLOAD_FNAME}")
        
        if (NOT EXISTS ${DOWNLOAD_PATH})
            file(DOWNLOAD ${DOWNLOAD_URL} ${DOWNLOAD_PATH}
                SHOW_PROGRESS
                STATUS download_status
                LOG download_log)
            list(GET download_status 0 status_code)
            if(NOT status_code EQUAL 0)
                message(FATAL_ERROR "Error downloading ${DOWNLOAD_URL}: ${download_log}")
            endif()

            execute_process(
                COMMAND ${CMAKE_COMMAND} -E tar xzf ${DOWNLOAD_PATH}
                WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/external"
                RESULT_VARIABLE extract_result
            )
            if(NOT extract_result EQUAL 0)
                message(FATAL_ERROR "Error extracting ${DOWNLOAD_FNAME}: ${extract_result}")
            endif()
        endif()

    endif()

    if(BUILDING_NIBRARY_FROM_SOURCE)

        include(ExternalProject)
        
        ExternalProject_Add(build_nibrary

            SOURCE_DIR "${NIBRARY_SOURCE_DIR}"

            PREFIX ${CMAKE_BINARY_DIR}/external/nibrary

            CMAKE_ARGS
                -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
                -DBUILD_SHARED_LIBS=${BUILD_SHARED_LIBS}
                -DCMAKE_C_COMPILER=${CMAKE_C_COMPILER}
                -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}
                -DCMAKE_OSX_DEPLOYMENT_TARGET=${CMAKE_OSX_DEPLOYMENT_TARGET}
                $<$<STREQUAL:${CMAKE_SYSTEM_NAME},Darwin>:-DOpenMP_C_FLAGS=$ENV{OpenMP_C_FLAGS}>
                $<$<STREQUAL:${CMAKE_SYSTEM_NAME},Darwin>:-DOpenMP_CXX_FLAGS=$ENV{OpenMP_CXX_FLAGS}>
                $<$<STREQUAL:${CMAKE_SYSTEM_NAME},Darwin>:-DOpenMP_C_LIB_NAMES=$ENV{OpenMP_C_LIB_NAMES}>
                $<$<STREQUAL:${CMAKE_SYSTEM_NAME},Darwin>:-DOpenMP_CXX_LIB_NAMES=$ENV{OpenMP_CXX_LIB_NAMES}>
                $<$<STREQUAL:${CMAKE_SYSTEM_NAME},Darwin>:-DOpenMP_omp_LIBRARY=$ENV{OpenMP_omp_LIBRARY}>
        )

        ExternalProject_Add_Step(build_nibrary POST_BUILD
            COMMENT "Moving nibrary headers and libraries"
            DEPENDEES install
            COMMAND ${CMAKE_COMMAND} -D CMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX} -D NIBRARY_MIN_VERSION=${NIBRARY_MIN_VERSION} -P "${CMAKE_CURRENT_LIST_DIR}/ExternalNibrary_aux.cmake"
            ALWAYS 0
        )

    endif()

endif()
