mkdir -p vendor/build
cd vendor/build
cmake.exe -G "Visual Studio 17 2022" -A x64 ..\vst3sdk -DSMTG_CREATE_PLUGIN_LINK=0
cmake.exe --build .
