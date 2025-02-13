#include "Oscillator.h"

Oscillator::Oscillator()
{
	// populate sine wave table
	m_table = new float[m_table_len];
	for (uint32_t i = 0; i < m_table_len; i++)
		m_table[i] = sin(2.0f * 3.1415927f * i / m_table_len);

	// set initial oscillator frequency
	updatePhaseDelta();
}

Oscillator::~Oscillator()
{
	// if (m_table != nullptr)
	// 	delete[] m_table;
}

void Oscillator::updatePhaseDelta()
{
	// ratio of oscillator frequency over sample rate
	const float freq_ratio = m_frequency / m_sample_rate;
	// Moves fractional portion to integer, truncating whole number portion
	m_phase_delta = (uint32_t)(freq_ratio * ((uint64_t)1 << (sizeof(uint32_t)*8U)));
}

void Oscillator::setFrequency(const float frequency)
{
	m_frequency = frequency;
	updatePhaseDelta();
}

void Oscillator::setSampleRate(const float sampleRate)
{
	m_sample_rate = sampleRate;
	updatePhaseDelta();
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
