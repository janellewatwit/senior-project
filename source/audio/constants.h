#pragma once
#include <cstdint>
#include "public.sdk/source/vst/vstparameters.h"

namespace SoundsMagic::AudioEngine
{
// DSP
constexpr double PI = 3.141592653589793;

// CONFIG
constexpr uint8_t NUM_VOICES = 8U;
constexpr uint8_t NUM_OSCILLATORS = 64U;
constexpr uint8_t WAVETABLE_LOOKUP_BITS = 16U;

// PARAMS
constexpr Steinberg::Vst::ParamID VolumeParamID  = 100U;
constexpr Steinberg::Vst::ParamID AttackParamID  = 101U;
constexpr Steinberg::Vst::ParamID DecayParamID   = 102U;
constexpr Steinberg::Vst::ParamID SustainParamID = 103U;
constexpr Steinberg::Vst::ParamID ReleaseParamID = 104U;
}
