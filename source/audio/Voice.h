#pragma once
#include "SineWaveTable.h"
#include <array>
#include "Oscillator.h"
#include "constants.h"
#include "ADSR.h"

namespace SoundsMagic::AudioEngine
{
class Voice
{
private:
	std::array<Oscillator, NUM_OSCILLATORS> m_oscillators;

public:
	ADSR envelope = ADSR();
	int32_t m_noteId = -1;

	void playPitch(float pitch);
	void releaseNote();
	void setSampleRate(float sample_rate);
	void setOscillatorGain(uint8_t index, float gain);
	float sample();
};
}
