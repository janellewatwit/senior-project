#include "Visualizer.cpp"  // Or wherever your app starts

int main(int argc, char* argv[]) {
    //Begin visualizer incorporation
    Visualizer visualizer(16, WINDOW_LENGTH, WINDOW_HEIGHT, 44100, 512, 1);

    visualizer.start();

    return 0;
}
