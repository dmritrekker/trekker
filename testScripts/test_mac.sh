#!/bin/bash

# Requires LLVM and libomp
llvm_prefix=$(brew --prefix llvm@18)
libomp_prefix=$(brew --prefix libomp)

if ! brew list | grep -q "llvm@18"; then
  brew install llvm@18
fi

if ! brew list | grep -q "libomp"; then
  brew install libomp
fi

# Set environment variables for this script
export PATH="${llvm_prefix}/bin:$PATH"
export LDFLAGS="-L${llvm_prefix}/lib -L${libomp_prefix}/lib"
export OpenMP_omp_LIBRARY="${libomp_prefix}/lib/libomp.dylib"

chmod +x ./trekker_mac

# Display help
./trekker_mac

# Run a simple fiber tracking script
./trekker_mac track -f \
../testData/100307_FOD_Order4.nii.gz \
--seed ../testData/100307_lh_white.vtk \
--seed_count 100 \
--output out.vtk

# Display tractogram info
if [ -f "out.vtk" ]; then
    file_size=$(ls -l "out.vtk" | awk '{print $5}')
    echo "out.vtk size: $file_size bytes"
    ./trekker_mac info out.vtk
else
    echo "out.vtk does not exist."
fi

# Display info help
./trekker_mac info

# Display track help
./trekker_mac track
