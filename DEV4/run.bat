@echo off
set "ROOT=%~dp0"
set "ROOT=%ROOT:~0,-1%"
set "PATH=C:\msys64\ucrt64\bin;C:\msys64\usr\bin;%PATH%"
echo Building project...

cmake -S "%ROOT%" -B "%ROOT%\cmake-build-debug" -DCMAKE_BUILD_TYPE=Debug -DCMAKE_C_COMPILER=C:/msys64/ucrt64/bin/gcc.exe -DCMAKE_CXX_COMPILER=C:/msys64/ucrt64/bin/g++.exe -DCMAKE_MAKE_PROGRAM=C:/msys64/ucrt64/bin/ninja.exe -G Ninja

if %ERRORLEVEL% neq 0 (
    echo CMake configuration failed!
    pause
    exit /b 1
)

cmake --build "%ROOT%\cmake-build-debug" --config Debug

if %ERRORLEVEL% neq 0 (
    echo Build failed!
    pause
    exit /b 1
)

echo Launching...
start "" "%ROOT%\cmake-build-debug\beatbox_64464.exe"
