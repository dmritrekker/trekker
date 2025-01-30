#!/bin/bash

chmod +x ./trekker_linux

# Display help
echo "Testing help"
./trekker_linux
echo "Done"

# Display image info
echo "Display image info"
./trekker_linux info ../testData/100307_FOD_Order4.nii.gz
echo "Done"

# Display surface info
echo "Display surface info"
./trekker_linux info ../testData/100307_lh_white.vtk
echo "Done"

# Run a simple fiber tracking script
echo "Run a simple fiber tracking script"
./trekker_linux track -f \
../testData/100307_FOD_Order4.nii.gz \
--seed ../testData/100307_lh_white.vtk \
--seed_count 100 \
--output out.vtk
echo "Done"

# Display tractogram info
if [ -f "out.vtk" ]; then
    file_size=$(ls -l "out.vtk" | awk '{print $5}')
    echo "out.vtk size: $file_size bytes"
    ./trekker_linux info out.vtk
else
    echo "out.vtk does not exist."
fi

# Display info help
echo "Testing info help"
./trekker_linux info
echo "Done"


# Display track help
echo "Testing track help"
./trekker_linux track
echo "Done"
