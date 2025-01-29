@echo off

@REM Display help
call .\trekker_win.exe

@REM Run a simple fiber tracking script
call .\trekker_win.exe track -f ^
..\testData\100307_FOD_Order4.nii.gz ^
--seed ..\testData\100307_lh_white.vtk ^
--seed_count 100 ^
--output out_win.vtk

@REM Display tractogram info
if exist "out_win.vtk" (
    echo out_win.vtk exists.
    for %%a in ("out_win.vtk") do echo File size: %%~za bytes
    call .\trekker_win.exe info out_win.vtk
) else (
    echo out_win.vtk does not exist.
)

@REM Display info help
call .\trekker_win.exe info

@REM Display track help
call .\trekker_win.exe track

