cmake_minimum_required(VERSION 3.14.0)
set(CMAKE_OSX_DEPLOYMENT_TARGET 10.13 CACHE STRING "")

set(vst3sdk_SOURCE_DIR "./vendor/vst3sdk")
if(NOT vst3sdk_SOURCE_DIR)
    message(FATAL_ERROR "Path to VST3 SDK is empty!")
endif()

project(SeniorProject
    # This is your plug-in version number. Change it here only.
    # Version number symbols usable in C++ can be found in
    # source/version.h and ${PROJECT_BINARY_DIR}/projectversion.h.
    VERSION 1.0.0.0 
    DESCRIPTION "SeniorProject VST 3 Plug-in"
)

set(SMTG_VSTGUI_ROOT "${vst3sdk_SOURCE_DIR}")

add_subdirectory(${vst3sdk_SOURCE_DIR} ${PROJECT_BINARY_DIR}/vst3sdk)
smtg_enable_vst3_sdk()

smtg_add_vst3plugin(SeniorProject
    source/version.h
    source/myplugincids.h
    source/mypluginprocessor.h
    source/mypluginprocessor.cpp
    source/myplugincontroller.h
    source/myplugincontroller.cpp
    source/mypluginentry.cpp
)

#- VSTGUI Wanted ----
if(SMTG_ENABLE_VSTGUI_SUPPORT)
    target_sources(SeniorProject
        PRIVATE
            resource/myplugineditor.uidesc
    )
    target_link_libraries(SeniorProject
        PRIVATE
            vstgui_support
    )
    smtg_target_add_plugin_resources(SeniorProject
        RESOURCES
            "resource/myplugineditor.uidesc"
    )
endif(SMTG_ENABLE_VSTGUI_SUPPORT)
# -------------------

smtg_target_add_plugin_snapshots (SeniorProject
    RESOURCES
        resource/AFB246EE41A5566BB4E3934B44D20DF7_snapshot.png
        resource/AFB246EE41A5566BB4E3934B44D20DF7_snapshot_2.0x.png
)

target_link_libraries(SeniorProject
    PRIVATE
        sdk
)

smtg_target_configure_version_file(SeniorProject)

if(SMTG_MAC)
    smtg_target_set_bundle(SeniorProject
        BUNDLE_IDENTIFIER com.janellew.seniorproject
        COMPANY_NAME "Senior Project"
    )
    smtg_target_set_debug_executable(SeniorProject
        "/Applications/VST3PluginTestHost.app"
        "--pluginfolder;$(BUILT_PRODUCTS_DIR)"
    )
elseif(SMTG_WIN)
    target_sources(SeniorProject PRIVATE 
        resource/win32resource.rc
    )
    if(MSVC)
        set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT SeniorProject)

        smtg_target_set_debug_executable(SeniorProject
            "$(ProgramW6432)/Steinberg/VST3PluginTestHost/VST3PluginTestHost.exe"
            "--pluginfolder \"$(OutDir)/\""
        )
    endif()
endif(SMTG_MAC)
