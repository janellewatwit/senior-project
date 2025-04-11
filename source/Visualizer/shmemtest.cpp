#include "Visualizer.cpp" 


/*
    This is an example of usage

    in foresight, creating a pipe for the visualizer to feed in data may be better
    Since, we are currently locked to only the shared memory. 

*/
int main(int argc, char* argv[]) {

    //Arguments are: Bin numbers, window size, samplerate, fftsize, and style
    Visualizer visualizer(16, WINDOW_LENGTH, WINDOW_HEIGHT, 22000, 1024, 0);

    visualizer.start();

    return 0;
}
