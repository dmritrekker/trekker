#!/bin/bash

# MacOS build example

# Requires cmake
# Requires llvm (tested with llvm@18, llvm@19 is not supported)
# Requires libomp

# --- Find Homebrew-installed LLVM and libomp ---
llvm_prefix=$(brew --prefix llvm@18)
libomp_prefix=$(brew --prefix libomp)

# --- Check for required tools and install if necessary ---
if ! command -v cmake &> /dev/null; then
  echo "Error: CMake not found. Please install it."
  exit 1
fi

if ! brew list | grep -q "llvm@18"; then
  brew install llvm@18
fi

if ! brew list | grep -q "libomp"; then
  brew install libomp
fi


# Set environment variables for this script
export PATH="${llvm_prefix}/bin:$PATH"
export LDFLAGS="-L${llvm_prefix}/lib -L${libomp_prefix}/lib"
export CPPFLAGS="-I${llvm_prefix}/include -I${libomp_prefix}/include"
export OpenMP_C_FLAGS="-I${llvm_prefix}/include -I${libomp_prefix}/include -Xclang -fopenmp"
export OpenMP_CXX_FLAGS="-I${llvm_prefix}/include -I${libomp_prefix}/include -Xclang -fopenmp"
export OpenMP_C_LIB_NAMES="omp"
export OpenMP_CXX_LIB_NAMES="omp"
export OpenMP_omp_LIBRARY="${libomp_prefix}/lib/libomp.dylib"
# ================

# ================
# Later, if the script worked successfully, set environment variables permanently in the system using the following
# echo 'export PATH="/opt/homebrew/opt/llvm@18/bin:$PATH"' >> ~/.zshrc
# echo 'export LDFLAGS="-L/opt/homebrew/opt/llvm@18/lib -L/opt/homebrew/opt/libomp/lib"' >> ~/.zshrc
# echo 'export CPPFLAGS="-I/opt/homebrew/opt/llvm@18/include -I/opt/homebrew/opt/libomp/include"' >> ~/.zshrc
# echo 'export OpenMP_C_FLAGS="-I/opt/homebrew/opt/llvm@18/include -I/opt/homebrew/opt/libomp/include -Xclang -fopenmp"' >> ~/.zshrc
# echo 'export OpenMP_CXX_FLAGS="-I/opt/homebrew/opt/llvm@18/include -I/opt/homebrew/opt/libomp/include -Xclang -fopenmp"' >> ~/.zshrc
# echo 'export OpenMP_C_LIB_NAMES="omp"' >> ~/.zshrc
# echo 'export OpenMP_CXX_LIB_NAMES="omp"' >> ~/.zshrc
# echo 'export OpenMP_omp_LIBRARY="/opt/homebrew/opt/libomp/lib/libomp.dylib"' >> ~/.zshrc
# ================


buildType=Release
buildShared=OFF
buildDir=build-static

mkdir -p ${buildDir}
cd ${buildDir}

cmakeExe=cmake
c_compiler=/opt/homebrew/opt/llvm@18/bin/clang
cxx_compiler=/opt/homebrew/opt/llvm@18/bin/clang++

system_arch=$(uname -m)
echo "Running build script for ${system_arch}"

${cmakeExe} \
-DCMAKE_OSX_ARCHITECTURES="${system_arch}" \
-DCMAKE_C_COMPILER="${c_compiler}" \
-DCMAKE_CXX_COMPILER="${cxx_compiler}" \
-DCMAKE_BUILD_TYPE="${buildType}" \
-DBUILD_SHARED_LIBS="${buildShared}" \
-DOpenMP_C_FLAGS="${OpenMP_C_FLAGS}" \
-DOpenMP_CXX_FLAGS="${OpenMP_CXX_FLAGS}" \
-DOpenMP_C_LIB_NAMES="${OpenMP_C_LIB_NAMES}" \
-DOpenMP_CXX_LIB_NAMES="${OpenMP_CXX_LIB_NAMES}" \
-DOpenMP_omp_LIBRARY="${OpenMP_omp_LIBRARY}" \
..

${cmakeExe} --build . --config ${buildType} --target install --parallel $(sysctl -n hw.ncpu)

cd ..
