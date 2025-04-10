cmake_minimum_required(VERSION 3.14.0)
set(CMAKE_OSX_DEPLOYMENT_TARGET 10.13 CACHE STRING "")

set(vst3sdk_SOURCE_DIR "./vendor/vst3sdk")
if(NOT vst3sdk_SOURCE_DIR)
    message(FATAL_ERROR "Path to VST3 SDK is empty!")
endif()

project(SoundsMagic
    # This is your plug-in version number. Change it here only.
    # Version number symbols usable in C++ can be found in
    # source/version.h and ${PROJECT_BINARY_DIR}/projectversion.h.
    VERSION 1.0.0.0 
    DESCRIPTION "SoundsMagic VST 3 Plug-in"
)

set(SMTG_VSTGUI_ROOT "${vst3sdk_SOURCE_DIR}")

add_subdirectory(${vst3sdk_SOURCE_DIR} ${PROJECT_BINARY_DIR}/vst3sdk)
smtg_enable_vst3_sdk()

smtg_add_vst3plugin(SoundsMagic
	source/main.cpp
    source/version.h
    source/cids.h
    source/VSTProcessor.h
    source/VSTProcessor.cpp
    source/VSTController.h
    source/VSTController.cpp
	source/audio/constants.h
	source/audio/ADSR.h
	source/audio/ADSR.cpp
	source/audio/Oscillator.h
	source/audio/Oscillator.cpp
	source/audio/SineWaveTable.h
	source/audio/SineWaveTable.cpp
	source/audio/Synthesizer.h
	source/audio/Synthesizer.cpp
	source/audio/Voice.h
	source/audio/Voice.cpp
	source/utils/Logger.h
	source/utils/Logger.cpp
	source/utils/Timer.h
	source/utils/Timer.cpp

    source/SharedMemoryWriter.h
    source/SharedMemoryWriter.cpp
    source/SharedMemoryData.h
)

#- VSTGUI Wanted ----
if(SMTG_ENABLE_VSTGUI_SUPPORT)
    target_sources(SoundsMagic
        PRIVATE
            resource/myplugineditor.uidesc
    )
    target_link_libraries(SoundsMagic
        PRIVATE
            vstgui_support
    )
    smtg_target_add_plugin_resources(SoundsMagic
        RESOURCES
            "resource/myplugineditor.uidesc"
    )
endif(SMTG_ENABLE_VSTGUI_SUPPORT)
# -------------------

smtg_target_add_plugin_snapshots (SoundsMagic
    RESOURCES
        resource/AFB246EE41A5566BB4E3934B44D20DF7_snapshot.png
        resource/AFB246EE41A5566BB4E3934B44D20DF7_snapshot_2.0x.png
)

target_link_libraries(SoundsMagic
    PRIVATE
        sdk
)

smtg_target_configure_version_file(SoundsMagic)

if(SMTG_MAC)
    smtg_target_set_bundle(SoundsMagic
        BUNDLE_IDENTIFIER com.janellew.SoundsMagic
        COMPANY_NAME "SoundsMagic"
    )
    smtg_target_set_debug_executable(SoundsMagic
        "/Applications/VST3PluginTestHost.app"
        "--pluginfolder;$(BUILT_PRODUCTS_DIR)"
    )
elseif(SMTG_WIN)
    target_sources(SoundsMagic PRIVATE 
        resource/win32resource.rc
    )
    if(MSVC)
        set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT SoundsMagic)

        smtg_target_set_debug_executable(SoundsMagic
            "$(ProgramW6432)/Steinberg/VST3PluginTestHost/VST3PluginTestHost.exe"
            "--pluginfolder \"$(OutDir)/\""
        )
    endif()
endif(SMTG_MAC)
