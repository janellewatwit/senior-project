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
					// adjust master volume
					case AudioEngine::VolumeParamID:
						if (paramQueue->getPoint(numPoints - 1, sampleOffset, value) == kResultTrue)
							m_synth.setMasterVolume((float)value);
						break;
					case AudioEngine::AttackParamID:
						if (paramQueue->getPoint(numPoints - 1, sampleOffset, value) == kResultTrue)
							m_synth.setAttack(value);
						break;
					case AudioEngine::DecayParamID:
						if (paramQueue->getPoint(numPoints - 1, sampleOffset, value) == kResultTrue)
							m_synth.setDecay(value);
						break;
					case AudioEngine::SustainParamID:
						if (paramQueue->getPoint(numPoints - 1, sampleOffset, value) == kResultTrue)
							m_synth.setSustain(value);
						break;
					case AudioEngine::ReleaseParamID:
						if (paramQueue->getPoint(numPoints - 1, sampleOffset, value) == kResultTrue)
							m_synth.setRelease(value);
						break;
				}

				// adjusting gain of oscillators
				const Steinberg::Vst::ParamID paramID = paramQueue->getParameterId();
				if (paramID >= 1000 && paramID < 1064)
				{
					if (paramQueue->getPoint(numPoints - 1, sampleOffset, value) == kResultTrue)
					{
						m_synth.setOscillatorGain(paramID - 1001, value);
					}
				}
			}
		}
	}	

	//--- Here you have to implement your processing
	m_synth.processMIDIEvents(data.inputEvents);
	m_synth.generateAudio(data);

#ifdef PROFILING
	m_timer.end();
	m_logger.stream() << m_synth.countActiveVoices();
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
	m_synth.setSampleRate(newSetup.sampleRate);
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
