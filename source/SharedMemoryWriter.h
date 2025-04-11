#pragma once
#include "SharedMemoryData.h"
#include <windows.h>
#include <chrono>
#include <cstring>
#include <stdexcept>


extern const wchar_t* MEM_NAME;

//This is where we interface OUTWARDS, the name writer
class SharedMemoryWriter{
    public:
        //create instance
        SharedMemoryWriter(const std::string& name, size_t size);
        ~SharedMemoryWriter();

        //create safe writer
        void write(float* channelBuffer, size_t numSamples);

    private:
        size_t bufferSize; //size of one unit of data, multiply by size for total array in bytes
        HANDLE hMapFile; //where shared mem is stored
        void* pBuf; //This is the pointer, again it works as an array for simplicity

        //I think a largely better explaination is we have all these pointers. We start with the pointer owned by VSTProcess
        //and then we tell pBuf to hold that pointer. Now, pbuf exists within the shared memory, and is the entry point.
        //for how the data is copied into shared memory Then on the
        //reader side, we know where this shared memory is, and we copy from an identical reference
};