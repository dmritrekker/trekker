cmake_minimum_required (VERSION 3.15)

PROJECT(Trekker)

add_definitions(-DCMAKE_BUILD_TYPE=Release)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(BUILD_SHARED_LIBS OFF)

# Set output names
set(trekkerExe trekker)

if(UNIX AND NOT APPLE) 

	message(STATUS "Building for Linux")

	# Needed for copying the library
	set(trekkerLibName libTrekker.a)

	# Set up manual page
	add_definitions(-DSHOWMANUAL="man ${CMAKE_SOURCE_DIR}/doc/build/man/trekker")

	# Build and link only static libraries and executables
	set(CMAKE_EXE_LINKER_FLAGS ${CMAKE_EXE_LINKER_FLAGS} "-static -static-libgcc -static-libstdc++")
	set(CMAKE_FIND_LIBRARY_SUFFIXES ".a")

	# Check for zlib support
	find_package(ZLIB)
	if(ZLIB_LIBRARIES)
		add_definitions(-D HAVE_ZLIB)
		message(STATUS "Enabled zlib support")
	else()
		message(STATUS "Disabled zlib support")
	endif()	

	# Check for multithreading support
	set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
	find_package(Threads)
	if(CMAKE_USE_PTHREADS_INIT)
		add_definitions(-D ENABLE_MULTITHREADING)
		find_library(PTHREAD_LIBRARIES NAMES libpthread.a)
		message(STATUS ${PTHREAD_LIBRARIES})
		message(STATUS "Enabled multithreading support")
	else()
		message(STATUS "Disabled multithreading support")
	endif()

elseif(MSVC)

    	message(STATUS "Building for Windows")

	# Building for Windows
	add_definitions(-D BUILD_FOR_WINDOWS)

	# Enable static compilation
	set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

	# Disable some compiler warnings for cl
	add_compile_options(/wd4244) # conversion from 'double' to 'x', possible loss of data
	add_compile_options(/wd4267) # conversion from 'size_t' to 'x', possible loss of data
	add_compile_options(/wd4996) # 'sprintf': This function or variable may be unsafe. Consider using sprintf_s instead.
	add_compile_options(/wd4305) # truncation from 'double' to 'float'
	add_compile_options(/wd4101) # unreferenced local variable

	# Needed for copying the library
	set(trekkerLibName Trekker.lib)
else()

    message(FATAL_ERROR "This operating system is not supported")

endif()	



# Check for Python3
# /home/baran/apps/anaconda3/envs/trekkerWrapper/bin/python3.7
find_package(Python3)
if(Python3_FOUND)
	message(WARNING "Please change Python path if necessary: ${Python3_EXECUTABLE}")
	message(STATUS "Enabled Python wrapper")
else()
	message(STATUS "Disabled Python wrapper")
endif()

file(GLOB_RECURSE SRCS RELATIVE ${CMAKE_SOURCE_DIR} "src/*.c*")
list(REMOVE_ITEM SRCS "src/cmd.cpp")

add_library(tmp STATIC ${SRCS})
target_include_directories(tmp PUBLIC ${CMAKE_SOURCE_DIR}/src/nifticlib-2.0.0/znzlib)
target_include_directories(tmp PUBLIC ${CMAKE_SOURCE_DIR}/src/nifticlib-2.0.0/niftilib)

set(trekkerLib ${CMAKE_BINARY_DIR}/${trekkerLibName})

# add_custom_target(combined
#        COMMAND ar -x $<TARGET_FILE:tmp>
#        COMMAND ar -x ${ZLIB_LIBRARIES}
#        COMMAND ar -x ${PTHREAD_LIBRARIES}
#        COMMAND ar -qcs ${trekkerLib} *.o
#        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
#	DEPENDS tmp ${ZLIB_LIBRARIES} ${PTHREAD_LIBRARIES}
#        )

add_custom_target(combined
        COMMAND ar -x $<TARGET_FILE:tmp>
        COMMAND ar -qcs ${trekkerLib} *.o
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
	DEPENDS tmp
        )

add_library(c STATIC IMPORTED GLOBAL)
add_dependencies(c combined)
set_target_properties(c PROPERTIES IMPORTED_LOCATION ${trekkerLib})

add_executable(${trekkerExe} ${CMAKE_SOURCE_DIR}/src/cmd.cpp)
# target_link_libraries(${trekkerExe} c)
target_link_libraries(${trekkerExe} c ${PTHREAD_LIBRARIES} ${ZLIB_LIBRARIES})

INSTALL(TARGETS ${trekkerExe} DESTINATION ${CMAKE_BINARY_DIR}/install/bin)
INSTALL(CODE "configure_file(${CMAKE_BINARY_DIR}/${trekkerLibName} ${CMAKE_BINARY_DIR}/install/lib/${trekkerLibName} COPYONLY)")
INSTALL(CODE "configure_file(${CMAKE_SOURCE_DIR}/src/trekker.h ${CMAKE_BINARY_DIR}/install/include/trekker.h COPYONLY)")
INSTALL(CODE "configure_file(${CMAKE_SOURCE_DIR}/src/trekker.h ${CMAKE_SOURCE_DIR}/extensions/python/cython/trekker.h COPYONLY)")
INSTALL(CODE "configure_file(${CMAKE_BINARY_DIR}/install/lib/${trekkerLibName} ${CMAKE_SOURCE_DIR}/extensions/python/cython/${trekkerLibName} COPYONLY)")
INSTALL(CODE "execute_process(COMMAND ${Python3_EXECUTABLE} ${CMAKE_SOURCE_DIR}/extensions/python/setup.py bdist_wheel WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/extensions/python)")

