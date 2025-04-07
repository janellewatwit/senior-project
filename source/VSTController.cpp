//------------------------------------------------------------------------
// Copyright(c) 2025 Sounds Magic.
//------------------------------------------------------------------------

#include "VSTController.h"
#include "cids.h"
#include "vstgui/plugin-bindings/vst3editor.h"
#include "audio/constants.h"

using namespace Steinberg;

namespace SoundsMagic {

//------------------------------------------------------------------------
// VSTController Implementation
//------------------------------------------------------------------------
tresult PLUGIN_API VSTController::initialize (FUnknown* context)
{
	// Here the Plug-in will be instantiated

	//---do not forget to call parent ------
	tresult result = EditControllerEx1::initialize (context);
	if (result != kResultOk)
	{
		return result;
	}

	// Here you could register some parameters
	parameters.addParameter(STR16("Volume"), STR16("%"), 0, 1, Steinberg::Vst::ParameterInfo::kCanAutomate, AudioEngine::VolumeParamID);
	parameters.addParameter(STR16("Attack"), STR16("ms"), 0, 0, Steinberg::Vst::ParameterInfo::kCanAutomate, AudioEngine::AttackParamID);
	parameters.addParameter(STR16("Decay"), STR16("ms"), 0, 0, Steinberg::Vst::ParameterInfo::kCanAutomate, AudioEngine::DecayParamID);
	parameters.addParameter(STR16("Sustain"), STR16("ms"), 0, 1, Steinberg::Vst::ParameterInfo::kCanAutomate, AudioEngine::SustainParamID);
	parameters.addParameter(STR16("Release"), STR16("ms"), 0, 0, Steinberg::Vst::ParameterInfo::kCanAutomate, AudioEngine::ReleaseParamID);
	parameters.addParameter(STR16("Osc_01 gain"), STR16("%"), 0, 1, Steinberg::Vst::ParameterInfo::kCanAutomate, 1001U);
	parameters.addParameter(STR16("Osc_02 gain"), STR16("%"), 0, 1, Steinberg::Vst::ParameterInfo::kCanAutomate, 1002U);
	parameters.addParameter(STR16("Osc_03 gain"), STR16("%"), 0, 1, Steinberg::Vst::ParameterInfo::kCanAutomate, 1003U);
	parameters.addParameter(STR16("Osc_04 gain"), STR16("%"), 0, 1, Steinberg::Vst::ParameterInfo::kCanAutomate, 1004U);
	parameters.addParameter(STR16("Osc_05 gain"), STR16("%"), 0, 1, Steinberg::Vst::ParameterInfo::kCanAutomate, 1005U);
	parameters.addParameter(STR16("Osc_06 gain"), STR16("%"), 0, 1, Steinberg::Vst::ParameterInfo::kCanAutomate, 1006U);
	parameters.addParameter(STR16("Osc_07 gain"), STR16("%"), 0, 1, Steinberg::Vst::ParameterInfo::kCanAutomate, 1007U);
	parameters.addParameter(STR16("Osc_08 gain"), STR16("%"), 0, 1, Steinberg::Vst::ParameterInfo::kCanAutomate, 1008U);
	parameters.addParameter(STR16("Osc_09 gain"), STR16("%"), 0, 1, Steinberg::Vst::ParameterInfo::kCanAutomate, 1009U);
	parameters.addParameter(STR16("Osc_10 gain"), STR16("%"), 0, 1, Steinberg::Vst::ParameterInfo::kCanAutomate, 1010U);
	parameters.addParameter(STR16("Osc_11 gain"), STR16("%"), 0, 1, Steinberg::Vst::ParameterInfo::kCanAutomate, 1011U);
	parameters.addParameter(STR16("Osc_12 gain"), STR16("%"), 0, 1, Steinberg::Vst::ParameterInfo::kCanAutomate, 1012U);
	parameters.addParameter(STR16("Osc_13 gain"), STR16("%"), 0, 1, Steinberg::Vst::ParameterInfo::kCanAutomate, 1013U);
	parameters.addParameter(STR16("Osc_14 gain"), STR16("%"), 0, 1, Steinberg::Vst::ParameterInfo::kCanAutomate, 1014U);
	parameters.addParameter(STR16("Osc_15 gain"), STR16("%"), 0, 1, Steinberg::Vst::ParameterInfo::kCanAutomate, 1015U);
	parameters.addParameter(STR16("Osc_16 gain"), STR16("%"), 0, 1, Steinberg::Vst::ParameterInfo::kCanAutomate, 1016U);
	parameters.addParameter(STR16("Osc_17 gain"), STR16("%"), 0, 1, Steinberg::Vst::ParameterInfo::kCanAutomate, 1017U);
	parameters.addParameter(STR16("Osc_18 gain"), STR16("%"), 0, 1, Steinberg::Vst::ParameterInfo::kCanAutomate, 1018U);
	parameters.addParameter(STR16("Osc_19 gain"), STR16("%"), 0, 1, Steinberg::Vst::ParameterInfo::kCanAutomate, 1019U);
	parameters.addParameter(STR16("Osc_20 gain"), STR16("%"), 0, 1, Steinberg::Vst::ParameterInfo::kCanAutomate, 1020U);
	parameters.addParameter(STR16("Osc_21 gain"), STR16("%"), 0, 1, Steinberg::Vst::ParameterInfo::kCanAutomate, 1021U);
	parameters.addParameter(STR16("Osc_22 gain"), STR16("%"), 0, 1, Steinberg::Vst::ParameterInfo::kCanAutomate, 1022U);
	parameters.addParameter(STR16("Osc_23 gain"), STR16("%"), 0, 1, Steinberg::Vst::ParameterInfo::kCanAutomate, 1023U);
	parameters.addParameter(STR16("Osc_24 gain"), STR16("%"), 0, 1, Steinberg::Vst::ParameterInfo::kCanAutomate, 1024U);
	parameters.addParameter(STR16("Osc_25 gain"), STR16("%"), 0, 1, Steinberg::Vst::ParameterInfo::kCanAutomate, 1025U);
	parameters.addParameter(STR16("Osc_26 gain"), STR16("%"), 0, 1, Steinberg::Vst::ParameterInfo::kCanAutomate, 1026U);
	parameters.addParameter(STR16("Osc_27 gain"), STR16("%"), 0, 1, Steinberg::Vst::ParameterInfo::kCanAutomate, 1027U);
	parameters.addParameter(STR16("Osc_28 gain"), STR16("%"), 0, 1, Steinberg::Vst::ParameterInfo::kCanAutomate, 1028U);
	parameters.addParameter(STR16("Osc_29 gain"), STR16("%"), 0, 1, Steinberg::Vst::ParameterInfo::kCanAutomate, 1029U);
	parameters.addParameter(STR16("Osc_30 gain"), STR16("%"), 0, 1, Steinberg::Vst::ParameterInfo::kCanAutomate, 1030U);
	parameters.addParameter(STR16("Osc_31 gain"), STR16("%"), 0, 1, Steinberg::Vst::ParameterInfo::kCanAutomate, 1031U);
	parameters.addParameter(STR16("Osc_32 gain"), STR16("%"), 0, 1, Steinberg::Vst::ParameterInfo::kCanAutomate, 1032U);
	parameters.addParameter(STR16("Osc_33 gain"), STR16("%"), 0, 1, Steinberg::Vst::ParameterInfo::kCanAutomate, 1033U);
	parameters.addParameter(STR16("Osc_34 gain"), STR16("%"), 0, 1, Steinberg::Vst::ParameterInfo::kCanAutomate, 1034U);
	parameters.addParameter(STR16("Osc_35 gain"), STR16("%"), 0, 1, Steinberg::Vst::ParameterInfo::kCanAutomate, 1035U);
	parameters.addParameter(STR16("Osc_36 gain"), STR16("%"), 0, 1, Steinberg::Vst::ParameterInfo::kCanAutomate, 1036U);
	parameters.addParameter(STR16("Osc_37 gain"), STR16("%"), 0, 1, Steinberg::Vst::ParameterInfo::kCanAutomate, 1037U);
	parameters.addParameter(STR16("Osc_38 gain"), STR16("%"), 0, 1, Steinberg::Vst::ParameterInfo::kCanAutomate, 1038U);
	parameters.addParameter(STR16("Osc_39 gain"), STR16("%"), 0, 1, Steinberg::Vst::ParameterInfo::kCanAutomate, 1039U);
	parameters.addParameter(STR16("Osc_40 gain"), STR16("%"), 0, 1, Steinberg::Vst::ParameterInfo::kCanAutomate, 1040U);
	parameters.addParameter(STR16("Osc_41 gain"), STR16("%"), 0, 1, Steinberg::Vst::ParameterInfo::kCanAutomate, 1041U);
	parameters.addParameter(STR16("Osc_42 gain"), STR16("%"), 0, 1, Steinberg::Vst::ParameterInfo::kCanAutomate, 1042U);
	parameters.addParameter(STR16("Osc_43 gain"), STR16("%"), 0, 1, Steinberg::Vst::ParameterInfo::kCanAutomate, 1043U);
	parameters.addParameter(STR16("Osc_44 gain"), STR16("%"), 0, 1, Steinberg::Vst::ParameterInfo::kCanAutomate, 1044U);
	parameters.addParameter(STR16("Osc_45 gain"), STR16("%"), 0, 1, Steinberg::Vst::ParameterInfo::kCanAutomate, 1045U);
	parameters.addParameter(STR16("Osc_46 gain"), STR16("%"), 0, 1, Steinberg::Vst::ParameterInfo::kCanAutomate, 1046U);
	parameters.addParameter(STR16("Osc_47 gain"), STR16("%"), 0, 1, Steinberg::Vst::ParameterInfo::kCanAutomate, 1047U);
	parameters.addParameter(STR16("Osc_48 gain"), STR16("%"), 0, 1, Steinberg::Vst::ParameterInfo::kCanAutomate, 1048U);
	parameters.addParameter(STR16("Osc_49 gain"), STR16("%"), 0, 1, Steinberg::Vst::ParameterInfo::kCanAutomate, 1049U);
	parameters.addParameter(STR16("Osc_50 gain"), STR16("%"), 0, 1, Steinberg::Vst::ParameterInfo::kCanAutomate, 1050U);
	parameters.addParameter(STR16("Osc_51 gain"), STR16("%"), 0, 1, Steinberg::Vst::ParameterInfo::kCanAutomate, 1051U);
	parameters.addParameter(STR16("Osc_52 gain"), STR16("%"), 0, 1, Steinberg::Vst::ParameterInfo::kCanAutomate, 1052U);
	parameters.addParameter(STR16("Osc_53 gain"), STR16("%"), 0, 1, Steinberg::Vst::ParameterInfo::kCanAutomate, 1053U);
	parameters.addParameter(STR16("Osc_54 gain"), STR16("%"), 0, 1, Steinberg::Vst::ParameterInfo::kCanAutomate, 1054U);
	parameters.addParameter(STR16("Osc_55 gain"), STR16("%"), 0, 1, Steinberg::Vst::ParameterInfo::kCanAutomate, 1055U);
	parameters.addParameter(STR16("Osc_56 gain"), STR16("%"), 0, 1, Steinberg::Vst::ParameterInfo::kCanAutomate, 1056U);
	parameters.addParameter(STR16("Osc_57 gain"), STR16("%"), 0, 1, Steinberg::Vst::ParameterInfo::kCanAutomate, 1057U);
	parameters.addParameter(STR16("Osc_58 gain"), STR16("%"), 0, 1, Steinberg::Vst::ParameterInfo::kCanAutomate, 1058U);
	parameters.addParameter(STR16("Osc_59 gain"), STR16("%"), 0, 1, Steinberg::Vst::ParameterInfo::kCanAutomate, 1059U);
	parameters.addParameter(STR16("Osc_60 gain"), STR16("%"), 0, 1, Steinberg::Vst::ParameterInfo::kCanAutomate, 1060U);
	parameters.addParameter(STR16("Osc_61 gain"), STR16("%"), 0, 1, Steinberg::Vst::ParameterInfo::kCanAutomate, 1061U);
	parameters.addParameter(STR16("Osc_62 gain"), STR16("%"), 0, 1, Steinberg::Vst::ParameterInfo::kCanAutomate, 1062U);
	parameters.addParameter(STR16("Osc_63 gain"), STR16("%"), 0, 1, Steinberg::Vst::ParameterInfo::kCanAutomate, 1063U);
	parameters.addParameter(STR16("Osc_64 gain"), STR16("%"), 0, 1, Steinberg::Vst::ParameterInfo::kCanAutomate, 1064U);

	return result;
}

//------------------------------------------------------------------------
tresult PLUGIN_API VSTController::terminate ()
{
	// Here the Plug-in will be de-instantiated, last possibility to remove some memory!

	//---do not forget to call parent ------
	return EditControllerEx1::terminate ();
}

//------------------------------------------------------------------------
tresult PLUGIN_API VSTController::setComponentState (IBStream* state)
{
	// Here you get the state of the component (Processor part)
	if (!state)
		return kResultFalse;

	return kResultOk;
}

//------------------------------------------------------------------------
tresult PLUGIN_API VSTController::setState (IBStream* state)
{
	// Here you get the state of the controller

	return kResultTrue;
}

//------------------------------------------------------------------------
tresult PLUGIN_API VSTController::getState (IBStream* state)
{
	// Here you are asked to deliver the state of the controller (if needed)
	// Note: the real state of your plug-in is saved in the processor

	return kResultTrue;
}

//------------------------------------------------------------------------
IPlugView* PLUGIN_API VSTController::createView (FIDString name)
{
	// Here the Host wants to open your editor (if you have one)
	if (FIDStringsEqual (name, Vst::ViewType::kEditor))
	{
		// create your editor here and return a IPlugView ptr of it
		auto* view = new VSTGUI::VST3Editor (this, "main", "myplugineditor.uidesc");
		return view;
	}
	return nullptr;
}

//------------------------------------------------------------------------
tresult PLUGIN_API VSTController::setParamNormalized (Vst::ParamID tag, Vst::ParamValue value)
{
	// called by host to update your parameters
	tresult result = EditControllerEx1::setParamNormalized (tag, value);
	return result;
}

//------------------------------------------------------------------------
tresult PLUGIN_API VSTController::getParamStringByValue (Vst::ParamID tag, Vst::ParamValue valueNormalized, Vst::String128 string)
{
	// called by host to get a string for given normalized value of a specific parameter
	// (without having to set the value!)
	return EditControllerEx1::getParamStringByValue (tag, valueNormalized, string);
}

//------------------------------------------------------------------------
tresult PLUGIN_API VSTController::getParamValueByString (Vst::ParamID tag, Vst::TChar* string, Vst::ParamValue& valueNormalized)
{
	// called by host to get a normalized value from a string representation of a specific parameter
	// (without having to set the value!)
	return EditControllerEx1::getParamValueByString (tag, string, valueNormalized);
}

//------------------------------------------------------------------------
} // namespace SoundsMagic
