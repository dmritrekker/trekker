#!/bin/bash

chmod +x ./trekker_linux

# Display help
./trekker_linux

# Run a simple fiber tracking script
./trekker_linux track -f \
../testData/100307_FOD_Order4.nii.gz \
--seed ../testData/100307_lh_white.vtk \
--seed_count 100 \
--output out.vtk

# Display tractogram info
if [ -f "out.vtk" ]; then
    file_size=$(ls -l "out.vtk" | awk '{print $5}')
    echo "out.vtk size: $file_size bytes"
    ./trekker_linux info out.vtk
else
    echo "out.vtk does not exist."
fi

# Display info help
./trekker_linux info

# Display track help
./trekker_linux track
