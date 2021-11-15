@SET FOD="data\FOD.nii"
@SET SEEDimag="data\seedImage.nii"
@SET SEEDcoor="data\seedCoordinates.txt"

@echo =================================
@echo DEMO 1: Seed image based tracking
@echo =================================
..\binaries\trekker_win_x64_v0.9.exe ^
-fod %FOD% ^
-seed_image %SEEDimag% ^
-seed_count 100 ^
-enableOutputOverwrite ^
-output win_demo_output1 ^
-verboseLevel 0 ^
-writeColors

@echo:
@echo:
@echo ======================================
@echo DEMO 2: Seed coordinate based tracking
@echo ======================================
..\binaries\trekker_win_x64_v0.9.exe ^
-fod %FOD% ^
-seed_coordinates %SEEDcoor% ^
-enableOutputOverwrite ^
-output win_demo_output2 ^
-verboseLevel 0 ^
-writeColors

@echo:
@echo ======================================
@echo If you can see 'win_demo_output1' and 'win_demo_output2' in the demo folder, all is good.
@echo ======================================
