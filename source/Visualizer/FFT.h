#pragma once
#include <vector>
#include <complex>
#include <cmath>
#include <algorithm>
#include <iostream>

#ifdef DEBUG
    #define DEBUG_PRINT(msg) std::cout << "DEBUG: " << msg << std::endl;
#else
    #define DEBUG_PRINT(msg)
#endif

/*
    This class is created in part with Open ai - GPT 4, Testing and debugging was performed both by
    us and the model.

    Unbenownst to me, I could have done the simpler DFT since we would not need to process all that much audio for accuracy, 
    though this was good practice and very interesting to learn about, FFT has many applications
*/
class FFT {
public:

    using Complex = std::complex<float>; //We can represent magnitude and phase of signal using this
    static void setup(std::vector<float>& input); //Original starter for FFT, Generally obselete from Insert data
    static std::vector<Complex> compute(int start, int inc); //Performed FFT on supplied data (Needs to be after setup or insertdata)
    static void insertData(const std::vector<float>&); //Inserts data while keeping a handful of previous data to smooth calculation

private:

    static std::vector<Complex> audioData; //Where data ready to be computed is stored
    static std::vector<Complex> realToComplex(const std::vector<float>& input); //Convert a float vector into complex
    static void bitReverseReorder(std::vector<Complex>& data); //Required for Cooley Tukey algorithm, reverses the bits of each number
    //The reason this is importnat is due to how FFT will pair these later.

    static int nextPowerOfTwo(int n); // Calculates next power of 2
    static std::vector<float> padToPowerOfTwo(const std::vector<float>& input); //Useful for when the number of elements is
    //unpredicable, meaning you can always insert fitted data.
};

