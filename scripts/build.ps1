# Generate build files if they don't exist
if (-not (Test-Path -Path "./build")) {
	& "$PSScriptRoot\generate.ps1"
}

# Build project
cmake.exe --build "./build"


#Create Visualizer
# Define directory for Visualizer
$VISUALIZER_DIR = "source/Visualizer"

# Navigate to the Visualizer directory
Set-Location -Path $VISUALIZER_DIR

# Compiler flags
$CXX = "g++"
$CXXFLAGS = @("-Wall", "-std=c++17", "-DUNICODE", "-D_UNICODE")

# Output executable
$OUTPUT = "shmemtest.exe"

# Compile the source files (run from inside the Visualizer directory)
& $CXX $CXXFLAGS -o $OUTPUT "shmemtest.cpp" -I"src/include" -L"src/lib" -lSDL3 -DDEBUG

Set-Location -Path "../../"

