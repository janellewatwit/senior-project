#include "Synthesizer.h"

namespace SoundsMagic::AudioEngine
{
	void Synthesizer::setMasterVolume(float volume)
	{
		m_master_volume = volume;
	}

	void Synthesizer::setSampleRate(float sampleRate)
	{
		for(Voice& v : m_voices)
		{
			v.setSampleRate(sampleRate);
		}
	}

	void Synthesizer::setOscillatorGain(uint8_t index, float gain)
	{
		for(AudioEngine::Voice& voice : m_voices)
		{
			voice.setOscillatorGain(index, gain);
		}
	}

	void Synthesizer::processMIDIEvents(Steinberg::Vst::IEventList* inputEvents)
	{
		if(inputEvents)
		{
			for(int i = 0; i < inputEvents->getEventCount(); i++)
			{
				Steinberg::Vst::Event e;
				inputEvents->getEvent(i, e);
				if(e.type == Steinberg::Vst::Event::kNoteOnEvent)
				{
					for(AudioEngine::Voice& v : m_voices)
					{
						if (v.m_noteId == -1)
						{
							v.playPitch(e.noteOn.pitch);
							break;
						}
					}
				}
				if(e.type == Steinberg::Vst::Event::kNoteOffEvent)
				{
					for(AudioEngine::Voice& v : m_voices)
					{
						if (v.m_noteId == e.noteOff.pitch)
						{
							v.releaseNote();
						}
					}
				}
			}
		}
	}

	void Synthesizer::generateAudio(Steinberg::Vst::ProcessData& data)
	{
		// iterate samples in buffer
		for (int s = 0; s < data.numSamples; s++)
		{
			// sample oscillator
			float sample = 0.0f;
			for (AudioEngine::Voice& v : m_voices)
			{
				if(v.envelope.getState() != OFF)
					sample += v.sample();
			}
			sample *= m_master_volume;

			// write sample to each channel of each output
			for (int o = 0; o < data.numOutputs; o++)
			{
				for (int c = 0; c < data.outputs[o].numChannels; c++)
				{
					data.outputs[o].channelBuffers32[c][s] = sample;

					//I had a different approach, but this is more efficient
					if(memBuff.size() < 1024) memBuff.push_back(sample);
				}
			}
		}
	}

	uint16_t Synthesizer::countActiveVoices()
	{
		uint16_t count = 0U;
		for (Voice& v : m_voices)
			if (v.envelope.getState() != OFF)
				count++;
		return count;
	}

	void Synthesizer::setAttack(float attack)
	{
		for (Voice& v : m_voices)
			v.envelope.setAttackTime(attack);
	}
	
	void Synthesizer::setDecay(float decay)
	{
		for (Voice& v : m_voices)
			v.envelope.setDecayTime(decay);
	}
	
	void Synthesizer::setSustain(float sustain)
	{
		for (Voice& v : m_voices)
			v.envelope.setSustainLevel(sustain);
	}
	
	void Synthesizer::setRelease(float release)
	{
		for (Voice& v : m_voices)
			v.envelope.setReleaseTime(release);
	}
}
