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


# Make sure trekker_linux is executable
trekker=./trekker_macOS

chmod +x ${trekker}

# --- Configuration ---
test_data_dir="../testData"
results_dir="testResults"

# Input files
FOD="$test_data_dir/FOD.nii.gz"
WBT="$test_data_dir/WBT.vtk"
L_LGN_IMG="$test_data_dir/L_LGN.nii.gz"
L_LGN_SURF="$test_data_dir/L_LGN.vtk"
L_V1_IMG="$test_data_dir/L_V1.nii.gz"
L_V1_SURF="$test_data_dir/L_V1.vtk"

# Output files
out_track2img="$results_dir/track2img.nii.gz"
out_track2surf="$results_dir/track2surf.vtk"
out_track="$results_dir/track.vtk"
out_filter_1_img="$results_dir/filter_1_img.vtk"
out_filter_1_surf="$results_dir/filter_1_surf.vtk"
out_filter_2_img="$results_dir/filter_2_img.vtk"
out_filter_2_surf="$results_dir/filter_2_surf.vtk"

# --- Function to display file information ---
display_file_info() {
  local file="$1"

  if [ -z "$file" ]; then
    echo "Error: No file specified."
    echo "Usage: $0 <file>"
    return 1
  fi

  if [ -f "$file" ]; then
    file_size=$(stat -f %z "$file")

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
echo "===Test 5: Display WBT tractogram info"
echo "=========="
echo
${trekker} info "$WBT"



echo
echo
echo "=========="
echo "===Test 6: track2img"
echo "=========="
echo
${trekker} track2img -f "$WBT" "$out_track2img" -v quiet

display_file_info "$out_track2img"





echo
echo
echo "=========="
echo "===Test 7: track"
echo "=========="
echo
${trekker} track -f "$FOD" \
  --seed "$L_LGN_SURF" \
  --seed_trials 100 \
  --seed_count 1000 \
  --pathway stop_before_exit_A "$L_LGN_SURF" \
  --pathway require_entry_B "$L_V1_SURF" \
  --pathway stop_before_exit_B "$L_V1_SURF" \
  --maxlength 120 \
  --output "$out_track" \
  --verbose quiet

display_file_info "$out_track"



echo
echo
echo "=========="
echo "===Test 8: seedless filter with images"
echo "=========="
echo

${trekker} filter -f "$WBT" \
  --pathway require_entry "$L_LGN_IMG" \
  --pathway require_entry "$L_V1_IMG" \
  --maxlength 120 \
  --output "$out_filter_1_img" \
  --verbose quiet

display_file_info "$out_filter_1_img"



echo
echo
echo "=========="
echo "===Test 9: seedless filter with surfaces"
echo "=========="
echo

${trekker} filter -f "$WBT" \
  --pathway require_entry "$L_LGN_SURF" \
  --pathway require_entry "$L_V1_SURF" \
  --maxlength 120 \
  --output "$out_filter_1_surf" \
  --verbose quiet

display_file_info "$out_filter_1_surf"



echo
echo
echo "=========="
echo "===Test 10: seeded filter with images"
echo "=========="
echo
${trekker} filter -f "$WBT" \
  --seed "$L_LGN_IMG" \
  --seed_trials 100 \
  --pathway stop_before_exit_A "$L_LGN_IMG" \
  --pathway require_entry_B "$L_V1_IMG" \
  --pathway stop_before_exit_B "$L_V1_IMG" \
  --maxlength 120 \
  --output "$out_filter_2_img" \
  --verbose quiet

display_file_info "$out_filter_2_img"



echo
echo
echo "=========="
echo "===Test 11: seeded filter with surfaces"
echo "=========="
echo
${trekker} filter -f "$WBT" \
  --seed "$L_LGN_SURF" \
  --seed_trials 100 \
  --pathway stop_before_exit_A "$L_LGN_SURF" \
  --pathway require_entry_B "$L_V1_SURF" \
  --pathway stop_before_exit_B "$L_V1_SURF" \
  --maxlength 120 \
  --output "$out_filter_2_surf" \
  --verbose quiet

display_file_info "$out_filter_2_surf"


echo
echo
echo "=========="
echo "===Test 12: track2surf"
echo "=========="
echo
${trekker} track2surf -f "$out_filter_2_surf" "$L_V1_SURF" "$out_track2surf" dens --feature streamlineDensity -v quiet

display_file_info "$out_track2surf"






echo
echo
echo "=========="
echo "===Completed tests"
echo "=========="
echo



