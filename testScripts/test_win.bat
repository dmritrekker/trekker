@echo off

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
call "%batchDir%trekker_win.exe" track2img -f "%wbt_tractogram%" "%out_track2img%" -v quite
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
  --verbose quite
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
  --verbose quite
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
  --verbose quite
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
  --verbose quite
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
  --verbose quite
call test_win_disp_file_info.bat "%out_filter_2_surf%"
echo.

echo "=========="
echo "===Test 12: track2surf"
echo "=========="
echo.
call "%batchDir%trekker_win.exe" track2surf -f "%out_filter_2_surf%" "%l_v1_surf%" "%out_track2surf%" dens --feature streamlineDensity -v quite
call test_win_disp_file_info.bat "%out_track2surf%"
echo.

echo "=========="
echo "===Completed tests"
echo "=========="
echo.