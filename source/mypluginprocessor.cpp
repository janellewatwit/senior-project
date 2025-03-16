//------------------------------------------------------------------------
// Copyright(c) 2025 Senior Project.
//------------------------------------------------------------------------

#include "mypluginprocessor.h"
#include "myplugincids.h"

#include "base/source/fstreamer.h"
#include "pluginterfaces/vst/ivstparameterchanges.h"
#include <algorithm>
#include "audio/constants.h"
#include "public.sdk/source/vst/vsteventshelper.h"

using namespace Steinberg;

namespace SeniorProject {
//------------------------------------------------------------------------
// SeniorProjectProcessor
//------------------------------------------------------------------------
SeniorProjectProcessor::SeniorProjectProcessor ()
{
	//--- set the wanted controller for our processor
	setControllerClass (kSeniorProjectControllerUID);
}

//------------------------------------------------------------------------
SeniorProjectProcessor::~SeniorProjectProcessor ()
{}

//------------------------------------------------------------------------
tresult PLUGIN_API SeniorProjectProcessor::initialize (FUnknown* context)
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

	/* If you don't need an event bus, you can remove the next line */
	addEventInput (STR16 ("Event In"), 1);

	m_voices.reserve(NUM_VOICES);
	for (int i = 0; i < NUM_VOICES; i++)
	{
		m_voices.emplace_back(m_wavetable, 64U);
	}

	m_logfile.open(LOGFILE);

	return kResultOk;
}

//------------------------------------------------------------------------
tresult PLUGIN_API SeniorProjectProcessor::terminate ()
{
	// Here the Plug-in will be de-instantiated, last possibility to remove some memory!
	m_logfile.close();

	//---do not forget to call parent ------
	return AudioEffect::terminate ();
}

//------------------------------------------------------------------------
tresult PLUGIN_API SeniorProjectProcessor::setActive (TBool state)
{
	//--- called when the Plug-in is enable/disable (On/Off) -----
	return AudioEffect::setActive (state);
}

//------------------------------------------------------------------------
tresult PLUGIN_API SeniorProjectProcessor::process (Vst::ProcessData& data)
{
	auto tStart = m_timer.now();
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
					case VolumeParamID:
						if (paramQueue->getPoint(numPoints - 1, sampleOffset, value) == kResultTrue)
							m_master_volume = (float)value;
						break;
				}

				// const Steinberg::Vst::ParamID paramID = paramQueue->getParameterId();
				// if (paramID >= 1000 && paramID < 1064)
				// {
				// 	const int idx = paramID - 1000;
				// 	if (paramQueue->getPoint(numPoints - 1, sampleOffset, value) == kResultTrue)
				// 		m_voice.m_oscillators[idx].setGain(value);
				// }
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
				for(Voice& v : m_voices)
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
				for(Voice& v : m_voices)
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
	int active_voices = 0;
	for (Voice& v : m_voices)
		if (v.m_noteId != -1) active_voices++;
	for (int s = 0; s < data.numSamples; s++)
	{
		// sample oscillator
		float sample = 0.0f;
		for (Voice& v : m_voices)
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
	auto tEnd = m_timer.now();
	auto duration = tEnd - tStart;
	m_logfile << active_voices;
	m_logfile << ":";
	m_logfile << duration.count();
	m_logfile << "\n";

	return kResultOk;
}

//------------------------------------------------------------------------
tresult PLUGIN_API SeniorProjectProcessor::setupProcessing (Vst::ProcessSetup& newSetup)
{
	//--- called before any processing ----
	for(Voice& v : m_voices)
	{
		v.setSampleRate(newSetup.sampleRate);
	}
	return AudioEffect::setupProcessing (newSetup);
}

//------------------------------------------------------------------------
tresult PLUGIN_API SeniorProjectProcessor::canProcessSampleSize (int32 symbolicSampleSize)
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
tresult PLUGIN_API SeniorProjectProcessor::setState (IBStream* state)
{
	// called when we load a preset, the model has to be reloaded
	IBStreamer streamer (state, kLittleEndian);
	
	return kResultOk;
}

//------------------------------------------------------------------------
tresult PLUGIN_API SeniorProjectProcessor::getState (IBStream* state)
{
	// here we need to save the model
	IBStreamer streamer (state, kLittleEndian);

	return kResultOk;
}

//------------------------------------------------------------------------
} // namespace SeniorProject
