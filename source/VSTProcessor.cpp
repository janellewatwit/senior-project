//------------------------------------------------------------------------
// Copyright(c) 2025 Sounds Magic.
//------------------------------------------------------------------------

#include "VSTProcessor.h"
#include "cids.h"

#include "base/source/fstreamer.h"
#include "pluginterfaces/vst/ivstparameterchanges.h"
#include <algorithm>
#include "audio/constants.h"
#include "public.sdk/source/vst/vsteventshelper.h"

using namespace Steinberg;

namespace SoundsMagic {
//------------------------------------------------------------------------
// VSTProcessor
//------------------------------------------------------------------------
VSTProcessor::VSTProcessor ()
{
	//--- set the wanted controller for our processor
	setControllerClass (kSoundsMagicControllerUID);
}

//------------------------------------------------------------------------
VSTProcessor::~VSTProcessor ()
{}

//------------------------------------------------------------------------
tresult PLUGIN_API VSTProcessor::initialize (FUnknown* context)
{
	// Here the Plug-in will be instantiated
	
	//---always initialize the parent-------
	tresult result = AudioEffect::initialize (context);
	// if everything Ok, continue
	if (result != kResultOk)
	{
		return result;
	}

	//--- create Audio IO ------
	addAudioInput (STR16 ("Stereo In"), Steinberg::Vst::SpeakerArr::kStereo);
	addAudioOutput (STR16 ("Stereo Out"), Steinberg::Vst::SpeakerArr::kStereo);
	addEventInput (STR16 ("Event In"), 1);

	return kResultOk;
}

//------------------------------------------------------------------------
tresult PLUGIN_API VSTProcessor::terminate ()
{
	// Here the Plug-in will be de-instantiated, last possibility to remove some memory!

	//---do not forget to call parent ------
	return AudioEffect::terminate ();
}

//------------------------------------------------------------------------
tresult PLUGIN_API VSTProcessor::setActive (TBool state)
{
	//--- called when the Plug-in is enable/disable (On/Off) -----
	return AudioEffect::setActive (state);
}

//------------------------------------------------------------------------
tresult PLUGIN_API VSTProcessor::process (Vst::ProcessData& data)
{
#ifdef PROFILING
	m_timer.start();
#endif

	//--- First : Read inputs parameter changes-----------
    if (data.inputParameterChanges)
    {
        int32 numParamsChanged = data.inputParameterChanges->getParameterCount ();
        for (int32 index = 0; index < numParamsChanged; index++)
        {
            if (auto* paramQueue = data.inputParameterChanges->getParameterData (index))
            {
                Vst::ParamValue value;
                int32 sampleOffset;
                int32 numPoints = paramQueue->getPointCount ();
                switch (paramQueue->getParameterId ())
                {
					case AudioEngine::VolumeParamID:
						if (paramQueue->getPoint(numPoints - 1, sampleOffset, value) == kResultTrue)
							m_master_volume = (float)value;
						break;
				}

				const Steinberg::Vst::ParamID paramID = paramQueue->getParameterId();
				if (paramID >= 1000 && paramID < 1064)
				{
					if (paramQueue->getPoint(numPoints - 1, sampleOffset, value) == kResultTrue)
					{
						const int idx = paramID - 1001;
						for(AudioEngine::Voice& voice : m_voices)
						{
							voice.setOscillatorGain(idx, value);
						}
					}
				}
			}
		}
	}

	if(data.inputEvents)
	{
		for(int i = 0; i < data.inputEvents->getEventCount(); i++)
		{
			Steinberg::Vst::Event e;
			data.inputEvents->getEvent(i, e);
			if(e.type == Steinberg::Vst::Event::kNoteOnEvent)
			{
				for(AudioEngine::Voice& v : m_voices)
				{
					if (v.m_noteId == -1)
					{
						v.setFrequencyByMIDI(e.noteOn.pitch);
						v.m_noteId = e.noteOn.pitch;
						v.m_gain = 1.0f;
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
						v.m_noteId = -1;
						v.m_gain = 0.0f;
					}
				}
			}
		}
	}

	//--- Here you have to implement your processing
	// iterate samples in buffer
	for (int s = 0; s < data.numSamples; s++)
	{
		// sample oscillator
		float sample = 0.0f;
		for (AudioEngine::Voice& v : m_voices)
		{
			if(v.m_noteId != -1)
				sample += v.sample();
		}
		sample *= m_master_volume;

		// write sample to each channel of each output
		for (int o = 0; o < data.numOutputs; o++)
		{
			for (int c = 0; c < data.outputs[o].numChannels; c++)
			{
				data.outputs[o].channelBuffers32[c][s] = sample;
			}
		}
	}

#ifdef PROFILING
	int active_voices = 0;
	for (AudioEngine::Voice& v : m_voices)
		if (v.m_noteId != -1) active_voices++;
	m_timer.end();
	m_logger.stream() << active_voices;
	m_logger.stream() << ":";
	m_logger.stream() << m_timer.duration_ns();
	m_logger.stream() << "\n";
#endif

	return kResultOk;
}

//------------------------------------------------------------------------
tresult PLUGIN_API VSTProcessor::setupProcessing (Vst::ProcessSetup& newSetup)
{
	//--- called before any processing ----
	for(AudioEngine::Voice& v : m_voices)
	{
		v.setSampleRate(newSetup.sampleRate);
	}
	return AudioEffect::setupProcessing (newSetup);
}

//------------------------------------------------------------------------
tresult PLUGIN_API VSTProcessor::canProcessSampleSize (int32 symbolicSampleSize)
{
	// by default kSample32 is supported
	if (symbolicSampleSize == Vst::kSample32)
		return kResultTrue;

	// disable the following comment if your processing support kSample64
	/* if (symbolicSampleSize == Vst::kSample64)
		return kResultTrue; */

	return kResultFalse;
}

//------------------------------------------------------------------------
tresult PLUGIN_API VSTProcessor::setState (IBStream* state)
{
	// called when we load a preset, the model has to be reloaded
	IBStreamer streamer (state, kLittleEndian);
	
	return kResultOk;
}

//------------------------------------------------------------------------
tresult PLUGIN_API VSTProcessor::getState (IBStream* state)
{
	// here we need to save the model
	IBStreamer streamer (state, kLittleEndian);

	return kResultOk;
}

//------------------------------------------------------------------------
} // namespace SoundsMagic
