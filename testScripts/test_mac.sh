#!/bin/bash

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
