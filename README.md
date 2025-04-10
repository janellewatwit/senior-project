# Sounds Magic
A virtual synthesizer (VST3) plugin with unique audio visualizations.

## Clone
This repo uses submodules for dependencies. You must clone with the `--recursive` flag.
Do not modify files in the `vendor/` directory as they belong to other repositories.
```cmd
git clone --recursive git@github.com:janellewatwit/senior-project.git
```

**Note**
    A recent update changed the clone method, instead try this:
```cmd
    git clone --recursive https://github.com/janellewatwit/senior-project.git
```
    Code is still messy but it works.

## Build
Install [CMake](https://cmake.org/download/) and [Visual Studio 2022](https://visualstudio.microsoft.com/vs/).

Generate project files for Visual Studio 2022. The resulting solution will be at `build/SoundsMagic.sln`.
```cmd
./scripts/generate.ps1
```

You can open `SeniorProject.sln` and build with Visual Studio, or build with CMake:
```cmd
./scripts/build.ps1
```
