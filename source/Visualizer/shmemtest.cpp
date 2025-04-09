#include "Visualizer.cpp" 

int main(int argc, char* argv[]) {
    //Begin visualizer incorporation
    Visualizer visualizer(16, WINDOW_LENGTH, WINDOW_HEIGHT, 44100, 1024, 1);

    visualizer.start();

    return 0;
}
