// SharedMemoryData.h
#pragma once
#include <cstdint>

const size_t MAX_AUDIO_SAMPLES = 1024;

struct SharedMemoryData {
    uint32_t versionStart; // Used for consistency check
    uint64_t lastUpdateTimestampMs; // UNIX time in ms
    float audioData[MAX_AUDIO_SAMPLES]; // Interleaved or mono, up to you
    uint32_t versionEnd; // Must match versionStart
};
