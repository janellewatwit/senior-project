#pragma once
#include <cstdint>

// DSP
constexpr double PI = 3.141592653589793;

// CONFIG
constexpr uint8_t NUM_VOICES = 8U;
constexpr uint8_t NUM_OSCILLATORS = 64U;
constexpr char* LOGFILE = "C:\\Users\\felix\\dev\\school\\senior-project\\scripts\\profiling\\log.txt";

// PARAMS
constexpr Steinberg::Vst::ParamID VolumeParamID = 100U;
