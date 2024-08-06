#!/bin/bash

cmakeExe=cmake
buildType=Release #Release or Debug
buildShared=OFF
buildDir=build-static

inc_path="../nibrary/${buildDir}/install/include/nibrary_v0.1.0"
lib_path="../nibrary/${buildDir}/install/lib/nibrary_v0.1.0"

c_compiler=/bin/gcc
cxx_compiler=/bin/g++

rm -rf ${buildDir}
mkdir -p ${buildDir}
cd ${buildDir}

${cmakeExe} \
-DCMAKE_C_COMPILER=${c_compiler} \
-DCMAKE_CXX_COMPILER=${cxx_compiler} \
-DCMAKE_BUILD_TYPE=${buildType} \
-DCMAKE_INCLUDE_PATH=${inc_path} \
-DCMAKE_LIBRARY_PATH=${lib_path} \
-DBUILD_SHARED_LIBS=${buildShared} \
..

${cmakeExe} --build . --config ${buildType} --target install --parallel 16

cd ..

