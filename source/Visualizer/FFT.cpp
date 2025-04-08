#include "FFT.h"

//Default debug statement I now love
#ifdef DEBUG
    #define DEBUG_PRINT(msg) std::cout << "DEBUG: " << msg << std::endl;
#else
    #define DEBUG_PRINT(msg)
#endif

    /*
        Respoinsible for performing Fast Fourier Transformation
        MUST be called after setup (Setup initally loads data)

        Start >> where the data has left off
        Inc >> The amount to increase / end
    
    */

    std::vector<FFT::Complex> FFT::audioData;

   std::vector<FFT::Complex> FFT::compute(int start, int inc) {
        DEBUG_PRINT("Beginning FFT\n");

        if (audioData.empty()) {
            DEBUG_PRINT("Is Empty!");
            return {}; // Early return for empty input
        }

        // Ensure the 'start' and 'inc' values are valid
        if (start < 0 || start >= audioData.size() || inc <= 0) {
            DEBUG_PRINT("Invalid start or increment value");
            return {};
        }

        //How much of total data to compute (if we pass the entireity of the data in the testing case.)
        int end = std::min(start + inc, static_cast<int>(audioData.size())); //Accomodates left overs
        int n = end - start;

        // Slice the audioData to process only the segment from start to start+inc
        std::vector<Complex> segment(audioData.begin() + start, audioData.begin() + end);



        // Iterative Cooley-Tukey FFT on the segment
        int m = 1;
        while (m < n) {
            Complex phaseShift = std::polar(1.0f, static_cast<float>(-2.0f * 3.1415f / m)); // Twiddle factor >> This is the phase shift for cosine and sine
            // printf("m = %d, phaseShift = (%f, %f)\n", m, phaseShift.real(), phaseShift.imag()); For printing if any weirdness. Verify valid vlaues from the above.

            // FFT computation for this segment
            for (int k = 0; k < n; k += m) { //Decides the waves to calculate, notice we skip more waves later (FFT)
                Complex w = 1;
                for (int j = 0; j < m / 2; j++) {
                    if (j < m) {
                        //We segment in order to get the seperate parts
                        Complex t = w * segment[k + j + m / 2]; //Butterfly operation (Seperates FFT from DFT)
                        Complex u = segment[k + j];
                        segment[k + j] = u + t;
                        segment[k + j + m / 2] = u - t;
                        w *= phaseShift;
                    }
                }
            }

            m *= 2;  // Double m at each stage (additional seperation from DFT)
        }

        DEBUG_PRINT("Complete computation");
        return segment;  // Return only the FFT result for this segment
    }


    // Set up for FFT, converting real input to complex and reordering
    void FFT::setup(std::vector<float>& input) {
        int n = input.size();
        if (n <= 1) {
            printf("Input size too small for FFT, returning early.\n");
            return;
        }
        
        
        audioData = realToComplex(input);  // Convert to complex
        DEBUG_PRINT("Converted to Complex");
    
    }

    //update data with slight overlap!
    
    // Keeps unmodified time-domain signal with overlap
    void FFT::insertData(const std::vector<float>& input) {
        // Remove the non-overlapping part
        size_t overlapSize = audioData.size() / 2;

        if (audioData.size() > overlapSize) {
            audioData.erase(audioData.begin(), audioData.begin() + (audioData.size() - overlapSize));
        }

        // Append new samples
        for (float sample : input) {
            audioData.emplace_back(sample, 0.0f);
        }

        // Keep size as power of 2 (for FFT later)
        size_t newSize = 1;
        while (newSize * 2 <= audioData.size()) {
            newSize *= 2;
        }
        audioData.resize(newSize);
    }


    // Convert real input to complex (imaginary part = 0)
    std::vector<FFT::Complex> FFT::realToComplex(const std::vector<float>& input) {
        std::vector<Complex> complexInput(input.size());
        for (size_t i = 0; i < input.size(); ++i) {
            complexInput[i] = Complex(input[i], 0.0f);
        }
        return complexInput;
    }

    // Bit-reversal reorder (for Cooley-Tukey FFT)
    void FFT::bitReverseReorder(std::vector<Complex>& data) {
        int n = data.size();
        int bits = 0;
    
        // Find the number of bits needed (for power of 2, would be log2(n))
        while ((1 << bits) < n) bits++;

        int j = 0;
        for (int i = 0; i < n; i++) {
    
            if (i < j) {
                std::swap(data[i], data[j]);
            }
    
            int mask = n >> 1; // Initial mask is half the size of n
    
            // Prevent the infinite loop by checking if mask is zero
            while (j >= mask && mask > 0) { // Added check for mask > 0
                j -= mask;   // Reduce j by mask until it's less than mask
                mask >>= 1;   // Shift mask to the right (halve it)
            }
    
            j += mask;  // Increase j by the current mask
        }
    
    }
    
    
    
    
    

    // Return the next power of two for padding
    int FFT::nextPowerOfTwo(int n) {
        if (n == 0) return 1;
        return 1 << (int)std::ceil(std::log2(n));  //Round up to next log 2 power
    }

    /*
        FFT Only works on powers of 2, so we pad it. This does skew results, but if fft is large enough it wont matter
    */
    std::vector<float> FFT::padToPowerOfTwo(const std::vector<float>& input) {
        int n = input.size();
        int nextPow2 = nextPowerOfTwo(n);

        std::vector<float> paddedInput(nextPow2, 0.0f);
        std::copy(input.begin(), input.end(), paddedInput.begin());

        DEBUG_PRINT("Padding data to size: " << nextPow2);
        return paddedInput;
    }
