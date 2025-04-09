# Define the C++ compiler
CXX = g++
CXXFLAGS = -Wall -std=c++17 -DUNICODE -D_UNICODE

# Include library flags (if needed, depending on where SDL3 is located)
LDFLAGS = -L./Visualizer/src/lib -lSDL3

# Targets
TARGETS = Writer.exe Reader.exe

# All target rule
all: $(TARGETS)

# Rule to build Writer.exe
#Writer.exe: Writer.cpp SharedMemoryData.h 
#	$(CXX) $(CXXFLAGS) -o Writer.exe Writer.cpp SharedMemoryData.h 

# Rule to build Reader.exe with additional include and lib directories
Reader.exe:
	g++ $(CXXFLAGS) -o Visualizer/shmemtest.exe Visualizer/shmemtest.cpp -IVisualizer/src/include -LVisualizer/src/lib -lSDL3 -DDEBUG
clean:
	del *.exe
