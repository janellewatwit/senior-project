#pragma once
#include "SineWaveTable.h"
#include <vector>
#include "Oscillator.h"

class Voice
{
private:
	SineWaveTable& m_wavetable;
	std::vector<Oscillator> m_oscillators;

public:
	int32_t m_noteId = -1;
	float m_gain = 0.0f;
	Voice(SineWaveTable& wt, int16_t num_oscillators);

	void setFrequencyByMIDI(uint16_t midi);

	void setSampleRate(float sample_rate);

	float sample();
};
