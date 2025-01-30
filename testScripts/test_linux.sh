#!/bin/bash

# Make sure trekker_linux is executable
chmod +x ./trekker_linux

# --- Configuration ---
test_data_dir="../testData"
results_dir="testResults"

# Input files
fod_image="$test_data_dir/100307_FOD_Order4.nii.gz"
surface_file="$test_data_dir/100307_lh_white.vtk"
tractogram_file="$test_data_dir/100307_lh_50K.vtk"

# Output files
track2img_output="$results_dir/100307_lh_50K.nii.gz"
track2surf_output="$results_dir/100307_lh_white_50K.vtk"
fiber_tracking_output="$results_dir/out.vtk"

# --- Function to display file information ---
display_file_info() {
  local file="$1"

  if [ -z "$file" ]; then
    echo "Error: No file specified."
    echo "Usage: $0 <file>"
    return 1
  fi

  if [ -f "$file" ]; then
    file_size=$(stat -c%s "$file")

    echo
    echo "$file size: $file_size bytes"
    ./trekker_linux info "$file"
    echo

    return 0
  else
    echo
    echo "$file does not exist."
    echo
    return 1
  fi
}

# --- Main Script ---

# Create results directory if it doesn't exist
mkdir -p "$results_dir"

# Display help
echo "Testing help"
./trekker_linux
echo "Done"
echo

# Display info help
echo "Testing info help"
./trekker_linux info
echo "Done"
echo

# Display dMRI recon help
echo "Testing dMRI recon help"
./trekker_linux dMRI recon
echo "Done"
echo

# Display image info
echo "Display image info"
./trekker_linux info "$fod_image" -v debug
echo "Done"
echo

# Display surface info
echo "Display surface info"
./trekker_linux info "$surface_file" -v debug
echo "Done"
echo

# Display tractogram info
echo "Display tractogram info"
./trekker_linux info "$tractogram_file" -v debug
echo "Done"
echo

# track2img
echo "track2img"
./trekker_linux track2img -f "$tractogram_file" "$track2img_output"
echo "Done"
echo

display_file_info "$track2img_output"

# track2surf
echo "track2surf"
./trekker_linux track2surf -f "$tractogram_file" "$surface_file" "$track2surf_output" dens --feature streamlineDensity
echo "Done"
echo

display_file_info "$track2surf_output"

# Run a simple fiber tracking script
echo "Run a simple fiber tracking script"
./trekker_linux track -f "$fod_image" \
  --seed "$surface_file" \
  --seed_count 100 \
  --output "$fiber_tracking_output"
echo "Done"
echo

display_file_info "$fiber_tracking_output"
