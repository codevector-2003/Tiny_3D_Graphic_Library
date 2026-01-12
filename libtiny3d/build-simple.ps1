# Simple Build Script - No Make Required!
# Just install a C++ compiler (MinGW or Visual Studio)

$ErrorActionPreference = "Stop"

Write-Host "========================================" -ForegroundColor Cyan
Write-Host "Tiny3D - Simple Build (No Make needed)" -ForegroundColor Cyan
Write-Host "========================================" -ForegroundColor Cyan
Write-Host ""

# Check for compilers
$compiler = $null
$isWindows = $true

if (Get-Command g++ -ErrorAction SilentlyContinue) {
    $compiler = "g++"
    Write-Host "✓ Found g++ compiler" -ForegroundColor Green
}
elseif (Get-Command cl -ErrorAction SilentlyContinue) {
    $compiler = "cl"
    Write-Host "✓ Found MSVC compiler" -ForegroundColor Green
}
else {
    Write-Host "✗ No C++ compiler found!" -ForegroundColor Red
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
    "src/renderer.cpp"
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
            Write-Host "✗ Compilation failed for $src" -ForegroundColor Red
            exit 1
        }
    }
    
    # Create static library
    Write-Host ""
    Write-Host "Creating static library..." -ForegroundColor Yellow
    & ar rcs build/lib/libtiny3d.a $objects
    if ($LASTEXITCODE -ne 0) {
        Write-Host "✗ Library creation failed" -ForegroundColor Red
        exit 1
    }
    Write-Host "✓ Library created: build/lib/libtiny3d.a" -ForegroundColor Green
    
    # Build demo
    Write-Host ""
    Write-Host "Building demo..." -ForegroundColor Yellow
    & g++ -std=c++17 -O2 -Iinclude demo/main.cpp build/lib/libtiny3d.a -o build/bin/demo.exe
    if ($LASTEXITCODE -eq 0) {
        Write-Host "✓ Demo built: build/bin/demo.exe" -ForegroundColor Green
    }
    
    # Build tests
    Write-Host ""
    Write-Host "Building tests..." -ForegroundColor Yellow
    & g++ -std=c++17 -O2 -Iinclude tests/test_animation.cpp build/lib/libtiny3d.a -o build/bin/test_animation.exe
    if ($LASTEXITCODE -eq 0) {
        Write-Host "✓ Test built: build/bin/test_animation.exe" -ForegroundColor Green
    }
    
    & g++ -std=c++17 -O2 -Iinclude tests/test_math.cpp build/lib/libtiny3d.a -o build/bin/test_math.exe
    if ($LASTEXITCODE -eq 0) {
        Write-Host "✓ Test built: build/bin/test_math.exe" -ForegroundColor Green
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
            Write-Host "✗ Compilation failed for $src" -ForegroundColor Red
            exit 1
        }
    }
    
    # Create static library
    Write-Host ""
    Write-Host "Creating static library..." -ForegroundColor Yellow
    & lib /OUT:build/lib/tiny3d.lib $objects
    if ($LASTEXITCODE -ne 0) {
        Write-Host "✗ Library creation failed" -ForegroundColor Red
        exit 1
    }
    Write-Host "✓ Library created: build/lib/tiny3d.lib" -ForegroundColor Green
    
    # Build demo
    Write-Host ""
    Write-Host "Building demo..." -ForegroundColor Yellow
    & cl /std:c++17 /O2 /EHsc /Iinclude demo/main.cpp build/lib/tiny3d.lib /Fe:build/bin/demo.exe
    if ($LASTEXITCODE -eq 0) {
        Write-Host "✓ Demo built: build/bin/demo.exe" -ForegroundColor Green
    }
    
    # Build tests
    Write-Host ""
    Write-Host "Building tests..." -ForegroundColor Yellow
    & cl /std:c++17 /O2 /EHsc /Iinclude tests/test_animation.cpp build/lib/tiny3d.lib /Fe:build/bin/test_animation.exe
    if ($LASTEXITCODE -eq 0) {
        Write-Host "✓ Test built: build/bin/test_animation.exe" -ForegroundColor Green
    }
    
    & cl /std:c++17 /O2 /EHsc /Iinclude tests/test_math.cpp build/lib/tiny3d.lib /Fe:build/bin/test_math.exe
    if ($LASTEXITCODE -eq 0) {
        Write-Host "✓ Test built: build/bin/test_math.exe" -ForegroundColor Green
    }
}

Write-Host ""
Write-Host "========================================" -ForegroundColor Green
Write-Host "✓ Build completed successfully!" -ForegroundColor Green
Write-Host "========================================" -ForegroundColor Green
Write-Host ""
Write-Host "To run demo:" -ForegroundColor Cyan
Write-Host "  .\build\bin\demo.exe" -ForegroundColor White
Write-Host ""
Write-Host "To run tests:" -ForegroundColor Cyan
Write-Host "  .\build\bin\test_animation.exe" -ForegroundColor White
Write-Host "  .\build\bin\test_math.exe" -ForegroundColor White
Write-Host ""
