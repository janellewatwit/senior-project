#include "Voice.h"
#include "constants.h"

void Voice::setFrequencyByMIDI(uint16_t midi)
{
	float baseFrequency = 440.0f * (float)pow(2.0, (midi - 69.0) / 12.0);
	for (int i = 0; i < m_oscillators.size(); i++)
	{
		m_oscillators[i].setFrequency(baseFrequency * (i+1));
		m_oscillators[i].resetPhase();
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
	return sample * m_gain;
}
