# Simple Build Script - No Make Required!
# Just install a C++ compiler (MinGW or Visual Studio)

$ErrorActionPreference = "Stop"

Write-Host "========================================" -ForegroundColor Cyan
Write-Host "Tiny3D - Simple Build (No Make needed)" -ForegroundColor Cyan
Write-Host "========================================" -ForegroundColor Cyan
Write-Host ""

# Check for compilers
$compiler = $null

if (Get-Command g++ -ErrorAction SilentlyContinue) {
    $compiler = "g++"
    Write-Host "Found g++ compiler" -ForegroundColor Green
}
elseif (Get-Command cl -ErrorAction SilentlyContinue) {
    $compiler = "cl"
    Write-Host "Found MSVC compiler" -ForegroundColor Green
}
else {
    Write-Host "ERROR: No C++ compiler found!" -ForegroundColor Red
    Write-Host ""
    Write-Host "Please install one of:" -ForegroundColor Yellow
    Write-Host "  1. MinGW-w64: https://sourceforge.net/projects/mingw-w64/" -ForegroundColor White
    Write-Host "  2. Visual Studio: https://visualstudio.microsoft.com/downloads/" -ForegroundColor White
    Write-Host "  3. Or via Chocolatey: choco install mingw" -ForegroundColor White
    Write-Host ""
    exit 1
}

# Create directories
Write-Host "Creating build directories..." -ForegroundColor Yellow
New-Item -ItemType Directory -Force -Path "build/obj" | Out-Null
New-Item -ItemType Directory -Force -Path "build/lib" | Out-Null
New-Item -ItemType Directory -Force -Path "build/bin" | Out-Null

# Source files
$sources = @(
    "src/animation.cpp",
    "src/canvas.cpp",
    "src/lighting.cpp",
    "src/math3d.cpp",
    "src/renderer.cpp",
    "src/display.cpp",
    "src/window_display.cpp"
)

$objects = @()

# Compile each source file
Write-Host ""
Write-Host "Compiling source files..." -ForegroundColor Yellow

if ($compiler -eq "g++") {
    # GCC/MinGW compilation
    foreach ($src in $sources) {
        $obj = "build/obj/" + [System.IO.Path]::GetFileNameWithoutExtension($src) + ".o"
        $objects += $obj
        Write-Host "  Compiling: $src" -ForegroundColor Gray
        & g++ -std=c++17 -Wall -Wextra -O2 -Iinclude -c $src -o $obj
        if ($LASTEXITCODE -ne 0) {
            Write-Host "Compilation failed for $src" -ForegroundColor Red
            exit 1
        }
    }
    
    # Create static library
    Write-Host ""
    Write-Host "Creating static library..." -ForegroundColor Yellow
    & ar rcs build/lib/libtiny3d.a $objects
    if ($LASTEXITCODE -ne 0) {
        Write-Host "Library creation failed" -ForegroundColor Red
        exit 1
    }
    Write-Host "Library created: build/lib/libtiny3d.a" -ForegroundColor Green
    
    # Build demos
    Write-Host ""
    Write-Host "Building demos..." -ForegroundColor Yellow
    & g++ -std=c++17 -O2 -Iinclude demo/main.cpp build/lib/libtiny3d.a -o build/bin/demo.exe
    if ($LASTEXITCODE -eq 0) {
        Write-Host "Demo built: build/bin/demo.exe" -ForegroundColor Green
    }
    
    & g++ -std=c++17 -O2 -Iinclude demo/test1_lines.cpp build/lib/libtiny3d.a -lgdi32 -o build/bin/test1_lines.exe
    if ($LASTEXITCODE -eq 0) {
        Write-Host "Test 1 built: build/bin/test1_lines.exe" -ForegroundColor Green
    }
    
    & g++ -std=c++17 -O2 -Iinclude demo/test2_3d_static.cpp build/lib/libtiny3d.a -lgdi32 -o build/bin/test2_3d_static.exe
    if ($LASTEXITCODE -eq 0) {
        Write-Host "Test 2 built: build/bin/test2_3d_static.exe" -ForegroundColor Green
    }
    
    & g++ -std=c++17 -O2 -Iinclude demo/test3_3d_animated.cpp build/lib/libtiny3d.a -lgdi32 -o build/bin/test3_3d_animated.exe
    if ($LASTEXITCODE -eq 0) {
        Write-Host "Test 3 built: build/bin/test3_3d_animated.exe" -ForegroundColor Green
    }
    
    # Build tests
    Write-Host ""
    Write-Host "Building tests..." -ForegroundColor Yellow
    & g++ -std=c++17 -O2 -Iinclude tests/test_animation.cpp build/lib/libtiny3d.a -o build/bin/test_animation.exe
    if ($LASTEXITCODE -eq 0) {
        Write-Host "Test built: build/bin/test_animation.exe" -ForegroundColor Green
    }
    
    & g++ -std=c++17 -O2 -Iinclude tests/test_math.cpp build/lib/libtiny3d.a -o build/bin/test_math.exe
    if ($LASTEXITCODE -eq 0) {
        Write-Host "Test built: build/bin/test_math.exe" -ForegroundColor Green
    }
    
}
elseif ($compiler -eq "cl") {
    # MSVC compilation
    foreach ($src in $sources) {
        $obj = "build/obj/" + [System.IO.Path]::GetFileNameWithoutExtension($src) + ".obj"
        $objects += $obj
        Write-Host "  Compiling: $src" -ForegroundColor Gray
        & cl /std:c++17 /W4 /O2 /EHsc /Iinclude /c $src /Fo:$obj
        if ($LASTEXITCODE -ne 0) {
            Write-Host "Compilation failed for $src" -ForegroundColor Red
            exit 1
        }
    }
    
    # Create static library
    Write-Host ""
    Write-Host "Creating static library..." -ForegroundColor Yellow
    & lib /OUT:build/lib/tiny3d.lib $objects
    if ($LASTEXITCODE -ne 0) {
        Write-Host "Library creation failed" -ForegroundColor Red
        exit 1
    }
    Write-Host "Library created: build/lib/tiny3d.lib" -ForegroundColor Green
    
    # Build demos
    Write-Host ""
    Write-Host "Building demos..." -ForegroundColor Yellow
    & cl /std:c++17 /O2 /EHsc /Iinclude demo/main.cpp build/lib/tiny3d.lib /Fe:build/bin/demo.exe
    if ($LASTEXITCODE -eq 0) {
        Write-Host "Demo built: build/bin/demo.exe" -ForegroundColor Green
    }
    
    & cl /std:c++17 /O2 /EHsc /Iinclude demo/interactive.cpp build/lib/tiny3d.lib /Fe:build/bin/interactive.exe
    if ($LASTEXITCODE -eq 0) {
        Write-Host "Interactive demo built: build/bin/interactive.exe" -ForegroundColor Green
    }
    
    # Build tests
    Write-Host ""
    Write-Host "Building tests..." -ForegroundColor Yellow
    & cl /std:c++17 /O2 /EHsc /Iinclude tests/test_animation.cpp build/lib/tiny3d.lib /Fe:build/bin/test_animation.exe
    if ($LASTEXITCODE -eq 0) {
        Write-Host "Test built: build/bin/test_animation.exe" -ForegroundColor Green
    }
    
    & cl /std:c++17 /O2 /EHsc /Iinclude tests/test_math.cpp build/lib/tiny3d.lib /Fe:build/bin/test_math.exe
    if ($LASTEXITCODE -eq 0) {
        Write-Host "Test built: build/bin/test_math.exe" -ForegroundColor Green
    }
}

Write-Host ""
Write-Host "========================================" -ForegroundColor Green
Write-Host "Build completed successfully!" -ForegroundColor Green
Write-Host "========================================" -ForegroundColor Green
Write-Host ""
Write-Host "Run tests in order:" -ForegroundColor Cyan
Write-Host "  1. Test line drawing:" -ForegroundColor Yellow
Write-Host "     .\build\bin\test1_lines.exe" -ForegroundColor White
Write-Host ""
Write-Host "  2. Test static 3D cube:" -ForegroundColor Yellow
Write-Host "     .\build\bin\test2_3d_static.exe" -ForegroundColor White
Write-Host ""
Write-Host "  3. Test animated 3D cube:" -ForegroundColor Yellow
Write-Host "     .\build\bin\test3_3d_animated.exe" -ForegroundColor White
Write-Host ""
Write-Host "To run static demo:" -ForegroundColor Cyan
Write-Host "  .\build\bin\demo.exe" -ForegroundColor White
Write-Host ""
Write-Host "To run tests:" -ForegroundColor Cyan
Write-Host "  .\build\bin\test_animation.exe" -ForegroundColor White
Write-Host "  .\build\bin\test_math.exe" -ForegroundColor White
Write-Host ""
