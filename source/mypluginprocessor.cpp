//------------------------------------------------------------------------
// Copyright(c) 2025 Senior Project.
//------------------------------------------------------------------------

#include "mypluginprocessor.h"
#include "myplugincids.h"

#include "base/source/fstreamer.h"
#include "pluginterfaces/vst/ivstparameterchanges.h"
#include <algorithm>
#include "audio/constants.h"

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

	float base_freq = 300.0f;
	for (int i = 0; i < NUM_OSCILLATORS; i++)
	{
		m_oscillators.emplace_back(m_wavetable);
		m_oscillators[i].setFrequency(base_freq * (i+1));
		m_oscillators[i].setGain(1.0);
	}

	return kResultOk;
}

//------------------------------------------------------------------------
tresult PLUGIN_API SeniorProjectProcessor::terminate ()
{
	// Here the Plug-in will be de-instantiated, last possibility to remove some memory!
	
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

				const Steinberg::Vst::ParamID paramID = paramQueue->getParameterId();
				if (paramID >= 1000 && paramID < 1064)
				{
					const int idx = paramID - 1000;
					if (paramQueue->getPoint(numPoints - 1, sampleOffset, value) == kResultTrue)
						m_oscillators[idx].setGain(value);
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
		for (Oscillator& osc : m_oscillators)
		{
			sample += osc.sample() / m_oscillators.size();
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

	return kResultOk;
}

//------------------------------------------------------------------------
tresult PLUGIN_API SeniorProjectProcessor::setupProcessing (Vst::ProcessSetup& newSetup)
{
	//--- called before any processing ----
	for (Oscillator& osc : m_oscillators)
		osc.setSampleRate(newSetup.sampleRate);
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
