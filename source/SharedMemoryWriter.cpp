#include <iostream>
#include <stdexcept>
#include <chrono>
#include <windows.h>

#include "SharedMemoryData.h"

const wchar_t* MEM_NAME = L"Local\\SDSMGshmem";

class SharedMemoryWriter {
public:
    SharedMemoryWriter(const std::string& name, size_t size) {
        bufferSize = size + sizeof(SharedMemoryData); // Include space for the timestamp
        hMapFile = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, bufferSize, MEM_NAME);
        if (!hMapFile) {
            throw std::runtime_error("Failed to create file mapping.");
        }

        pBuf = MapViewOfFile(hMapFile, FILE_MAP_ALL_ACCESS, 0, 0, bufferSize);
        if (!pBuf) {
            CloseHandle(hMapFile);
            throw std::runtime_error("Failed to map view of file.");
        }
    }

    ~SharedMemoryWriter() {
        UnmapViewOfFile(pBuf);
        CloseHandle(hMapFile);
    }

    // Write a single channel buffer
    void write(float* channelBuffer, size_t numSamples) {
        if (!pBuf) return;

        size_t requiredSize = numSamples * sizeof(float);
        if (requiredSize > MAX_DATA_SIZE * sizeof(float)) return; // Prevent overflow

        // Get the pointer to the shared memory structure
        SharedMemoryData* data = reinterpret_cast<SharedMemoryData*>(pBuf);

        // Copy the audio data into the shared memory
        memcpy(data->audioData, channelBuffer, requiredSize);

        // Update the timestamp to the current time
        data->lastUpdateTimestamp = std::chrono::system_clock::now();
    }

private:
    size_t bufferSize;
    HANDLE hMapFile;
    void* pBuf;
};
