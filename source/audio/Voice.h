#pragma once
#include "SineWaveTable.h"
#include <vector>
#include "Oscillator.h"

class Voice
{
private:
	SineWaveTable m_wavetable = SineWaveTable(16U);

public:
	std::vector<Oscillator> m_oscillators;
	Voice(int16_t num_oscillators);

	void setFrequencyByMIDI(uint16_t midi);

	void setSampleRate(float sample_rate);

	float sample();
};
