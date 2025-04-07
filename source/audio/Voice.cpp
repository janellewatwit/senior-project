#include "Voice.h"
#include "constants.h"

namespace SoundsMagic::AudioEngine
{
void Voice::playPitch(float pitch)
{
	float baseFrequency = 440.0f * (float)pow(2.0, (pitch - 69.0) / 12.0);
	for (int i = 0; i < m_oscillators.size(); i++)
	{
		m_oscillators[i].setFrequency(baseFrequency * (i+1));
		m_oscillators[i].resetPhase();
	}
	m_noteId = pitch;
	envelope.noteOn();
}

void Voice::releaseNote()
{
	m_noteId = -1;
	envelope.noteOff();
}

void Voice::setSampleRate(float sample_rate)
{
	envelope.setSampleRate(sample_rate);
	for (Oscillator& osc : m_oscillators)
		osc.setSampleRate(sample_rate);
}

void Voice::setOscillatorGain(uint8_t index, float gain)
{
	m_oscillators.at(index).setGain(gain);
}

float Voice::sample()
{
	float sample = 0.0f;
	for (Oscillator& osc : m_oscillators)
	{
		sample += osc.sample() / m_oscillators.size();
	}
	return sample * envelope.sample();
}
}
