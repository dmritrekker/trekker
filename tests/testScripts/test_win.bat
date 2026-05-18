@echo off
setlocal enabledelayedexpansion

@REM Get the directory of the current batch file
set "batchDir=%~dp0"

@REM --- Configuration ---
set "test_data_dir=%batchDir%..\testData"
set "results_dir=%batchDir%testResults"

@REM Input files
set "fod_image=%test_data_dir%\FOD.nii.gz"
set "wbt_tractogram=%test_data_dir%\WBT.vtk"
set "l_lgn_img=%test_data_dir%\L_LGN.nii.gz"
set "l_lgn_surf=%test_data_dir%\L_LGN.vtk"
set "l_v1_img=%test_data_dir%\L_V1.nii.gz"
set "l_v1_surf=%test_data_dir%\L_V1.vtk"

@REM Output files
set "out_track2img=%results_dir%\track2img.nii.gz"
set "out_track2surf=%results_dir%\track2surf.vtk"
set "out_track=%results_dir%\track.vtk"
set "out_filter_1_img=%results_dir%\filter_1_img.vtk"
set "out_filter_1_surf=%results_dir%\filter_1_surf.vtk"
set "out_filter_2_img=%results_dir%\filter_2_img.vtk"
set "out_filter_2_surf=%results_dir%\filter_2_img.vtk"
set "out_purifibre=%results_dir%\purifibre.vtk"

@REM TRX output files
set "out_wbt_trx=%results_dir%\wbt.trx"
set "out_wbt_100_trx=%results_dir%\wbt_100.trx"
set "out_wbt_colored_trx=%results_dir%\wbt_colored.trx"
set "out_wbt_colored_50_trx=%results_dir%\wbt_colored_50.trx"
set "out_wbt_field_trx=%results_dir%\wbt_field.trx"
set "out_wbt_field_50_trx=%results_dir%\wbt_field_50.trx"
set "out_weights_bin=%results_dir%\weights.bin"
set "out_weights_csv=%results_dir%\weights.csv"

@REM --- Main Script ---

@REM Create results directory if it doesn't exist
if not exist "%results_dir%" mkdir "%results_dir%"

echo "=========="
echo "===Test 1: Display Trekker help"
echo "=========="
echo.
call "%batchDir%trekker_win.exe"
echo.

echo "=========="
echo "===Test 2: Display info help"
echo "=========="
echo.
call "%batchDir%trekker_win.exe" info
echo.

echo "=========="
echo "===Test 3: Display dMRI recon help"
echo "=========="
echo.
call "%batchDir%trekker_win.exe" dMRI recon
echo.

echo "=========="
echo "===Test 4: Display FOD image info"
echo "=========="
echo.
call "%batchDir%trekker_win.exe" info "%fod_image%"
echo.

echo "=========="
echo "===Test 5: Display WBT tractogram info"
echo "=========="
echo.
call "%batchDir%trekker_win.exe" info "%wbt_tractogram%"
echo.

echo "=========="
echo "===Test 6: track2img"
echo "=========="
echo.
call "%batchDir%trekker_win.exe" track2img -f "%wbt_tractogram%" "%out_track2img%" -v quiet
call test_win_disp_file_info.bat "%out_track2img%"
echo.

echo "=========="
echo "===Test 7: track"
echo "=========="
echo.
call "%batchDir%trekker_win.exe" track -f "%fod_image%" ^
  --seed "%l_lgn_surf%" ^
  --seed_trials 100 ^
  --seed_count 1000 ^
  --pathway stop_before_exit_A "%l_lgn_surf%" ^
  --pathway require_entry_B "%l_v1_surf%" ^
  --pathway stop_before_exit_B "%l_v1_surf%" ^
  --maxlength 120 ^
  --output "%out_track%" ^
  --verbose quiet
call test_win_disp_file_info.bat "%out_track%"
echo.

echo "=========="
echo "===Test 8: seedless filter with images"
echo "=========="
echo.
call "%batchDir%trekker_win.exe" filter -f "%wbt_tractogram%" ^
  --pathway require_entry "%l_lgn_img%" ^
  --pathway require_entry "%l_v1_img%" ^
  --maxlength 120 ^
  --output "%out_filter_1_img%" ^
  --verbose quiet
call test_win_disp_file_info.bat "%out_filter_1_img%"
echo.

echo "=========="
echo "===Test 9: seedless filter with surfaces"
echo "=========="
echo.
call "%batchDir%trekker_win.exe" filter -f "%wbt_tractogram%" ^
  --pathway require_entry "%l_lgn_surf%" ^
  --pathway require_entry "%l_v1_surf%" ^
  --maxlength 120 ^
  --output "%out_filter_1_surf%" ^
  --verbose quiet
call test_win_disp_file_info.bat "%out_filter_1_surf%"
echo.

echo "=========="
echo "===Test 10: seeded filter with images"
echo "=========="
echo.
call "%batchDir%trekker_win.exe" filter -f "%wbt_tractogram%" ^
  --seed "%l_lgn_img%" ^
  --seed_trials 100 ^
  --pathway stop_before_exit_A "%l_lgn_img%" ^
  --pathway require_entry_B "%l_v1_img%" ^
  --pathway stop_before_exit_B "%l_v1_img%" ^
  --maxlength 120 ^
  --output "%out_filter_2_img%" ^
  --verbose quiet
call test_win_disp_file_info.bat "%out_filter_2_img%"
echo.

echo "=========="
echo "===Test 11: seeded filter with surfaces"
echo "=========="
echo.
call "%batchDir%trekker_win.exe" filter -f "%wbt_tractogram%" ^
  --seed "%l_lgn_surf%" ^
  --seed_trials 100 ^
  --pathway stop_before_exit_A "%l_lgn_surf%" ^
  --pathway require_entry_B "%l_v1_surf%" ^
  --pathway stop_before_exit_B "%l_v1_surf%" ^
  --maxlength 120 ^
  --output "%out_filter_2_surf%" ^
  --verbose quiet
call test_win_disp_file_info.bat "%out_filter_2_surf%"
echo.

echo "=========="
echo "===Test 12: track2surf"
echo "=========="
echo.
call "%batchDir%trekker_win.exe" track2surf -f "%out_filter_2_surf%" "%l_v1_surf%" "%out_track2surf%" dens --feature streamlineDensity -v quiet
call test_win_disp_file_info.bat "%out_track2surf%"
echo.


echo "=========="
echo "===Test 13: purifibre"
echo "=========="
echo.
call "%batchDir%trekker_win.exe" purifibre -f -p 10 "%out_filter_1_surf%" "%out_purifibre%" -v quiet
call test_win_disp_file_info.bat "%out_purifibre%"
echo.


echo "=========="
echo "===Test 14: Convert WBT.vtk to TRX format"
echo "=========="
echo.
call "%batchDir%trekker_win.exe" convert -f "%wbt_tractogram%" "%out_wbt_trx%" -v quiet
call test_win_disp_file_info.bat "%out_wbt_trx%"
echo.

echo "=========="
echo "===Test 15: Select a 100-streamline subset (TRX -> TRX)"
echo "=========="
echo.
call "%batchDir%trekker_win.exe" select -f "%out_wbt_trx%" "%out_wbt_100_trx%" --ordered 1 100 -v quiet
call test_win_disp_file_info.bat "%out_wbt_100_trx%"
echo.

echo "=========="
echo "===Test 16: Display TRX file info (should show format and streamline count)"
echo "=========="
echo.
call "%batchDir%trekker_win.exe" info "%out_wbt_100_trx%"
echo.

echo "=========="
echo "===Test 17: Add color to TRX (DPV field; exercises SIGSEGV fix)"
echo "=========="
echo.
call "%batchDir%trekker_win.exe" addColor -f "%out_wbt_100_trx%" "%out_wbt_colored_trx%" -v quiet
call test_win_disp_file_info.bat "%out_wbt_colored_trx%"
echo.
echo --- TRX info after addColor (should show RGB DPV field) ---
call "%batchDir%trekker_win.exe" info "%out_wbt_colored_trx%"
echo.

echo "=========="
echo "===Test 18: Select TRX -> TRX with DPV field (verify field is forwarded)"
echo "=========="
echo.
call "%batchDir%trekker_win.exe" select -f "%out_wbt_colored_trx%" "%out_wbt_colored_50_trx%" --ordered 1 50 -v quiet
call test_win_disp_file_info.bat "%out_wbt_colored_50_trx%"
echo.
echo --- TRX info after select (RGB DPV field should still be present) ---
call "%batchDir%trekker_win.exe" info "%out_wbt_colored_50_trx%"
echo.

echo "=========="
echo "===Test 19: Import a DPS weight field into TRX"
echo "=========="
echo.
python -c "import struct; f=open(r'%out_weights_bin%','wb'); [f.write(struct.pack('f', i*0.01+1.0)) for i in range(100)]; f.close(); print('Created 100 float32 weights')"
copy /Y "%out_wbt_100_trx%" "%out_wbt_field_trx%" >nul
call "%batchDir%trekker_win.exe" fieldImport "%out_wbt_field_trx%" "%out_weights_bin%" STREAMLINE float 1 weight -v quiet
echo.
echo --- TRX info after fieldImport (should show 'weight' DPS field) ---
call "%batchDir%trekker_win.exe" info "%out_wbt_field_trx%"
echo.

echo "=========="
echo "===Test 20: Select TRX -> TRX with DPS field (verify DPS is forwarded)"
echo "=========="
echo.
call "%batchDir%trekker_win.exe" select -f "%out_wbt_field_trx%" "%out_wbt_field_50_trx%" --ordered 1 50 -v quiet
call test_win_disp_file_info.bat "%out_wbt_field_50_trx%"
echo.
echo --- TRX info after select (weight DPS field should still be present) ---
call "%batchDir%trekker_win.exe" info "%out_wbt_field_50_trx%"
echo.

echo "=========="
echo "===Test 21: Export DPS field from TRX"
echo "=========="
echo.
call "%batchDir%trekker_win.exe" fieldExport -f "%out_wbt_field_50_trx%" "%out_weights_csv%" --fields weight -v quiet
if exist "%out_weights_csv%" (
    for /f "tokens=3" %%i in ('find /c /v "" "%out_weights_csv%"') do set line_count=%%i
    echo Exported weight CSV line count: !line_count! (expected 51: 1 header + 50 values)
) else (
    echo ERROR: %out_weights_csv% was not created
)
echo.

echo "=========="
echo "===Test 22: Remove DPS field from TRX"
echo "=========="
echo.
call "%batchDir%trekker_win.exe" fieldRemove "%out_wbt_field_trx%" weight -v quiet
echo.
echo --- TRX info after fieldRemove (weight DPS field should be gone) ---
call "%batchDir%trekker_win.exe" info "%out_wbt_field_trx%"
echo.


echo "=========="
echo "===Completed tests"
echo "=========="
echo.

exit 0
