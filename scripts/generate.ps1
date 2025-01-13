New-Item -ItemType Directory -Force -Path "./build"
cmake.exe -G "Visual Studio 17 2022" -A x64 -B "./build" "." -DSMTG_CREATE_PLUGIN_LINK=0
