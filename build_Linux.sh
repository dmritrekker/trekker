#!/bin/bash

cmakeExe=/home/baran/apps/cmake-3.15.0-Linux-x86_64/bin/cmake
pythonExe=/home/baran/apps/anaconda3/envs/trekkerWrapper/bin/python

rm -rf build/Linux
mkdir -p build/Linux
cd build/Linux
${cmakeExe} -DCMAKE_BUILD_TYPE=Release -DPython3_EXECUTABLE=${pythonExe} ../..
${cmakeExe} --build . --target install --parallel 8

