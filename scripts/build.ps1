# Build script for Frostform Engine
# Run this from the project root directory

param(
    [string]$BuildType = "Release",
    [switch]$Clean
)

$BuildDir = "build"

if ($Clean) {
    Write-Host "Cleaning build directory..."
    if (Test-Path $BuildDir) {
        Remove-Item -Recurse -Force $BuildDir
    }
}

Write-Host "Creating build directory..."
New-Item -ItemType Directory -Force -Path $BuildDir | Out-Null

Write-Host "Configuring with CMake..."
Push-Location $BuildDir
cmake .. -DCMAKE_BUILD_TYPE=$BuildType

Write-Host "Building project..."
cmake --build . --config $BuildType

Pop-Location

Write-Host "Build complete! Executable is in $BuildDir/bin/$BuildType/"