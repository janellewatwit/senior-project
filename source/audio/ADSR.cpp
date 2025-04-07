#include "ADSR.h"

namespace SoundsMagic::AudioEngine
{
ADSRState ADSR::getState()
{
	return m_state;
}

void ADSR::noteOn()
{
	m_level = 0.0f;
	m_state = ATTACK;
}

void ADSR::noteOff()
{
	m_state = RELEASE;
}

void ADSR::setSampleRate(float sampleRate)
{
	m_sampleRate = sampleRate;
}

void ADSR::setAttackTime(float attackPercent)
{
	m_attackTime = attackPercent * m_sampleRate;
	if (m_attackTime == 0.0f) m_attackTime = 1.0f; // min time is 1 sample
}

void ADSR::setDecayTime(float decayPercent)
{
	m_decayTime = decayPercent * m_sampleRate;
	if (m_decayTime == 0.0f) m_decayTime = 1.0f; // min time is 1 sample
}

void ADSR::setSustainLevel(float sustainLevel)
{
	m_sustainLevel = sustainLevel;
}

void ADSR::setReleaseTime(float releasePercent)
{
	m_releaseTime = releasePercent * m_sampleRate;
	if (m_releaseTime == 0.0f) m_releaseTime = 1.0f; // min time is 1 sample
}

float ADSR::sample()
{
	switch(m_state)
	{
		case OFF:
			m_level = 0.0f;
			break;
		
		case ATTACK:
			m_level += 1.0f/m_attackTime;
			if (m_level >= 1.0f)
				m_state = DECAY;
			break;
		
		case DECAY:
			m_level -= (1.0f - m_sustainLevel)/m_decayTime;
			if (m_level <= m_sustainLevel)
				m_state = SUSTAIN;
			break;

		case SUSTAIN:
			m_level = m_sustainLevel;
			break;

		case RELEASE:
			m_level -= m_sustainLevel/m_releaseTime;
			if (m_level <= 0.0f)
				m_state = OFF;
			break;
	}

	return m_level;
}	
}
