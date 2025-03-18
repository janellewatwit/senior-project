//------------------------------------------------------------------------
// Copyright(c) 2025 Sounds Magic.
//------------------------------------------------------------------------

#pragma once

#include "pluginterfaces/base/funknown.h"
#include "pluginterfaces/vst/vsttypes.h"

namespace SoundsMagic {
//------------------------------------------------------------------------
static const Steinberg::FUID kSoundsMagicProcessorUID (0xAFB246EE, 0x41A5566B, 0xB4E3934B, 0x44D20DF7);
static const Steinberg::FUID kSoundsMagicControllerUID (0xCAC29C16, 0xB4165729, 0x8EEC2517, 0x81AE3399);

#define SoundsMagicVST3Category "Instrument"

//------------------------------------------------------------------------
} // namespace SoundsMagic
