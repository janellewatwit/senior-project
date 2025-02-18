#include "Oscillator.h"

Oscillator::Oscillator(SineWaveTable& const wavetable) : m_wavetable(wavetable)
{
	updatePhaseDelta();
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
	return m_wavetable.sample(m_phase_accumulator);
}
