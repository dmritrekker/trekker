#!/bin/bash

# Requires libomp
libomp_prefix=$(brew --prefix libomp)

if ! brew list | grep -q "libomp"; then
  brew install libomp
fi

export OpenMP_omp_LIBRARY="${libomp_prefix}/lib/libomp.dylib"
echo "limomp path: $OpenMP_omp_LIBRARY"

chmod +x ./trekker_mac

# Display help
./trekker_mac

# Run a simple fiber tracking script
./trekker_mac track -f \
../testData/100307_FOD_Order4.nii.gz \
--seed ../testData/100307_lh_white.vtk \
--seed_count 100 \
--output out_linux.vtk

# Display tractogram info
./trekker_mac info out_mac.vtk
