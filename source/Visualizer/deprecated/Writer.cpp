#include <windows.h>
#include <iostream>
#include <stdexcept>
#include <chrono>
#include <thread>
#include "SharedMemoryData.h"
#include <math.h>
#include <vector>
#include <random>

// Test class created by AI
const wchar_t* MEM_NAME = L"Local\\SDSMGshmem";

std::vector<float> generateSineWave(size_t count, float frequency, float sampleRate) {
    std::vector<float> samples(count);
    float increment = 2.0f * 3.141519f * frequency / sampleRate;

    for (size_t i = 0; i < count; ++i) {
        samples[i] = std::sin(increment * i);
    }

    return samples;
}

int main() {
    HANDLE hMapFile = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, MEM_NAME); // Open existing shared memory
    if (!hMapFile) {
        // If not found, create it
        std::cerr << "Shared memory does not exist. Creating new shared memory..." << std::endl;
        hMapFile = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0,
                                      sizeof(SharedMemoryData), MEM_NAME);
        if (!hMapFile) {
            std::cerr << "Failed to create file mapping." << std::endl;
            return 1;
        }
    }

    void* pBuf = MapViewOfFile(hMapFile, FILE_MAP_ALL_ACCESS, 0, 0, sizeof(SharedMemoryData));
    if (!pBuf) {
        std::cerr << "Failed to map view of file." << std::endl;
        CloseHandle(hMapFile);
        return 1;
    }

    SharedMemoryData* data = reinterpret_cast<SharedMemoryData*>(pBuf);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> freqDist(50.0f, 20000.0f); // Range: 50Hz to 20kHz

    while (true) {
        float randomFrequency = freqDist(gen);
        std::vector<float> samples = generateSineWave(1024, randomFrequency, 44100);

        // Copy samples to shared memory
        memcpy(data->audioData, samples.data(), samples.size() * sizeof(float));
        data->lastUpdateTimestamp = std::chrono::system_clock::now();

        std::cout << "Wrote sine wave @ " << randomFrequency << " Hz to shared memory." << std::endl;

        std::this_thread::sleep_for(std::chrono::seconds(1)); // Update once per second
    }

    UnmapViewOfFile(pBuf);
    CloseHandle(hMapFile);

    return 0;
}
