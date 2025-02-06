#!/bin/bash

# Linux build example

buildType=Release
buildShared=OFF
buildDir=build-static

mkdir -p ${buildDir}
cd ${buildDir}

cmakeExe=cmake
c_compiler=/bin/gcc
cxx_compiler=/bin/g++


${cmakeExe} \
-DCMAKE_C_COMPILER=${c_compiler} \
-DCMAKE_CXX_COMPILER=${cxx_compiler} \
-DCMAKE_BUILD_TYPE=${buildType} \
-DBUILD_SHARED_LIBS=${buildShared} \
..


${cmakeExe} --build . --config ${buildType} --target install --parallel $(nproc)

cd ..
