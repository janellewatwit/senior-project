//------------------------------------------------------------------------
// Copyright(c) 2025 Senior Project.
//------------------------------------------------------------------------

#pragma once

#include "pluginterfaces/base/funknown.h"
#include "pluginterfaces/vst/vsttypes.h"

namespace SeniorProject {
//------------------------------------------------------------------------
static const Steinberg::FUID kSeniorProjectProcessorUID (0xAFB246EE, 0x41A5566B, 0xB4E3934B, 0x44D20DF7);
static const Steinberg::FUID kSeniorProjectControllerUID (0xCAC29C16, 0xB4165729, 0x8EEC2517, 0x81AE3399);

#define SeniorProjectVST3Category "Instrument"

//------------------------------------------------------------------------
} // namespace SeniorProject
