// SharedMemoryWriter.cpp
#include "SharedMemoryWriter.h"

const wchar_t* MEM_NAME = L"Local\\SDSMGshmem";


    SharedMemoryWriter::SharedMemoryWriter(const std::string& name, size_t size) {
        bufferSize = sizeof(SharedMemoryData);

        hMapFile = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, bufferSize, MEM_NAME);
        if (!hMapFile) throw std::runtime_error("Failed to create file mapping.");

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

    void SharedMemoryWriter::write(float* samples, size_t numSamples) {
        if (!pBuf || numSamples > MAX_AUDIO_SAMPLES) return;

        SharedMemoryData* data = reinterpret_cast<SharedMemoryData*>(pBuf);

        // Invalidate while writing
        data->versionStart++;

        memcpy(data->audioData, samples, numSamples * sizeof(float));

        auto nowMs = std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::system_clock::now().time_since_epoch()
        ).count();
        data->lastUpdateTimestampMs = nowMs;

        // Finalize version
        data->versionEnd = data->versionStart;
    }

    
