@echo off

@REM Get the directory of the current batch file
set "batchDir=%~dp0"

@REM --- Configuration ---
set "test_data_dir=%batchDir%..\testData"
set "results_dir=%batchDir%testResults"

@REM Input files
set "fod_image=%test_data_dir%\100307_FOD_Order4.nii.gz"
set "surface_file=%test_data_dir%\100307_lh_white.vtk"
set "tractogram_file=%test_data_dir%\100307_lh_50K.vtk"

@REM Output files
set "track2img_output=%results_dir%\100307_lh_50K.nii.gz"
set "track2surf_output=%results_dir%\100307_lh_white_50K.vtk"
set "fiber_tracking_output=%results_dir%\out.vtk"

@REM @REM --- Function to display file information ---
@REM :display_file_info
@REM set "file=%~1"

@REM if not exist "%file%" (
@REM     echo
@REM     echo %file% does not exist.
@REM     echo
@REM     exit /b 1
@REM )

@REM for %%a in ("%file%") do (
@REM     echo
@REM     echo %file% size: %%~za bytes
@REM )

@REM call "%batchDir%trekker_win.exe" info "%file%"
@REM echo

@REM goto :eof

@REM --- Main Script ---

@REM Create results directory if it doesn't exist
if not exist "%results_dir%" mkdir "%results_dir%"

@REM Display help
echo "Testing help"
call "%batchDir%trekker_win.exe"
echo "Done"
echo.

@REM Display info help
echo "Testing info help"
call "%batchDir%trekker_win.exe" info
echo "Done"
echo.

@REM Display dMRI recon help
echo "Testing dMRI recon help"
call "%batchDir%trekker_win.exe" dMRI recon
echo "Done"
echo.

@REM Display image info
echo "Display image info"
call "%batchDir%trekker_win.exe" info "%fod_image%" -v debug
echo "Done"
echo.

@REM Display surface info
echo "Display surface info"
call "%batchDir%trekker_win.exe" info "%surface_file%" -v debug
echo "Done"
echo.

@REM Display tractogram info
echo "Display tractogram info"
call "%batchDir%trekker_win.exe" info "%tractogram_file%" -v debug
echo "Done"
echo.

@REM track2img
echo "track2img"
call "%batchDir%trekker_win.exe" track2img -f "%tractogram_file%" "%track2img_output%"
echo "Done"
echo.

@REM call :display_file_info "%track2img_output%"

@REM track2surf
echo "track2surf"
call "%batchDir%trekker_win.exe" track2surf -f "%tractogram_file%" "%surface_file%" "%track2surf_output%" dens --feature streamlineDensity
echo "Done"
echo.

@REM call :display_file_info "%track2surf_output%"

@REM Run a simple fiber tracking script
echo "Run a simple fiber tracking script"
call "%batchDir%trekker_win.exe" track -f "%fod_image%" ^
  --seed "%surface_file%" ^
  --seed_count 100 ^
  --output "%fiber_tracking_output%"
echo "Done"
echo.

@REM call :display_file_info "%fiber_tracking_output%"

echo.



