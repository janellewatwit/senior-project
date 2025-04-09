#include "Visualizer.h"
#include "SharedMemoryReader.cpp"

//For Debug (tis the name)
#ifdef DEBUG
    #define DEBUG_PRINT(msg) std::cout << "DEBUG: " << msg << std::endl;
#else
    #define DEBUG_PRINT(msg)
#endif



    //Constructor
    Visualizer::Visualizer(int bins, int width, int height, int sampleRate, int fftSize, int style)
    : numBins(bins), screenWidth(width), screenHeight(height), sampleRate(sampleRate), fftSize(fftSize), style(style)
    { 
        using Complex = std::complex<float>;
        this->screenHeight = static_cast<float>(height);
        DEBUG_PRINT("height " << screenHeight);
        BAR_HEIGHT_MAX = screenHeight * 0.9f; //Make it reach till the top 10%
        DEBUG_PRINT("BAR_HEIGHT_MAX " << BAR_HEIGHT_MAX);

        // Calculate bin width in Hz (useful for mapping frequencies)
        binWidth = static_cast<float>(sampleRate) / fftSize;

        rainbow = false;
        computeSize = fftSize/4; //TODO find proper scaling for this

        /*
            Color Setup -- should direct elsewhere in future due to expandability
        */
        switch (style) {
            case 1:
                // purple
                styleArr[0] = 155; styleArr[1] = 30; styleArr[2] = 200; styleArr[3] = 255;      
                break;
            case 2:
                // rainbow here
                styleArr[0] = 0; styleArr[1] = 0; styleArr[2] = 0; styleArr[3] = 0;
                rainbow = true;
                break;
            default:
                // default to white
                styleArr[0] = 255; styleArr[1] = 255; styleArr[2] = 255; styleArr[3] = 255;
                break;
        }
        
        DEBUG_PRINT("Instance Successfully Created"); 


    }


    /*
        Allocates proper size to bins and creates soundbars
        Returns: true if created, false otherwise
    */
    bool Visualizer::createScene()
    {   
        // Return error if necessary values are undefined or invalid
        if (sampleRate <= 0 || fftSize <= 0 || screenWidth <= 0 || screenHeight <= 0 || numBins <= 0)
            return false;
    
        //This is my setup
        DEBUG_PRINT("Creating Log Bins / Soundbars");
        generateLogBins();

        DEBUG_PRINT("Beginning SDL Setup");
        createSDLComponents();

    
        return true; // Success
    }

    bool Visualizer::createSDLComponents() {
        /*
            Perform SDL Setup (RATHER than in main)        
        */
       DEBUG_PRINT("Initializing");
       //Initial Setup
       if (!SDL_Init(SDL_INIT_VIDEO)) { //Removed Audio since DAW is responsible for that
           SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
           DEBUG_PRINT("Error with INIT Video");
           return false;
       }
       DEBUG_PRINT("Creating Window");
       if (!SDL_CreateWindowAndRenderer("Visualizer Test", WINDOW_LENGTH, WINDOW_HEIGHT, 0, &window, &renderer)) {
        //Passes references for window and renderer
           SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
           DEBUG_PRINT("Window not created!");
           return false;
       }
       
       return true;

    }

    /*
        Create bins responsible for holding audio (based on frequency) and representing audio
    */
    void Visualizer::generateLogBins()
    {
        float fMin = 20.0f;  // Minimum frequency we care about
        float fMax = sampleRate / 2.0f;  // Nyquist limit (limit calcuable)
        
        //Clean start
        logBins.clear();
        soundBars.clear();
        float binPixels = (screenWidth / numBins);

        //I like a little bit of seperation
        float binSeparation = binPixels * 0.1f;  // 10% spacing
        binPixels *= 0.9f;  // Adjust bin width accordingly
    
        DEBUG_PRINT("Entering Logbin Loop");
        for (int i = 0; i < numBins; i++)
        {
            //Associated frequencies for each box
            float binFreq = fMin * pow(fMax / fMin, static_cast<float>(i) / (numBins - 1));
            binFreq = std::min(binFreq, sampleRate / 2.0f); //I think?
            logBins.push_back(binFreq);
    
            // Create the regions for each bar (evenly spaced)
            SDL_FRect bar = { i * (binPixels + binSeparation) + binSeparation / 2, 0, binPixels, BAR_HEIGHT_MIN };
            soundBars.push_back(bar);
        }


        DEBUG_PRINT("Successfully created sound bars and logbins");
        DEBUG_PRINT("Size of SoundBars: " << soundBars.size());
        DEBUG_PRINT("Size of logBins: " << logBins.size());
        
    }
    



    /*
        This uses the bin index to sum the amplitudes of the frequencies that belong to that bin.
        Args: Bin index we want to process.
        Returns: Amplitude of the bin.
    */
    float Visualizer::sumAmplitude(int binIndex) {
        DEBUG_PRINT("Performing Sum Amplitude");
        // Error checking: Ensure binIndex is valid
        if (binIndex < 0 || binIndex >= numBins || freqData.empty()) {
            return 0.0f;  // Return zero if binIndex is out of range or FFT data is unavailable.
        }

        // Get the frequency range for this bin using logBins
        float freqStart = logBins[binIndex];
        float freqEnd = (binIndex < numBins - 1) ? logBins[binIndex + 1] : sampleRate / 2.0f;

        float amplitude = 0.0f;

        // Convert frequency range to FFT index range
        int startIdx = static_cast<int>(freqStart / binWidth);
        int endIdx = static_cast<int>(freqEnd / binWidth);

        // Ensure index bounds
        startIdx = std::max(0, std::min(startIdx, fftSize / 2 - 1));
        endIdx = std::max(0, std::min(endIdx, fftSize / 2 - 1));
        DEBUG_PRINT("Index Range " << startIdx << " " << endIdx );

        // Sum the magnitudes of FFT bins within the range
        for (int i = startIdx; i <= endIdx; i++) {
            DEBUG_PRINT("Real: " << freqData[i].real() << " Imaginary: " << freqData[i].imag());
            amplitude += std::abs(freqData[i]); // Take magnitude from complex number
        }

        DEBUG_PRINT("Amplitude: " << amplitude);
        return amplitude;
    }


    /*
        Responsible for updating the visualizer. Pulls updated frequency from 'insertFreq()'
        Returns: 0 on success, else error 
    */
    int Visualizer::update(const std::vector<Complex>& input)
    {
        

        DEBUG_PRINT("Updating Visualizer");
        float maxAmplitude = 1.0f; // Prevent division by zero, can be dynamically updated
        std::vector<float> amplitudes(numBins, 0.0f); // Store computed amplitudes

        DEBUG_PRINT("INPUT ------ Real: " << input[0].real() << " Imaginary: " << input[0].imag());
    
        freqData = input;

        // First pass: Compute amplitudes and track max
       for (int i = 0; i < numBins; i++)
        {
            amplitudes[i] = sumAmplitude(i);
            DEBUG_PRINT("Amplitude at " << i << " ---- " << amplitudes[i]);
            if (amplitudes[i] > maxAmplitude)
            {
                maxAmplitude = amplitudes[i];
            }
        }

        DEBUG_PRINT("BAR HEITGHT MAX: " << BAR_HEIGHT_MAX);
        
        // Second pass: Normalize and update bars
        for (int i = 0; i < numBins; i++)
        {
            float normalizedAmplitude = (amplitudes[i] / (maxAmplitude + 0.1f));  // Add small constant to prevent division by zero

            // You may want to remove the DEBUG_PRINT here for performance reasons in production
            DEBUG_PRINT("Current Amplitude: " << normalizedAmplitude)

            soundBars[i].h = BAR_HEIGHT_MIN + (normalizedAmplitude) * BAR_HEIGHT_MAX;
            DEBUG_PRINT("HEIGHT FOR " << i << " = " << soundBars[i].h);
        }

        


        return 0;
    }

    //Render the updated graphics
    void Visualizer::render(SDL_Renderer* renderer)
    {

        // Refreshes scene
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Black background
        SDL_RenderClear(renderer);

        // Draw the soundbars
        SDL_SetRenderDrawColor(renderer, styleArr[0],  styleArr[1],  styleArr[2],  styleArr[3]); // now custom and cool
        for (const auto& bar : soundBars) {
            SDL_RenderFillRect(renderer, &bar);
        }

        SDL_RenderPresent(renderer);
    }

    //Destructor
    Visualizer::~Visualizer() {
        //wave buffer not needed

        if (window) SDL_DestroyWindow(window);
        if (renderer) SDL_DestroyRenderer(renderer);

        SDL_Delay(10000);
        SDL_Quit();

    }

    /*
        This will run the specified visualizer, so if you create a visualizer and want to demonstrate it, this is how you can do it.
    */
   void Visualizer::runVisualizer() {
        DEBUG_PRINT("Creating Scene / Entering Main Runner");
        //Creates Scene (simplifies work) AND is on thread!
        if (!createScene()) {
            throw std::runtime_error("Failed to Create Scene (Missing or Invalid Data).");
        } 

        SDL_RenderClear(renderer);
        SDL_RenderPresent(renderer);

        bool isInitialized = false;
        size_t current = 0;

        SharedMemoryReader reader(1024); // Instantiate the reader
        auto lastTime = reader.getLastUpdateTimestampMs();

        SDL_Delay(1000);

        while (running.load()) {
            // Process Exit event
            SDL_Event event;
            while (SDL_PollEvent(&event)) {
                if (event.type == SDL_EVENT_QUIT) {
                    running.store(false);
                    break;
                }
            }

            // Check for new shared memory data
            auto currentTime = reader.getLastUpdateTimestampMs();

            if (currentTime > lastTime) {
                lastTime = currentTime;
                std::vector<float> newData = reader.getData(1024);  // Pull shared memory audio

                // Print some values from newData to check
                std::cout << "newData size: " << newData.size() << std::endl;
                std::cout << "First 10 values of newData:" << std::endl;
                for (std::vector<float>::size_type i = 0; i < 10 && i < newData.size(); i++) {
                    std::cout << "newData[" << i << "] = " << newData[i] << std::endl;
                }

                // Feed data into visualizer
                this->updateData(newData);

                // Initialize or update FFT processing
                if (!isInitialized || audioSamplesChanged) {
                    FFT::setup(newData);  // Re-initialize FFT with fresh data
                    DEBUG_PRINT("FFT Setup Reinitialized");
                    isInitialized = true;
                    audioSamplesChanged = false;
                }

                // Automatic handling of current
                std::vector<Complex> fftData = FFT::compute(current, newData.size());
                DEBUG_PRINT("Updating frequencies\n");
                this->update(fftData);  // Update frequencies based on FFT results

                // Rendering
                DEBUG_PRINT("Rendering Bars\n");
                this->render(renderer);
            }

            SDL_Delay(100); // ~60 FPS
        }
    }



    //Intialize Thread and begin running program.
    void Visualizer::start() {
        // If already running, do nothing
        if (running.load()) return;
        //otherwise begin thread
        running.store(true); 
        DEBUG_PRINT("Attempting to run Visualizer");
        visThread = std::thread(&Visualizer::runVisualizer, this);
    }

    //Safely exit
    void Visualizer::stop() {
        running.store(false);
        // Wait for the thread to finish if it's joinable
        if (visThread.joinable()) {
            visThread.join();
        }
    }
    


    // We need an easier interface to actually update the audio
    void Visualizer::updateData(const std::vector<float>& newData) {
        audioSamples = newData;
        audioSamplesChanged = true;
    }
    
