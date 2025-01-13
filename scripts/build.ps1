# Generate build files if they don't exist
if (-not (Test-Path -Path "./build")) {
	& "$PSScriptRoot\generate.ps1"
}

# Build project
cmake.exe --build "./build"
