#pragma once

namespace SoundsMagic::AudioEngine
{
enum ADSRState
{
	OFF,
	ATTACK,
	DECAY,
	SUSTAIN,
	RELEASE
};

class ADSR
{
private:
	float m_level = 0.0f;
	ADSRState m_state = OFF;
	float m_sampleRate = 1.0f;

	float m_attackTime = 1.0f;
	float m_decayTime = 1.0f;
	float m_sustainLevel = 0.8f;
	float m_releaseTime = 1.0f;

public:
	// state machine
	ADSRState getState();
	void noteOn();
	void noteOff();

	// parameters
	void setSampleRate(float sampleRate);
	void setAttackTime(float attackPercent);
	void setDecayTime(float decayPercent);
	void setSustainLevel(float sustainLevel);
	void setReleaseTime(float releasePercent);

	// process level
	float sample();
};
}
