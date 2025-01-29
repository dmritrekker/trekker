@echo off

@REM Display help
trekker_win

@REM Run a simple fiber tracking script
trekker_win track -f ^
..\testData\100307_FOD_Order4.nii.gz ^
--seed ..\testData\100307_lh_white.vtk ^
--seed_count 100 ^
--output out_linux.vtk

@REM Display tractogram info
trekker_win info out_win.vtk
