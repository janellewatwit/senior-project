// SharedMemoryData.h
#pragma once
#include <cstdint>

const size_t MAX_AUDIO_SAMPLES = 1024;

//Original verison using chronotime was weird, GPT recommended this

struct SharedMemoryData {
    uint32_t versionStart; // Used for consistency check
    uint64_t lastUpdateTimestampMs; // UNIX time in ms
    float audioData[MAX_AUDIO_SAMPLES]; // Storage 
    uint32_t versionEnd; // Must match versionStart
};
