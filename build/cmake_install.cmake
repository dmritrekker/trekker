# Install script for directory: /home/baran/Work/code/dev/baranaydogan/trekker

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "1")
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}/home/baran/Work/code/dev/baranaydogan/trekker/build/install/bin/trekker" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}/home/baran/Work/code/dev/baranaydogan/trekker/build/install/bin/trekker")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}/home/baran/Work/code/dev/baranaydogan/trekker/build/install/bin/trekker"
         RPATH "")
  endif()
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/home/baran/Work/code/dev/baranaydogan/trekker/build/install/bin/trekker")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
file(INSTALL DESTINATION "/home/baran/Work/code/dev/baranaydogan/trekker/build/install/bin" TYPE EXECUTABLE FILES "/home/baran/Work/code/dev/baranaydogan/trekker/build/trekker")
  if(EXISTS "$ENV{DESTDIR}/home/baran/Work/code/dev/baranaydogan/trekker/build/install/bin/trekker" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}/home/baran/Work/code/dev/baranaydogan/trekker/build/install/bin/trekker")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}/home/baran/Work/code/dev/baranaydogan/trekker/build/install/bin/trekker")
    endif()
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  configure_file(/home/baran/Work/code/dev/baranaydogan/trekker/build/libTrekker.a /home/baran/Work/code/dev/baranaydogan/trekker/build/install/lib/libTrekker.a COPYONLY)
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  configure_file(/home/baran/Work/code/dev/baranaydogan/trekker/build/libTrekker.so /home/baran/Work/code/dev/baranaydogan/trekker/build/install/lib/libTrekker.so COPYONLY)
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  configure_file(/home/baran/Work/code/dev/baranaydogan/trekker/src/trekker.h /home/baran/Work/code/dev/baranaydogan/trekker/build/install/include/trekker.h COPYONLY)
endif()

if(CMAKE_INSTALL_COMPONENT)
  set(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
else()
  set(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
file(WRITE "/home/baran/Work/code/dev/baranaydogan/trekker/build/${CMAKE_INSTALL_MANIFEST}"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
