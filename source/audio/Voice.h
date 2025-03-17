#pragma once
#include "SineWaveTable.h"
#include <array>
#include "Oscillator.h"
#include "constants.h"

class Voice
{
private:
	std::array<Oscillator, NUM_OSCILLATORS> m_oscillators;

public:
	int32_t m_noteId = -1;
	float m_gain = 0.0f;

	void setFrequencyByMIDI(uint16_t midi);
	void setSampleRate(float sample_rate);
	void setOscillatorGain(uint8_t index, float gain);
	float sample();
};
