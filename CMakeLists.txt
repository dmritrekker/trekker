cmake_minimum_required (VERSION 3.15)

PROJECT(Trekker)

# Set output names
set(trekkerExe trekker)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(BUILD_SHARED_LIBS OFF)

if(UNIX AND NOT APPLE) 

	message(STATUS "Building for Linux")

	# Set up manual page
	add_definitions(-DSHOWMANUAL="man ${CMAKE_SOURCE_DIR}/doc/build/man/trekker")

	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -W -Wall -Wextra -pedantic -pedantic-errors -Wno-long-long -ggdb3 -fno-omit-frame-pointer")	
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -W -Wall -Wextra -pedantic -pedantic-errors -Wno-long-long")

	# FOR STATIC BUILD
	# Build and link only static libraries and executables
	set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static -static-libgcc -static-libstdc++ -Wl,--whole-archive -lpthread -Wl,--no-whole-archive")
	set(CMAKE_FIND_LIBRARY_SUFFIXES ".a")

	# Check for zlib support
	find_package(ZLIB)
	if(ZLIB_LIBRARIES)
		add_definitions(-D HAVE_ZLIB)
		message(STATUS "Enabled zlib support")
	else()
		message(STATUS "Disabled zlib support")
	endif()	

    # Needed for copying the library
	set(trekkerLibName libTrekker.a)

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

	# No zlib support at the moment
	
	# Needed for copying the library
	set(trekkerLibName Trekker.lib)
else()

    message(FATAL_ERROR "This operating system is not supported")

endif()	



# Verify compiler support for c++11
include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
if(COMPILER_SUPPORTS_CXX11)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
else()
    message(STATUS "Your compiler (${CMAKE_CXX_COMPILER}) does not support C++11 support. Please use a compatible compiler.")
endif()




# Check for Python3
# /home/baran/apps/anaconda3/envs/trekkerWrapper/bin/python3.7
find_package(Python3)
if(Python3_FOUND)
	message(WARNING "Please change Python path if necessary: ${Python3_EXECUTABLE}")
	message(STATUS "Enabled Python wrapper. A Python package will be build.")
else()
	message(STATUS "Disabled Python wrapper. A Python package will not be build.")
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

# ENABLE THIS
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

# FOR REGULAR COMPILATION
target_link_libraries(${trekkerExe} c ${ZLIB_LIBRARIES})

INSTALL(TARGETS ${trekkerExe} DESTINATION ${CMAKE_BINARY_DIR}/install/bin)
INSTALL(CODE "configure_file(${CMAKE_BINARY_DIR}/${trekkerLibName} ${CMAKE_BINARY_DIR}/install/lib/${trekkerLibName} COPYONLY)")
INSTALL(CODE "configure_file(${CMAKE_SOURCE_DIR}/src/trekker.h ${CMAKE_BINARY_DIR}/install/include/trekker.h COPYONLY)")
INSTALL(CODE "configure_file(${CMAKE_SOURCE_DIR}/extensions/python/setup.py ${CMAKE_BINARY_DIR}/install/extensions/python/setup.py COPYONLY)")
INSTALL(CODE "configure_file(${CMAKE_SOURCE_DIR}/extensions/python/cython/Trekker.pyx ${CMAKE_BINARY_DIR}/install/extensions/python/cython/Trekker.pyx COPYONLY)")
INSTALL(CODE "configure_file(${CMAKE_SOURCE_DIR}/src/trekker.h ${CMAKE_BINARY_DIR}/install/extensions/python/cython/trekker.h COPYONLY)")
INSTALL(CODE "configure_file(${CMAKE_BINARY_DIR}/install/lib/${trekkerLibName} ${CMAKE_BINARY_DIR}/install/extensions/python/cython/${trekkerLibName} COPYONLY)")
INSTALL(CODE "execute_process(COMMAND ${Python3_EXECUTABLE} ${CMAKE_BINARY_DIR}/install/extensions/python/setup.py bdist_wheel WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/install/extensions/python)")


# Add a custom clean-all target
add_custom_target(clean-all
   COMMAND ${CMAKE_BUILD_TOOL} clean
   COMMAND rm -rf ${CMAKE_BINARY_DIR}/extensions
   COMMAND rm -rf ${CMAKE_BINARY_DIR}/install
   COMMAND rm -f ${CMAKE_BINARY_DIR}/*.o
   COMMAND rm -f ${CMAKE_BINARY_DIR}/*.a
)
