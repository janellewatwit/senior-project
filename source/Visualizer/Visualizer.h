#pragma once
#include "FFT.cpp"
#include <SDL3/SDL.h>
#include <stdio.h>
#include <iostream>
#include <chrono>

//threading stuff
#include <thread>
#include <atomic> // how to stop thread
#include <mutex> //prevents race condition


//building no work I try this: 

SDL_Window* window;
SDL_Renderer* renderer;
SDL_AudioStream* stream;


inline const int WINDOW_HEIGHT = 480;
inline const int WINDOW_LENGTH = 640;


class Visualizer {
public:
    using Complex = std::complex<float>;

    // Constructor & Destructor
    Visualizer(int bins, int width, int height, int sampleRate, int fftSize,int style);
    ~Visualizer();

    // Creates necessary rectangles for visualization
    bool createScene();

    // Updates the sound bars based on frequency and amplitude data
    int update(const std::vector<Complex>& input);

    // Renders the bars using SDL
    void render(SDL_Renderer* renderer);

    //Added to reduce required code
    bool createSDLComponents();


    //Updates
    void start();

    void stop();

    //Specifically updates audioData - direct step b4 processing 
    void updateData(const std::vector<float>&newData);

    //Communication with VST
    bool audioSamplesChanged = false;
    std::mutex dataMutex;
    std::atomic<bool> running{false};

    //Shared buffer listener
    std::vector<float> audioSamples;

private:
    // Calculates the total amplitude in a given bin
    float sumAmplitude(int binIndex);

    //Responsible for estabilshing both frequency bins and pixels for soundbars
    void generateLogBins();

    void runVisualizer(); //since we use start + stop now for threading

    //threading related stuff
    std::thread visThread; 

    std::vector<std::complex<float>> freqData;  // Stores (frequency, amplitude) pairs
    std::vector<SDL_FRect> soundBars; // SDL rectangles for visualization
    std::vector<float> logBins;

    //Needed for Loading a wav
    //SDL_AudioSpec wavSpec; //Specs of your setup, sample rate and channgels I believe?
    //Uint8* wavBuffer;   //Buffer of EVERY sample, 0-255 (only for testing)
    //Uint32 wavLength; //How long is the audio?

    float computeSize;
    bool rainbow;

    int styleArr[4];

    int numBins;        // Number of frequency bins
    int screenWidth;    // Screen width
    int screenHeight;   // Screen height
    int sampleRate;     // Audio sample rate (e.g., 44100 Hz)
    int fftSize;        // FFT window size (e.g., 1024)
    int style;
    float binWidth;
    float BAR_HEIGHT_MAX;

    //Its a stylistic thing more than anything practical
    const float BAR_HEIGHT_MIN = 5.0f;




};
