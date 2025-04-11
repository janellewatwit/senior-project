#include "SharedMemoryReader.h"


//Weird formatting, but this is for a unique name for our shared memory, should not interfere with other programs
//using same thing. Local = this user, SDSMGshmem = sounds magic shared memory
const wchar_t* MEM_NAME = L"Local\\SDSMGshmem";
const int MAX_AGE = 100;

/*
    This is based on the code from here: https://learn.microsoft.com/en-us/windows/win32/memory/creating-named-shared-memory
    Additionally, Debugging was assisted from GPT due to time constraints on implementation.

    The architecture, insertion points, and testing are what we take credit for.
*/
SharedMemoryReader::SharedMemoryReader(size_t maxSamples, int maxAgeMs)
    : maxSamples(maxSamples), maxAgeMs(maxAgeMs) {

    //Size we expect one unit of data to take
    bufferSize = sizeof(SharedMemoryData);

    //This is the actual region of memory
    hMapFile = OpenFileMapping(FILE_MAP_READ, FALSE, L"Local\\SDSMGshmem");
    if (!hMapFile) {
        std::cerr << "Error opening file mapping: " << GetLastError() << std::endl;
        throw std::runtime_error("Failed to open file mapping.");
    }

    //This is a pointer to the memory, and arrays work as pointers. Which this makes so much more sense now.
    pBuf = MapViewOfFile(hMapFile, FILE_MAP_READ, 0, 0, bufferSize);
    if (!pBuf) {
        std::cerr << "Error mapping view of file: " << GetLastError() << std::endl;
        CloseHandle(hMapFile);
        throw std::runtime_error("Failed to map view of file.");
    }
}

//Destructor
SharedMemoryReader::~SharedMemoryReader() {
    if (pBuf) UnmapViewOfFile(pBuf);
    if (hMapFile) CloseHandle(hMapFile);
}


//This is never used. TODO delete, integrate, or just keep it so some other person can use
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

    //While the above practice is good, in theory, the fastest implementation would be first test
    //timestamp, then to write the data after, since there should be enough time in between writes, or at the very
    //least we already have the new data, even if the timestamp isnt inherintly the same
    //TLDR: The data is already updated and likely safe to use should the bits not be changed mid read
}

//Important because we can check if data is updated outside *Read does what both of these do, whos idea was this?
// (GPT)
uint64_t SharedMemoryReader::getLastUpdateTimestampMs() const {
    if (!pBuf) return 0;
    SharedMemoryData* data = reinterpret_cast<SharedMemoryData*>(pBuf); //Store before anything changes
    //only return lasttimestamp. Chances are, we could probably check this directly? but better safe than sorry.
    return data->lastUpdateTimestampMs;
}

//I like the smaller versions, it makes code digestible. We can dynamically chose how many samples to read at a time
std::vector<float> SharedMemoryReader::getData(size_t numSamples) const {
    //Create a temporary buffer and ensure everything is created properly. Additionally use a fallback
    std::vector<float> result(numSamples, 0.0f);
    if (!pBuf || numSamples > maxSamples || numSamples > MAX_AUDIO_SAMPLES) return result;

    //We create a copy of the buffer data (This is good in case it gets overwritten while reading)
    SharedMemoryData* data = reinterpret_cast<SharedMemoryData*>(pBuf);

    //Populate results with the samples and ONLY audio data.
    memcpy(result.data(), data->audioData, numSamples * sizeof(float));
    return result;
}
