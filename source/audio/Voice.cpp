#include "Voice.h"

Voice::Voice(int16_t num_oscillators)
{
	for (int i = 0; i < num_oscillators; i++)
	{
		m_oscillators.emplace_back(m_wavetable);
		m_oscillators[i].setGain(1.0);
	}
}

void Voice::setFrequencyByMIDI(uint16_t midi)
{
	float baseFrequency = 440.0f * pow(2.0, (midi - 69.0) / 12.0);
	for (int i = 0; i < m_oscillators.size(); i++)
	{
		m_oscillators[i].setFrequency(baseFrequency * (i+1));
	}
}

void Voice::setSampleRate(float sample_rate)
{
	for (Oscillator& osc : m_oscillators)
		osc.setSampleRate(sample_rate);
}

float Voice::sample()
{
	float sample = 0.0f;
	for (Oscillator& osc : m_oscillators)
	{
		sample += osc.sample() / m_oscillators.size();
	}
	return sample;
}