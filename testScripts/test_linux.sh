#!/bin/bash

chmod +x ./trekker_linux

# Display help
./trekker_linux

# Run a simple fiber tracking script
./trekker_linux track -f \
../testData/100307_FOD_Order4.nii.gz \
--seed ../testData/100307_lh_white.vtk \
--seed_count 100 \
--output out_linux.vtk

# Display tractogram info
./trekker_linux info out_linux.vtk
