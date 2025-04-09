// SharedMemoryReader.h
#pragma once
#include <windows.h>
#include <vector>
#include <cstdint>

#include "../SharedMemoryData.h"

class SharedMemoryReader {
public:
    SharedMemoryReader(size_t maxSamples, int maxAgeMs = 100);
    ~SharedMemoryReader();

    bool read(float* buffer, size_t numSamples);
    uint64_t getLastUpdateTimestampMs() const;
    std::vector<float> getData(size_t numSamples) const;

private:
    HANDLE hMapFile = nullptr;
    void* pBuf = nullptr;
    size_t bufferSize = 0;
    size_t maxSamples;
    int maxAgeMs;
};
