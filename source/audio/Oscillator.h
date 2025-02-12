#pragma once
#include <cstdint>
#include <cmath>

class Oscillator
{
private:
	// sample rate of host
	double m_sample_rate = 44100.0;
	// table containing sampled sine wave
	float* m_table = nullptr;
	// bits used to index `m_table`
	const uint32_t m_table_lookup_bits = 16U;
	// length of `m_table`
	uint32_t m_table_len = (1U << m_table_lookup_bits);
	// current phase of oscillator: 0 = 0%, 2^32 = 100%
	uint32_t m_phase_accumulator = 0U;
	// change in phase per sample
	uint32_t m_phase_delta = 0U;

public:
	Oscillator(float frequency);
	~Oscillator();
	virtual void setFrequency(const float freq);
	virtual float sample();
};
