#!/bin/bash

cmakeExe=cmake
buildType=Release #Release or Debug
buildShared=OFF
buildDir=build-static

inc_path="../nibrary/${buildDir}/install/include/nibrary_v0.1"
lib_path="../nibrary/${buildDir}/install/lib/nibrary_v0.1"

c_compiler=/bin/gcc-12
cxx_compiler=/bin/g++-12

# rm -rf ${buildDir}

rm /home/baran/Work/code/trekker/${buildDir}/CMakeFiles/trekker.dir/src/cmd/filter.cpp.o
rm /home/baran/Work/code/trekker/${buildDir}/CMakeFiles/trekker.dir/src/cmd/track.cpp.o

mkdir -p ${buildDir}
cd ${buildDir}

: '
${cmakeExe} \
-DCMAKE_C_COMPILER=${c_compiler} \
-DCMAKE_CXX_COMPILER=${cxx_compiler} \
-DCMAKE_BUILD_TYPE=${buildType} \
-DCMAKE_INCLUDE_PATH=${inc_path} \
-DCMAKE_LIBRARY_PATH=${lib_path} \
-DBUILD_SHARED_LIBS=${buildShared} \
..
'

${cmakeExe} --build . --config ${buildType} --target install --parallel 16

cd ..

