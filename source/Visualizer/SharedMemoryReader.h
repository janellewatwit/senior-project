// SharedMemoryReader.h
#pragma once
#include <windows.h>
#include <vector>
#include <cstdint>

#include "../SharedMemoryData.h"

/*
    See SharedMemoryReader.cpp for more detailed comments and insights
*/

class SharedMemoryReader {
public:
    //We want a max age since if the shared memory stays open, we may read very old data
    SharedMemoryReader(size_t maxSamples, int maxAgeMs = 100);
    ~SharedMemoryReader();

    bool read(float* buffer, size_t numSamples); //Long form safe reader > Should have return changed so we can make better use of it
    uint64_t getLastUpdateTimestampMs() const; //Useful to determine whether the data is updated
    std::vector<float> getData(size_t numSamples) const; //Directly grab the data

private:
    //See SharedMemoryReader.cpp for more context on these
    HANDLE hMapFile = nullptr;
    void* pBuf = nullptr;
    size_t bufferSize = 0;
    size_t maxSamples; //Different sizes cause conflicts in reading, we limit the user
    int maxAgeMs;
};
