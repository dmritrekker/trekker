@echo off

@REM Get the directory of the current batch file
set "batchDir=%~dp0"

@REM Display help
call "%batchDir%trekker_win.exe"


call "%batchDir%trekker_win.exe" info ..\testData\100307_FOD_Order4.nii.gz

call "%batchDir%trekker_win.exe" info ..\testData\100307_lh_white.vtk

@REM Run a simple fiber tracking script
call "%batchDir%trekker_win.exe" track -f ^
..\testData\100307_FOD_Order4.nii.gz ^
--seed ..\testData\100307_lh_white.vtk ^
--seed_count 100 ^
--output out.vtk ^
-v debug

@REM Display tractogram info
if exist "out.vtk" (
    echo out.vtk exists.
    for %%a in ("out.vtk") do echo File size: %%~za bytes
    call "%batchDir%trekker_win.exe" info out.vtk
) else (
    echo out.vtk does not exist.
)

@REM Display info help
call "%batchDir%trekker_win.exe" info

@REM Display track help
call "%batchDir%trekker_win.exe" track

