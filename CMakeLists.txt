cmake_minimum_required (VERSION 3.15)

PROJECT(Trekker)

# Set output names
set(trekkerExe trekker)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(BUILD_SHARED_LIBS OFF)

# Using C++11 standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

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

	# No zlib support for Windows at the moment

	# Needed for copying the library
	set(trekkerLibName libTrekker)
else()
    message(FATAL_ERROR "This operating system is not supported")
endif()

# Check if a python package should be built
if(${Build_Python3_WRAPPER} STREQUAL "ON")
		find_package(Python3)
		if(Python3_FOUND)
			message(STATUS "Enabled Python wrapper. A Python package will be build.")
			message(WARNING "Please change Python path if necessary which is currently set as: ${Python3_EXECUTABLE}")
		else()
			set(Build_Python3_WRAPPER "OFF")
			message(STATUS "Python not found. A Python package will not be build.")
		endif()
else()
	message(STATUS "A Python package will not be build.")
endif()

file(GLOB_RECURSE SRCS RELATIVE ${CMAKE_SOURCE_DIR} "src/*.c*")
list(REMOVE_ITEM SRCS "src/cmd.cpp")


# The Linux version statically links zlib as well. We skip compiling and linking zlib for Windows yet.
if(UNIX AND NOT APPLE)

		add_library(tmp STATIC ${SRCS})
		target_include_directories(tmp PUBLIC ${CMAKE_SOURCE_DIR}/src/nifticlib-2.0.0/znzlib)
		target_include_directories(tmp PUBLIC ${CMAKE_SOURCE_DIR}/src/nifticlib-2.0.0/niftilib)

		set(trekkerLib ${CMAKE_BINARY_DIR}/${trekkerLibName})
		add_custom_target(combined COMMAND ar -x $<TARGET_FILE:tmp> COMMAND ar -qcs ${trekkerLib} *.o WORKING_DIRECTORY ${CMAKE_BINARY_DIR} DEPENDS tmp)

		add_library(c STATIC IMPORTED GLOBAL)
		add_dependencies(c combined)
		set_target_properties(c PROPERTIES IMPORTED_LOCATION ${trekkerLib})

		add_executable(${trekkerExe} ${CMAKE_SOURCE_DIR}/src/cmd.cpp)

		target_link_libraries(${trekkerExe} c ${ZLIB_LIBRARIES})

		INSTALL(TARGETS ${trekkerExe} DESTINATION ${CMAKE_BINARY_DIR}/install/bin)
		INSTALL(CODE "configure_file(${CMAKE_BINARY_DIR}/${trekkerLibName} ${CMAKE_BINARY_DIR}/install/lib/${trekkerLibName} COPYONLY)")
		INSTALL(CODE "configure_file(${CMAKE_SOURCE_DIR}/src/trekker.h ${CMAKE_BINARY_DIR}/install/include/trekker.h COPYONLY)")

		if(${Build_Python3_WRAPPER} STREQUAL "ON")
				INSTALL(CODE "configure_file(${CMAKE_SOURCE_DIR}/extensions/python/setup.py ${CMAKE_BINARY_DIR}/install/extensions/python/setup.py COPYONLY)")
				INSTALL(CODE "configure_file(${CMAKE_SOURCE_DIR}/extensions/python/cython/Trekker.pyx ${CMAKE_BINARY_DIR}/install/extensions/python/cython/Trekker.pyx COPYONLY)")
				INSTALL(CODE "configure_file(${CMAKE_SOURCE_DIR}/src/trekker.h ${CMAKE_BINARY_DIR}/install/extensions/python/cython/trekker.h COPYONLY)")
				INSTALL(CODE "configure_file(${CMAKE_BINARY_DIR}/install/lib/${trekkerLibName} ${CMAKE_BINARY_DIR}/install/extensions/python/cython/${trekkerLibName} COPYONLY)")
				INSTALL(CODE "execute_process(COMMAND ${Python3_EXECUTABLE} ${CMAKE_BINARY_DIR}/install/extensions/python/setup.py bdist_wheel WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/install/extensions/python)")
		endif()

endif()

if(MSVC)

		add_library(${trekkerLibName} STATIC ${SRCS})

		target_include_directories(${trekkerLibName} PUBLIC ${CMAKE_SOURCE_DIR}/src/nifticlib-2.0.0/znzlib)
		target_include_directories(${trekkerLibName} PUBLIC ${CMAKE_SOURCE_DIR}/src/nifticlib-2.0.0/niftilib)

		add_executable(${trekkerExe} ${CMAKE_SOURCE_DIR}/src/cmd.cpp)
		target_link_libraries(${trekkerExe} ${trekkerLibName})

		INSTALL(TARGETS ${trekkerLibName} DESTINATION ${CMAKE_BINARY_DIR}/install/lib)
		INSTALL(TARGETS ${trekkerExe} DESTINATION ${CMAKE_BINARY_DIR}/install/bin)
		INSTALL(CODE "configure_file(${CMAKE_SOURCE_DIR}/src/trekker.h ${CMAKE_BINARY_DIR}/install/include/trekker.h COPYONLY)")

		if(${Build_Python3_WRAPPER} STREQUAL "ON")
				INSTALL(CODE "configure_file(${CMAKE_SOURCE_DIR}/extensions/python/setup.py ${CMAKE_BINARY_DIR}/install/extensions/python/setup.py COPYONLY)")
				INSTALL(CODE "configure_file(${CMAKE_SOURCE_DIR}/extensions/python/cython/Trekker.pyx ${CMAKE_BINARY_DIR}/install/extensions/python/cython/Trekker.pyx COPYONLY)")
				INSTALL(CODE "configure_file(${CMAKE_SOURCE_DIR}/src/trekker.h ${CMAKE_BINARY_DIR}/install/extensions/python/cython/trekker.h COPYONLY)")
				INSTALL(CODE "configure_file(${CMAKE_BINARY_DIR}/install/lib/${trekkerLibName}.lib ${CMAKE_BINARY_DIR}/install/extensions/python/cython/Trekker.lib COPYONLY)")
				string(REPLACE "/" "\\\\" PythonCommand ${Python3_EXECUTABLE})
				INSTALL(CODE "execute_process(COMMAND cmd.exe /c \"${PythonCommand}\" ${CMAKE_BINARY_DIR}/install/extensions/python/setup.py bdist_wheel WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/install/extensions/python)")
		endif()

endif()
