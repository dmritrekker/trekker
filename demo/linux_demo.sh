#!/bin/bash

FOD=data/FOD.nii
SEEDimag=data/seedImage.nii

trekker=../releases/v1.0b/trekker_linux_x64_v1.0b

${trekker} track \
${FOD} \
--output linux_demo_output.vtk \
--seed ${SEEDimag} \
--seed_count 100 \
--force \

