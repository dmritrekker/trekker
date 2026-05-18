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


# Make sure trekker_macOS is executable
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
out_purifibre="$results_dir/purifibre.vtk"

# TRX output files
out_wbt_trx="$results_dir/wbt.trx"
out_wbt_100_trx="$results_dir/wbt_100.trx"
out_wbt_colored_trx="$results_dir/wbt_colored.trx"
out_wbt_colored_50_trx="$results_dir/wbt_colored_50.trx"
out_wbt_field_trx="$results_dir/wbt_field.trx"
out_wbt_field_50_trx="$results_dir/wbt_field_50.trx"
out_weights_bin="$results_dir/weights.bin"
out_weights_csv="$results_dir/weights.csv"

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
echo "===Test 13: purifibre"
echo "=========="
echo
${trekker} purifibre -f -p 10 "$out_filter_1_surf" "$out_purifibre" -v quiet

display_file_info "$out_purifibre"




echo
echo
echo "=========="
echo "===Test 14: Convert WBT.vtk to TRX format"
echo "=========="
echo
${trekker} convert -f "$WBT" "$out_wbt_trx" -v quiet

display_file_info "$out_wbt_trx"


echo
echo
echo "=========="
echo "===Test 15: Select a 100-streamline subset (TRX -> TRX)"
echo "=========="
echo
${trekker} select -f "$out_wbt_trx" "$out_wbt_100_trx" --ordered 1 100 -v quiet

display_file_info "$out_wbt_100_trx"


echo
echo
echo "=========="
echo "===Test 16: Display TRX file info (should show format and streamline count)"
echo "=========="
echo
${trekker} info "$out_wbt_100_trx"


echo
echo
echo "=========="
echo "===Test 17: Add color to TRX (DPV field; exercises SIGSEGV fix)"
echo "=========="
echo
${trekker} addColor -f "$out_wbt_100_trx" "$out_wbt_colored_trx" -v quiet

display_file_info "$out_wbt_colored_trx"

echo
echo "--- TRX info after addColor (should show RGB DPV field) ---"
${trekker} info "$out_wbt_colored_trx"


echo
echo
echo "=========="
echo "===Test 18: Select TRX -> TRX with DPV field (verify field is forwarded)"
echo "=========="
echo
${trekker} select -f "$out_wbt_colored_trx" "$out_wbt_colored_50_trx" --ordered 1 50 -v quiet

display_file_info "$out_wbt_colored_50_trx"

echo
echo "--- TRX info after select (RGB DPV field should still be present) ---"
${trekker} info "$out_wbt_colored_50_trx"


echo
echo
echo "=========="
echo "===Test 19: Import a DPS weight field into TRX"
echo "=========="
echo
python3 -c "import struct; f=open('$out_weights_bin','wb'); [f.write(struct.pack('f', i*0.01+1.0)) for i in range(100)]; f.close(); print('Created 100 float32 weights')"
cp "$out_wbt_100_trx" "$out_wbt_field_trx"
${trekker} fieldImport "$out_wbt_field_trx" "$out_weights_bin" STREAMLINE float 1 weight -v quiet

echo
echo "--- TRX info after fieldImport (should show 'weight' DPS field) ---"
${trekker} info "$out_wbt_field_trx"


echo
echo
echo "=========="
echo "===Test 20: Select TRX -> TRX with DPS field (verify DPS is forwarded)"
echo "=========="
echo
${trekker} select -f "$out_wbt_field_trx" "$out_wbt_field_50_trx" --ordered 1 50 -v quiet

display_file_info "$out_wbt_field_50_trx"

echo
echo "--- TRX info after select (weight DPS field should still be present) ---"
${trekker} info "$out_wbt_field_50_trx"


echo
echo
echo "=========="
echo "===Test 21: Export DPS field from TRX"
echo "=========="
echo
${trekker} fieldExport -f "$out_wbt_field_50_trx" "$out_weights_csv" --fields weight -v quiet

if [ -f "$out_weights_csv" ]; then
  line_count=$(wc -l < "$out_weights_csv")
  echo "Exported weight CSV line count: $line_count (expected 51: 1 header + 50 values)"
else
  echo "ERROR: $out_weights_csv was not created"
fi


echo
echo
echo "=========="
echo "===Test 22: Remove DPS field from TRX"
echo "=========="
echo
${trekker} fieldRemove "$out_wbt_field_trx" weight -v quiet

echo
echo "--- TRX info after fieldRemove (weight DPS field should be gone) ---"
${trekker} info "$out_wbt_field_trx"


echo
echo
echo "=========="
echo "===Completed tests"
echo "=========="
echo



