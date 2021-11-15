#!/bin/bash

FOD=data/FOD.nii
SEEDimag=data/seedImage.nii
SEEDcoor=data/seedCoordinates.txt

echo "================================="
echo "DEMO 1: Seed image based tracking"
echo "================================="
../binaries/trekker_linux_x64_v0.9 \
-fod ${FOD} \
-seed_image ${SEEDimag} \
-seed_count 100 \
-enableOutputOverwrite \
-output linux_demo_output1 \
-writeColors

echo "\n\n======================================"
echo "DEMO 2: Seed coordinate based tracking"
echo "======================================"
../binaries/trekker_linux_x64_v0.9 \
-fod ${FOD} \
-seed_coordinates ${SEEDcoor} \
-seed_count 100 \
-enableOutputOverwrite \
-output linux_demo_output2 \
-writeColors
echo "======================================"
echo "If you can see 'linux_demo_output1' and 'linux_demo_output2' in the demo folder, all is good."
echo "======================================"
