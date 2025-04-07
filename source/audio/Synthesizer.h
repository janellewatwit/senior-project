#pragma once

#include <array>
#include "public.sdk/source/vst/vstparameters.h"
#include "public.sdk/source/vst/vsteventshelper.h"
#include "public.sdk/source/vst/vstaudioeffect.h"
#include "Voice.h"

namespace SoundsMagic::AudioEngine
{
class Synthesizer
{
private:
	std::array<AudioEngine::Voice, AudioEngine::NUM_VOICES> m_voices;
	float m_master_volume = 1.0;

public:
	void setMasterVolume(float volume);
	void setSampleRate(float sampleRate);
	void setOscillatorGain(uint8_t index, float gain);
	void processMIDIEvents(Steinberg::Vst::IEventList* inputEvents);
	void generateAudio(Steinberg::Vst::ProcessData& data);
	uint16_t countActiveVoices();
	void setAttack(float attack);
	void setDecay(float decay);
	void setSustain(float sustain);
	void setRelease(float release);
};
}
