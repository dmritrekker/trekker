#!/bin/bash

# Make sure trekker_linux is executable
trekker=./trekker_linux
trekker=/home/baran/Work/code/trekker/build-static/trekker_v1.0.0-rc3

chmod +x ${trekker}

# --- Configuration ---
test_data_dir="../testData"
results_dir="testResults"

# Input files
FOD="$test_data_dir/FOD.nii.gz"
L_LGN="$test_data_dir/L_LGN.nii.gz"
L_V1="$test_data_dir/L_V1.nii.gz"
L_WM="$test_data_dir/L_WM.vtk"
WBT="$test_data_dir/WBT.vtk"

# Output files
out_track2img="$results_dir/track2img.nii.gz"
out_track2surf="$results_dir/track2surf.vtk"
out_track="$results_dir/track.vtk"
out_filter_1="$results_dir/filter_1.vtk"
out_filter_2="$results_dir/filter_2.vtk"

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
    ${trekker} info "$file"
    echo "$file size: $file_size bytes"
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


echo "=========="
echo "===Test 1: Display Trekker help"
echo "=========="
echo
${trekker}

echo
echo
echo "=========="
echo "===Test 2: Display info help"
echo "=========="
echo
${trekker} info


echo
echo
echo "=========="
echo "===Test 3: Display dMRI recon help"
echo "=========="
echo
${trekker} dMRI recon


echo
echo
echo "=========="
echo "===Test 4: Display FOD image info"
echo "=========="
echo
${trekker} info "$FOD"


echo
echo
echo "=========="
echo "===Test 5: Display L_WM surface info"
echo "=========="
echo
${trekker} info "$L_WM"


echo
echo
echo "=========="
echo "===Test 6: Display WBT tractogram info"
echo "=========="
echo
${trekker} info "$WBT"


echo
echo
echo "=========="
echo "===Test 7: track2img"
echo "=========="
echo
${trekker} track2img -f "$WBT" "$out_track2img" -v quite

display_file_info "$out_track2img"


echo
echo
echo "=========="
echo "===Test 8: track2surf"
echo "=========="
echo
${trekker} track2surf -f "$WBT" "$L_WM" "$out_track2surf" dens --feature streamlineDensity -v quite

display_file_info "$out_track2surf"



echo
echo
echo "=========="
echo "===Test 9: track"
echo "=========="
echo
${trekker} track -f "$FOD" \
  --seed "$L_WM" \
  --seed_count 100 \
  --output "$out_track" \
  -v quite

display_file_info "$out_track"


echo
echo
echo "=========="
echo "===Test 10: filter #1"
echo "=========="
echo
${trekker} filter -f "$WBT" \
  -p require_entry "$L_LGN" \
  -p require_entry "$L_V1" \
  -v quite \
  "$out_filter_1"

display_file_info "$out_filter_1"



echo
echo
echo "=========="
echo "===Test 11: filter #2"
echo "=========="
echo
${trekker} filter -f "$WBT" \
  -s "$L_LGN" \
  --seed_trials 100 \
  -p require_entry_B "$L_V1" \
  -p stop_at_entry_B "$L_V1" \
  -p stop_after_exit_A "$L_LGN" \
  -v quite \
  "$out_filter_2"

display_file_info "$out_filter_2"


echo
echo
echo "=========="
echo "===Completed tests"
echo "=========="
echo

