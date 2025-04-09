#pragma once
#include "SharedMemoryData.h"
#include <windows.h>
#include <chrono>
#include <cstring>
#include <stdexcept>


extern const wchar_t* MEM_NAME;

class SharedMemoryWriter{
    public:
        SharedMemoryWriter(const std::string& name, size_t size);
        ~SharedMemoryWriter();

        void write(float* channelBuffer, size_t numSamples);

    private:
        size_t bufferSize;
        HANDLE hMapFile;
        void* pBuf;
};