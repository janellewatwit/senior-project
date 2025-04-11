#include "Visualizer.h"
#include "SharedMemoryReader.cpp" //This should be updated

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
        /*
            Calculations were weird so this was for debugging
        */
        this->screenHeight = static_cast<float>(height);
        DEBUG_PRINT("height " << screenHeight);
        BAR_HEIGHT_MAX = screenHeight * 0.9f; //Make it reach till the top 10%
        DEBUG_PRINT("BAR_HEIGHT_MAX " << BAR_HEIGHT_MAX);

        // This is a weak estimate of binwidth
        binWidth = static_cast<float>(sampleRate) / fftSize;

        //Originally we had in mind different styles
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
    

        //Todo -- should wrap exit logic around these
        DEBUG_PRINT("Creating Log Bins / Soundbars");
        generateLogBins();

        DEBUG_PRINT("Beginning SDL Setup");
        createSDLComponents();

    
        return true; // Success
    }

    /*
        Perform SDL Setup (RATHER than in main)    
            
        These are mostly standard SDL proceedures, See the playlist
        from Mike shah and other implementations
    */
    bool Visualizer::createSDLComponents() {

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
        float fMin = 20.0f;  // Minimum frequency we care about (Most low frequencies are inaudible)
        float fMax = sampleRate / 2.0f;  // Nyquist limit, maximum calcuable frequency
        
        //Clean start
        logBins.clear();
        soundBars.clear();
        float binPixels = (screenWidth / numBins); // Distribute the screenwidth among the bars

        //I like a little bit of seperation -- Cosmetic choice
        float binSeparation = binPixels * 0.1f;  // 10% spacing
        binPixels *= 0.9f;  // Adjust bin width accordingly
    

        DEBUG_PRINT("Entering Logbin Loop");
        for (int i = 0; i < numBins; i++)
        {
            //Associated frequency ranges for each box, we can use the previous one to create the actual bounds
            float binFreq = fMin * pow(fMax / fMin, static_cast<float>(i) / (numBins - 1)); //Slowly gets larger as we increase
            binFreq = std::min(binFreq, sampleRate / 2.0f); //Account for end of frequency range as to not exceed (cannot be represented)
            logBins.push_back(binFreq);
    
            // Create virtual representation of the frequencies based on calculated widths
            SDL_FRect bar = { i * (binPixels + binSeparation) + binSeparation / 2, 0, binPixels, BAR_HEIGHT_MIN };
            soundBars.push_back(bar);
        }

        //Ensure that they were creating these bins properly
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
        //TODO make these global
        float fMin = 20.0f;
        float fMax = sampleRate/2.0f;
        
        DEBUG_PRINT("Performing Sum Amplitude");

        //Ensure we're working with something
        if (binIndex < 0 || binIndex >= numBins || freqData.empty()) {
            return 0.0f; 
        }

        // Get the frequency range for this bin using logBins
        float freqStart;
        float freqEnd = logBins[binIndex];
        if(binIndex == 0) //if it is the first one, account for lower frequency, We did the calculation above 
        {
            
            freqStart = fMin;
        }
        else
        {
            freqStart = logBins[binIndex-1];
        }   //updated and cool


        float amplitude = 0.0f;

        //This was originally done not well, since we relied on a non scaling number to calculate the bins for a scaling number
        //We can use the formual from above to find it instead, so its slightly slower, but much more accurate.
        int startIdx = static_cast<int>((log(freqStart / fMin) / log(fMax / fMin)) * (numBins - 1));
        int endIdx = static_cast<int>((log(freqEnd / fMin) / log(fMax / fMin)) * (numBins - 1));

        //This also needed to be changed, It looks like this: 0 < index < numbins-1
        startIdx = std::max(0, std::min(startIdx, numBins - 1));
        endIdx = std::max(0, std::min(endIdx, numBins - 1));


        DEBUG_PRINT("Index Range " << startIdx << " " << endIdx );

        //Finally, we can get the total amplitude by summing the wave amplitudes
        for (int i = startIdx; i <= endIdx; i++) {
            DEBUG_PRINT("Real: " << freqData[i].real() << " Imaginary: " << freqData[i].imag());
            amplitude += std::abs(freqData[i]); // Take magnitude from complex number
            //We need absolute since waves are both negative and positive overtime, If we subtract negative
            //we lose important values and likely end with 0s
        }

        DEBUG_PRINT("Amplitude: " << amplitude);
        return amplitude;
    }


    /*
        Responsible for updating the visualizer. Pulls updated frequency from 'insertFreq()'
        Returns: 0 on success, else error 

        TODO fix this
    */
    int Visualizer::update(const std::vector<Complex>& input)
    {
        

        DEBUG_PRINT("Updating Visualizer");
        float maxAmplitude = 1.0f; // Prevent overflow mostly
        std::vector<float> amplitudes(numBins, 0.0f); // Store computed amplitudes, auto populate

        DEBUG_PRINT("INPUT ------ Real: " << input[0].real() << " Imaginary: " << input[0].imag());
    
        freqData = input; //This allows the data to be accessed accross the class

        // First, get the max ampltiude (what we use to base the normalization on)
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
        
        // Second, normalize the bars by the maxAmplitude
        for (int i = 0; i < numBins; i++)
        {
            //Sets scale of 0-1.1
            float normalizedAmplitude = (amplitudes[i] / (maxAmplitude + 0.1f));  // Add small constant to prevent division by zero

            DEBUG_PRINT("Current Amplitude: " << normalizedAmplitude)

            //Set the visual element to correspond (SDL FRect height), lock between bar height constraints
            soundBars[i].h = BAR_HEIGHT_MIN + (normalizedAmplitude) * BAR_HEIGHT_MAX;
            DEBUG_PRINT("HEIGHT FOR " << i << " = " << soundBars[i].h);
        }

        return 0;
    }

    //Render the updated graphics
    void Visualizer::render(SDL_Renderer* renderer)
    {

        // Refreshes scene -- If we kept drawing over and over again, it draws over and makes a sort of blur effect
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Black background
        SDL_RenderClear(renderer);

        // Draw the soundbars -- SDL takes care of this primarily
        SDL_SetRenderDrawColor(renderer, styleArr[0],  styleArr[1],  styleArr[2],  styleArr[3]); // now custom and cool
        for (const auto& bar : soundBars) {
            SDL_RenderFillRect(renderer, &bar);
        }

        //Ships to window
        SDL_RenderPresent(renderer);
    }

    //Destructor
    Visualizer::~Visualizer() {
        //wave buffer not needed anymore, so perform removal of SDL items

        if (window) SDL_DestroyWindow(window);
        if (renderer) SDL_DestroyRenderer(renderer);

        //Delay in case
        SDL_Delay(10000);
        SDL_Quit();

        //Most of what we have is vectors, so there is not much to clean up
    }

    /*
        This will run the specified visualizer, so if you create a visualizer and want to demonstrate it, this is how you can do it.

        Note, this is a mess and REALLY needs to be fixed.
    */
   void Visualizer::runVisualizer() {
        DEBUG_PRINT("Creating Scene / Entering Main Runner");
        //Creates Scene (simplifies work) AND is on thread!
        if (!createScene()) {
            throw std::runtime_error("Failed to Create Scene (Missing or Invalid Data).");
        } 

        //Originally, we used FFT::Setup, though since this needs to run constantly, I do not think this matters as much
        bool isInitialized = false;
        size_t current = 0; //Keep track of our position in audio data

        SharedMemoryReader reader(1024); // Instantiate the reader, code breaks here if sounds magic is not on
        auto lastTime = reader.getLastUpdateTimestampMs(); // Initialize original time

        SDL_Delay(1000); //This was used for testing, May remove?

        // Cross thread reference to keep this running
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

            //We use the timestamp attached to data to see if the writer has done anything
            if (currentTime > lastTime) { //Since each side is independent(writer / reader), we don't really need as much safety
                lastTime = currentTime;
                std::vector<float> newData = reader.getData(1024);  // Pull shared memory audio

                // Print some values from newData to check
                DEBUG_PRINT("newData size: " << newData.size());
                DEBUG_PRINT("First 10 values of newData:");
                for (std::vector<float>::size_type i = 0; i < 10 && i < newData.size(); i++) {
                    DEBUG_PRINT("newData[" << i << "] = " << newData[i]);
                }

                // Feed data into visualizer this is redundant, but it works, so I really do not want to touch it
                this->updateData(newData); // After touching, I lost 1 hour of time, so I am going to be very patient

                // This is likely what should be changed. Since we're passing data in twice before actually computing it
                if (!isInitialized || audioSamplesChanged) {
                    FFT::setup(newData); 
                    DEBUG_PRINT("FFT Setup Reinitialized");
                    isInitialized = true;
                    audioSamplesChanged = false;
                } //TODO update using updateData, and change updateData in FFT

                //TODO decide proper logic here. We are passing in consistent data so we can probably just base it on 
                //the buffer size, current isn't even changing.. but again, jenga, if I fix this I break everything else
                std::vector<Complex> fftData = FFT::compute(current, newData.size());


                DEBUG_PRINT("Updating frequencies\n");
                this->update(fftData);  // Update frequencies based on FFT results

                // Rendering
                DEBUG_PRINT("Rendering Bars\n");
                this->render(renderer);
            }

            SDL_Delay(32); // ~60 FPS -- For testing this was changed but it runs fast enough.
        }
    }



    //intialize thread and start program on said thread
    void Visualizer::start() {

        // If already running, do nothing 
        if (running.load()) return;

        //otherwise begin thread
        running.store(true); 
        DEBUG_PRINT("Attempting to run Visualizer");
        visThread = std::thread(&Visualizer::runVisualizer, this);
    }

    //Exit loop
    void Visualizer::stop() {
        running.store(false);

        // I did not think of this, I had asked about general threading tips from GPT
        if (visThread.joinable()) {
            visThread.join();
        }
    }
    


    // We need an easier interface to actually update the audio IE do incrementing NOT in main loop! 
    //its so hard to debug now but I understand it all!
    void Visualizer::updateData(const std::vector<float>& newData) {
        audioSamples = newData;
        audioSamplesChanged = true;
    }
    
