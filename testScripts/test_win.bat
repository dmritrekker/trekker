@echo off

@REM Get the directory of the current batch file
set "batchDir=%~dp0"

@REM Display help
echo "Testing help"
call "%batchDir%trekker_win.exe"
echo "Done"

@REM Display image info
echo "Display image info"
call "%batchDir%trekker_win.exe" info ..\testData\100307_FOD_Order4.nii.gz
echo "Done"

@REM Display surface info
echo "Display surface info"
call "%batchDir%trekker_win.exe" info ..\testData\100307_lh_white.vtk
echo "Done"


@REM Run a simple fiber tracking script
echo "Run a simple fiber tracking script"
call "%batchDir%trekker_win.exe" track -f ^
..\testData\100307_FOD_Order4.nii.gz ^
--seed ..\testData\100307_lh_white.vtk ^
--seed_count 100 ^
--output out.vtk
echo "Done"

@REM Display tractogram info
if exist "out.vtk" (
    echo out.vtk exists.
    for %%a in ("out.vtk") do echo File size: %%~za bytes
    call "%batchDir%trekker_win.exe" info out.vtk
) else (
    echo out.vtk does not exist.
)

@REM Display info help
echo "Testing info help"
call "%batchDir%trekker_win.exe" info
echo "Done"

@REM Display track help
echo "Testing track help"
call "%batchDir%trekker_win.exe" track
echo "Done"

