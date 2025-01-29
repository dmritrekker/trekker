@echo off

@REM Display help
call .\trekker_win.exe

@REM Run a simple fiber tracking script
call .\trekker_win.exe track -f ^
..\testData\100307_FOD_Order4.nii.gz ^
--seed ..\testData\100307_lh_white.vtk ^
--seed_count 100 ^
--output out_linux.vtk

@REM Display tractogram info
call .\trekker_win.exe info out_win.vtk
