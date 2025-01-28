@echo off

@REM Windows build example

set buildType=Release
set buildShared=OFF
set buildDir=build-static

if not exist %buildDir% mkdir %buildDir%
pushd %buildDir%

call "C:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Auxiliary\Build\vcvars64.bat"
cmake ^
  -DCMAKE_BUILD_TYPE=%buildType% ^
  -DBUILD_SHARED_LIBS=%buildShared% ^
  ..

@REM Determine number of logical processors instead of physical cores
set num_cores=
for /f "tokens=2 delims==" %%i in ('wmic cpu get numberoflogicalprocessors /value ^| find "="') do set num_cores=%%i

cmake --build . --config %buildType% --target install --parallel %num_cores%

popd
