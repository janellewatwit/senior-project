#pragma once
#include <cstdint>
#include <cmath>
#include "SineWaveTable.h"

class Oscillator
{
private:
	// oscillator frequency
	float m_frequency = 0.0f;
	// host sample rate
	float m_sample_rate = 1.0f;
	// wavetable to sample
	SineWaveTable& m_wavetable;
	// change in phase per sample
	uint32_t m_phase_delta = 0U;
	// current phase of oscillator: 0 = 0%, 2^32 = 100%
	uint32_t m_phase_accumulator = 0U;
	
	// parameters
	double m_gain = 1.0;

	// update m_phase_delta after changing frequency or sample rate
	void updatePhaseDelta();

public:
	Oscillator(SineWaveTable& wavetable);
	void setFrequency(const float freq);
	void setSampleRate(const float sample_rate);
	void setGain(const double gain);
	float sample();
};
