@echo off
REM Simple Build Script - No Make Required!
REM Just install a C++ compiler (MinGW or Visual Studio)

echo ========================================
echo Tiny3D - Simple Build (No Make needed)
echo ========================================
echo.

REM Check for g++
where g++ >nul 2>nul
if %ERRORLEVEL% EQU 0 goto :build_gcc

REM Check for cl (MSVC)
where cl >nul 2>nul
if %ERRORLEVEL% EQU 0 goto :build_msvc

echo ERROR: No C++ compiler found!
echo.
echo Please install one of:
echo   1. MinGW-w64: https://sourceforge.net/projects/mingw-w64/
echo   2. Visual Studio: https://visualstudio.microsoft.com/downloads/
echo   3. Or via Chocolatey: choco install mingw
echo.
pause
exit /b 1

:build_gcc
echo Found g++ compiler
echo.

REM Create directories
if not exist "build\obj" mkdir build\obj
if not exist "build\lib" mkdir build\lib
if not exist "build\bin" mkdir build\bin

echo Compiling source files...
g++ -std=c++17 -Wall -Wextra -O2 -Iinclude -c src/animation.cpp -o build/obj/animation.o
g++ -std=c++17 -Wall -Wextra -O2 -Iinclude -c src/canvas.cpp -o build/obj/canvas.o
g++ -std=c++17 -Wall -Wextra -O2 -Iinclude -c src/lighting.cpp -o build/obj/lighting.o
g++ -std=c++17 -Wall -Wextra -O2 -Iinclude -c src/math3d.cpp -o build/obj/math3d.o
g++ -std=c++17 -Wall -Wextra -O2 -Iinclude -c src/renderer.cpp -o build/obj/renderer.o
g++ -std=c++17 -Wall -Wextra -O2 -Iinclude -c src/display.cpp -o build/obj/display.o

if %ERRORLEVEL% NEQ 0 (
    echo Compilation failed!
    pause
    exit /b 1
)

echo.
echo Creating static library...
ar rcs build/lib/libtiny3d.a build/obj/*.o
echo Library created: build/lib/libtiny3d.a

echo.
echo Building demos...
g++ -std=c++17 -O2 -Iinclude demo/main.cpp build/lib/libtiny3d.a -o build/bin/demo.exe
echo Demo built: build/bin/demo.exe
g++ -std=c++17 -O2 -Iinclude demo/interactive.cpp build/lib/libtiny3d.a -o build/bin/interactive.exe
echo Interactive demo built: build/bin/interactive.exe

echo.
echo Building tests...
g++ -std=c++17 -O2 -Iinclude tests/test_animation.cpp build/lib/libtiny3d.a -o build/bin/test_animation.exe
g++ -std=c++17 -O2 -Iinclude tests/test_math.cpp build/lib/libtiny3d.a -o build/bin/test_math.exe
echo Tests built!

goto :success

:build_msvc
echo Found MSVC compiler
echo.

REM Create directories
if not exist "build\obj" mkdir build\obj
if not exist "build\lib" mkdir build\lib
if not exist "build\bin" mkdir build\bin

echo Compiling source files...
cl /std:c++17 /W4 /O2 /EHsc /Iinclude /c src/animation.cpp /Fo:build/obj/animation.obj
cl /std:c++17 /W4 /O2 /EHsc /Iinclude /c src/canvas.cpp /Fo:build/obj/canvas.obj
cl /std:c++17 /W4 /O2 /EHsc /Iinclude /c src/lighting.cpp /Fo:build/obj/lighting.obj
cl /std:c++17 /W4 /O2 /EHsc /Iinclude /c src/display.cpp /Fo:build/obj/display.obj
cl /std:c++17 /W4 /O2 /EHsc /Iinclude /c src/math3d.cpp /Fo:build/obj/math3d.obj
cl /std:c++17 /W4 /O2 /EHsc /Iinclude /c src/renderer.cpp /Fo:build/obj/renderer.obj

if %ERRORLEVEL% NEQ 0 (
    echo Compilation failed!
    pause
    exit /b 1
)

echo.
echo Creating static library...
lib /OUT:build/lib/tiny3d.lib build/obj/*.obj
echo Library created: build/lib/tiny3d.lib

echo.s...
cl /std:c++17 /O2 /EHsc /Iinclude demo/main.cpp build/lib/tiny3d.lib /Fe:build/bin/demo.exe
echo Demo built: build/bin/demo.exe
cl /std:c++17 /O2 /EHsc /Iinclude demo/interactive.cpp build/lib/tiny3d.lib /Fe:build/bin/interactive.exe
echo Interactive demo built: build/bin/interactivede demo/main.cpp build/lib/tiny3d.lib /Fe:build/bin/demo.exe
echo Demo built: build/bin/demo.exe

echo.
echo Building tests...
cl /std:c++17 /O2 /EHsc /Iinclude tests/test_animation.cpp build/lib/tiny3d.lib /Fe:build/bin/test_animation.exe
cl /std:c++17 /O2 /EHsc /Iinclude tests/test_math.cpp build/lib/tiny3d.lib /Fe:build/bin/test_math.exe
echo Tests built!

goto :success

:success
echo.
echo ========================================
echo Build cinteractive demo: build\bin\interactive.exe
echo To run static ompleted successfully!
echo ========================================
echo.
echo To run demo: build\bin\demo.exe
echo To run tests: build\bin\test_animation.exe
echo               build\bin\test_math.exe
echo.
pause
