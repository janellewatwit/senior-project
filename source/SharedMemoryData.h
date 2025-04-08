#include <chrono>
#include <cstring>

const int MAX_DATA_SIZE = 1024;

struct SharedMemoryData {
    float audioData[MAX_DATA_SIZE]; // Array to hold audio data
    std::chrono::system_clock::time_point lastUpdateTimestamp; // Timestamp for last update
};
