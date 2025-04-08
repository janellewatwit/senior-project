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

class FFT {
public:
    using Complex = std::complex<float>;

    static void setup(std::vector<float>& input);
    static std::vector<Complex> compute(int start, int inc);
    static void insertData(const std::vector<float>&);

private:
    static std::vector<Complex> audioData;
    static std::vector<Complex> realToComplex(const std::vector<float>& input);
    static void bitReverseReorder(std::vector<Complex>& data);
    static int nextPowerOfTwo(int n);
    static std::vector<float> padToPowerOfTwo(const std::vector<float>& input);
};

