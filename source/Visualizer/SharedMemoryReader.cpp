#include <iostream>
#include <chrono>
#include <cstring>
#include <windows.h>
#include <vector>
#include "../SharedMemoryData.h"

const wchar_t* MEM_NAME = L"Local\\SDSMGshmem";
const int MAX_AGE = 100;

class SharedMemoryReader {
public:
    SharedMemoryReader(size_t size) {
        bufferSize = sizeof(SharedMemoryData);
                     // Size of audio data
        hMapFile = OpenFileMapping(FILE_MAP_READ, FALSE, MEM_NAME);
        if (!hMapFile) {
            DWORD error = GetLastError();
            std::cerr << "Error opening file mapping: " << error << std::endl;
            throw std::runtime_error("Failed to open file mapping.");
        }

        pBuf = MapViewOfFile(hMapFile, FILE_MAP_READ, 0, 0, bufferSize);
        if (!pBuf) {
            DWORD error = GetLastError();
            CloseHandle(hMapFile);
            std::cerr << "Error mapping view of file: " << error << std::endl;
            throw std::runtime_error("Failed to map view of file.");
        }
    }

    ~SharedMemoryReader() {
        UnmapViewOfFile(pBuf);
        CloseHandle(hMapFile);
    }

    bool read(float* buffer, size_t numSamples) {
        if (!pBuf) return false;

        // Get pointer to the shared memory structure
        SharedMemoryData* data = reinterpret_cast<SharedMemoryData*>(pBuf);

        // Compare timestamp to check if data is fresh
        auto now = std::chrono::system_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(now - data->lastUpdateTimestamp);

        // Check if data is too old
        if (duration.count() > MAX_AGE) {
            return false; // Data is stale
        }

        // Copy audio data into the buffer
        memcpy(buffer, data->audioData, numSamples * sizeof(float));
        return true;
    }

    std::chrono::system_clock::time_point getLastUpdateTimestamp() const {
        if (!pBuf) return std::chrono::system_clock::time_point::min();
    
        SharedMemoryData* data = reinterpret_cast<SharedMemoryData*>(pBuf);
        return data->lastUpdateTimestamp;
    }

    std::vector<float> getData(size_t numSamples) const {
        std::vector<float> result(numSamples);
        if (!pBuf) return result;
    
        SharedMemoryData* data = reinterpret_cast<SharedMemoryData*>(pBuf);
        memcpy(result.data(), data->audioData, numSamples * sizeof(float));
        return result;
    }

private:
    size_t bufferSize;
    HANDLE hMapFile;
    void* pBuf;
};
