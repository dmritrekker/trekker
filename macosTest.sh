#!/bin/bash


# ================
# MacOS example
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
# Later, if the script was useful, set environment variables permanently in the system using the following
# echo 'export PATH="/opt/homebrew/opt/llvm@18/bin:$PATH"' >> ~/.zshrc
# echo 'export LDFLAGS="-L/opt/homebrew/opt/llvm@18/lib -L/opt/homebrew/opt/libomp/lib"' >> ~/.zshrc
# echo 'export CPPFLAGS="-I/opt/homebrew/opt/llvm@18/include -I/opt/homebrew/opt/libomp/include"' >> ~/.zshrc
# echo 'export OpenMP_C_FLAGS="-I/opt/homebrew/opt/llvm@18/include -I/opt/homebrew/opt/libomp/include -Xclang -fopenmp"' >> ~/.zshrc
# echo 'export OpenMP_CXX_FLAGS="-I/opt/homebrew/opt/llvm@18/include -I/opt/homebrew/opt/libomp/include -Xclang -fopenmp"' >> ~/.zshrc
# echo 'export OpenMP_C_LIB_NAMES="omp"' >> ~/.zshrc
# echo 'export OpenMP_CXX_LIB_NAMES="omp"' >> ~/.zshrc
# echo 'export OpenMP_omp_LIBRARY="/opt/homebrew/opt/libomp/lib/libomp.dylib"' >> ~/.zshrc
# ================


cmakeExe=cmake
buildType=Release #Release or Debug
buildShared=OFF
buildDir=build-static
trekkerExe=trekker_v1.0.0-rc3

# If wanted use system nibrary
inc_path="../nibr/include/nibrary_v0.3.0/include"
lib_path="../nibr/lib/nibrary_v0.3.0/lib"
use_system_nibrary=ON


mkdir -p ${buildDir}
cd ${buildDir}

c_compiler=/opt/homebrew/opt/llvm@18/bin/clang
cxx_compiler=/opt/homebrew/opt/llvm@18/bin/clang++

system_arch=$(uname -m)
echo "Running build script for ${system_arch}"

${cmakeExe} \
-DCMAKE_OSX_ARCHITECTURES=${system_arch} \
-DCMAKE_C_COMPILER=${c_compiler} \
-DCMAKE_CXX_COMPILER=${cxx_compiler} \
-DCMAKE_BUILD_TYPE=${buildType} \
-DBUILD_SHARED_LIBS=${buildShared} \
-DOpenMP_C_FLAGS="${OpenMP_C_FLAGS}" \
-DOpenMP_CXX_FLAGS="${OpenMP_CXX_FLAGS}" \
-DOpenMP_C_LIB_NAMES="${OpenMP_C_LIB_NAMES}" \
-DOpenMP_CXX_LIB_NAMES="${OpenMP_CXX_LIB_NAMES}" \
-DOpenMP_omp_LIBRARY="${OpenMP_omp_LIBRARY}" \
-DCMAKE_INCLUDE_PATH=${inc_path} \
-DCMAKE_LIBRARY_PATH=${lib_path} \
-DUSE_SYSTEM_NIBRARY=${use_system_nibrary} \
..


num_cores=$(sysctl -n hw.ncpu)
${cmakeExe} --build . --config ${buildType} --target install --parallel "$num_cores"


if [[ -f ./${trekkerExe} ]]; then
  chmod +x ./${trekkerExe}
  ./${trekkerExe}
else
  echo "Trekker executable not found!"
fi

cd ..

echo "Build and execution test completed."
