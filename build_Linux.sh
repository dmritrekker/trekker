#!/bin/bash

cmakeExe=/usr/bin/cmake
buildType=Release #Release or Debug

c_compiler=/usr/bin/gcc
cxx_compiler=/usr/bin/g++

buildPythonPackage=ON #ON or OFF
pythonExe=/usr/bin/python3


rm -rf build/Linux
mkdir -p build/Linux
cd build/Linux
${cmakeExe} -DCMAKE_CC_COMPILER=${c_compiler} -DCMAKE_CXX_COMPILER=${cxx_compiler} -DCMAKE_BUILD_TYPE=${buildType} -DBuild_Python3_WRAPPER=${buildPythonPackage} -DPython3_EXECUTABLE=${pythonExe} ../..
${cmakeExe} --build . --config ${buildType} --target install --parallel 8
cd ../..
