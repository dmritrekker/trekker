@echo off
echo Testing trekker_win.exe
set "batchDir=%~dp0"
echo Batch Directory: %batchDir%
"%batchDir%trekker_win.exe" -h
echo Done