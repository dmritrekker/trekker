#!/bin/bash

cmakeExe=cmake
buildType=Release #Release or Debug

c_compiler=gcc
cxx_compiler=g++

buildPythonPackage=ON #ON or OFF
pythonExe=python

rm -rf build/Linux
mkdir -p build/Linux
cd build/Linux
${cmakeExe} -DCMAKE_C_COMPILER=${c_compiler} -DCMAKE_CXX_COMPILER=${cxx_compiler} -DCMAKE_BUILD_TYPE=${buildType} -DBuild_Python3_WRAPPER=${buildPythonPackage} -DPython3_EXECUTABLE=${pythonExe} ../..
${cmakeExe} --build . --config ${buildType} --target install --parallel 16
cd ../..
