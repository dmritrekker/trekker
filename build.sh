#!/bin/bash

cmakeExe=cmake
buildType=Release #Release or Debug
buildShared=OFF
buildDir=build-static


mkdir -p ${buildDir}
cd ${buildDir}


# ===========================
# LINUX EXAMPLE
# Requires GCC-12 and above 
# Requires OpenMP (e.g. libomp-dev)


c_compiler=/bin/gcc
cxx_compiler=/bin/g++

${cmakeExe} \
-DCMAKE_C_COMPILER=${c_compiler} \
-DCMAKE_CXX_COMPILER=${cxx_compiler} \
-DCMAKE_BUILD_TYPE=${buildType} \
-DBUILD_SHARED_LIBS=${buildShared} \
..

# ===========================
# MacOS example
# Requires llvm (tested with llvm@18, llvm@19 is not supported)
# Requires libomp

# c_compiler=/opt/homebrew/opt/llvm@18/bin/clang
# cxx_compiler=/opt/homebrew/opt/llvm@18/bin/clang++

# ${cmakeExe} \
# -DCMAKE_C_COMPILER=${c_compiler} \
# -DCMAKE_CXX_COMPILER=${cxx_compiler} \
# -DCMAKE_BUILD_TYPE=${buildType} \
# -DBUILD_SHARED_LIBS=${buildShared} \
# -DOpenMP_C_FLAGS="-I/opt/homebrew/opt/llvm@18/include -I/opt/homebrew/opt/libomp/include -Xclang -fopenmp" \
# -DOpenMP_CXX_FLAGS="-I/opt/homebrew/opt/llvm@18/include -I/opt/homebrew/opt/libomp/include -Xclang -fopenmp" \
# -DOpenMP_C_LIB_NAMES="omp" \
# -DOpenMP_CXX_LIB_NAMES="omp" \
# -DOpenMP_omp_LIBRARY="/opt/homebrew/opt/libomp/lib/libomp.dylib" \
# ..

# ===========================

${cmakeExe} --build . --config ${buildType} --target install --parallel 16

cd ..

