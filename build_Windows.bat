@SET cmakeExe="C:/Program Files/CMake/bin/cmake"
@SET buildPythonPackage=ON
@SET pythonExe="C:/Program Files (x86)/Microsoft Visual Studio/Shared/Python37_64/python"
@SET buildType=Release

@rmdir build\Windows /s /q
@mkdir build\Windows
@cd build\Windows
@%cmakeExe% -DPython3_EXECUTABLE=%pythonExe% -DBuild_Python3_WRAPPER=%buildPythonPackage% ../..
@%cmakeExe% --build . --config %buildType% --target install --parallel 8
@cd ../..
