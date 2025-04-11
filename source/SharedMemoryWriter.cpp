// SharedMemoryWriter.cpp
#include "SharedMemoryWriter.h"

/*
    Based on MS documentation from here: https://learn.microsoft.com/en-us/windows/win32/memory/creating-named-shared-memory
    I use more detail in sharedmemoryreader.

    Debugging and implementation partly done with Chat GPT - If I have learned anyhting, while chatgpt is a great tool, by
    god vibe coding is sort of abysmal, but it can provide great insight!
*/

const wchar_t* MEM_NAME = L"Local\\SDSMGshmem";

    //epic constructor
    SharedMemoryWriter::SharedMemoryWriter(const std::string& name, size_t size) {
        //Size of one unit of an array
        bufferSize = sizeof(SharedMemoryData);

        //Actual location where shared mem is stored
        hMapFile = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, bufferSize, MEM_NAME);
        if (!hMapFile) throw std::runtime_error("Failed to create file mapping.");

        //Sort of the array, this is a pointer
        pBuf = MapViewOfFile(hMapFile, FILE_MAP_ALL_ACCESS, 0, 0, bufferSize);
        if (!pBuf) {
            CloseHandle(hMapFile);
            throw std::runtime_error("Failed to map view of file.");
        }

    }

    SharedMemoryWriter::~SharedMemoryWriter() {
        if (pBuf) UnmapViewOfFile(pBuf);
        if (hMapFile) CloseHandle(hMapFile);
    }

    //All encapsulated, and since there is no reading on this end we do not need to worry much about safety
    void SharedMemoryWriter::write(float* samples, size_t numSamples) {
        if (!pBuf || numSamples > MAX_AUDIO_SAMPLES) return;

        SharedMemoryData* data = reinterpret_cast<SharedMemoryData*>(pBuf);

        // Invalidate while writing
        data->versionStart++;

        //copy the provided data to the buffer
        memcpy(data->audioData, samples, numSamples * sizeof(float));

        //get time so we can check in future
        auto nowMs = std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::system_clock::now().time_since_epoch()
        ).count();
        data->lastUpdateTimestampMs = nowMs;

        // Finalize version - This does somethign similar to the timestamp, its not used except for in read, which is the safer version
        data->versionEnd = data->versionStart;
    }

    
