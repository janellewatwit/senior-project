#include "SharedMemoryReader.h"

const wchar_t* MEM_NAME = L"Local\\SDSMGshmem";
const int MAX_AGE = 100;

SharedMemoryReader::SharedMemoryReader(size_t maxSamples, int maxAgeMs)
    : maxSamples(maxSamples), maxAgeMs(maxAgeMs) {

    bufferSize = sizeof(SharedMemoryData);

    hMapFile = OpenFileMapping(FILE_MAP_READ, FALSE, L"Local\\SDSMGshmem");
    if (!hMapFile) {
        std::cerr << "Error opening file mapping: " << GetLastError() << std::endl;
        throw std::runtime_error("Failed to open file mapping.");
    }

    pBuf = MapViewOfFile(hMapFile, FILE_MAP_READ, 0, 0, bufferSize);
    if (!pBuf) {
        std::cerr << "Error mapping view of file: " << GetLastError() << std::endl;
        CloseHandle(hMapFile);
        throw std::runtime_error("Failed to map view of file.");
    }
}

SharedMemoryReader::~SharedMemoryReader() {
    if (pBuf) UnmapViewOfFile(pBuf);
    if (hMapFile) CloseHandle(hMapFile);
}

bool SharedMemoryReader::read(float* buffer, size_t numSamples) {
    if (!pBuf || numSamples > maxSamples || numSamples > MAX_AUDIO_SAMPLES) {
        return false;
    }

    SharedMemoryData* data = reinterpret_cast<SharedMemoryData*>(pBuf);

    uint32_t versionStart, versionEnd;
    uint64_t timestamp;

    // Read in a safe order
    do {
        versionStart = data->versionStart;
        memcpy(buffer, data->audioData, numSamples * sizeof(float));
        timestamp = data->lastUpdateTimestampMs;
        versionEnd = data->versionEnd;
    } while (versionStart != versionEnd); // Retry if a write was in progress

    auto nowMs = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()
    ).count();

    if ((nowMs - timestamp) > maxAgeMs) {
        return false; // Stale
    }

    return true;
}

uint64_t SharedMemoryReader::getLastUpdateTimestampMs() const {
    if (!pBuf) return 0;
    SharedMemoryData* data = reinterpret_cast<SharedMemoryData*>(pBuf);
    return data->lastUpdateTimestampMs;
}

std::vector<float> SharedMemoryReader::getData(size_t numSamples) const {
    std::vector<float> result(numSamples, 0.0f);
    if (!pBuf || numSamples > maxSamples || numSamples > MAX_AUDIO_SAMPLES) return result;

    SharedMemoryData* data = reinterpret_cast<SharedMemoryData*>(pBuf);

    // Optional: add consistency check if needed
    memcpy(result.data(), data->audioData, numSamples * sizeof(float));
    return result;
}
