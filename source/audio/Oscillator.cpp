#include "Oscillator.h"

Oscillator::Oscillator(float frequency)
{
	// populate sine wave table
	m_table = new float[m_table_len];
	for (uint32_t i = 0; i < m_table_len; i++)
		m_table[i] = sin(2.0f * 3.1415927f * i / m_table_len);

	// set initial oscillator frequency
	setFrequency(frequency);
}

Oscillator::~Oscillator()
{
	delete[] m_table;
}

void Oscillator::setFrequency(const float freq)
{
	// ratio of oscillator frequency over sample rate
	const float freq_ratio = freq / m_sample_rate;
	// freq_ratio * 2^32. Because (1 << 32) overflows.
	m_phase_delta = (uint32_t)(freq_ratio * 2.0f * (1U << (sizeof(uint32_t)*8U-1U)));
}

float Oscillator::sample()
{
	// increment phase by delta
	m_phase_accumulator += m_phase_delta;

	// convert phase to index in table and mask
	uint32_t index = m_phase_accumulator >> (sizeof(uint32_t) * 8U - m_table_lookup_bits);
	index &= (1U << m_table_lookup_bits) - 1U;

	// return sample from table
	return m_table[index];
}
