#include <iostream>
#include <windows.h>
#include <chrono>
#include "../SharedMemoryData.cpp"

const wchar_t* MEM_NAME = L"Local\\MySharedMemory";

int main() {
    HANDLE hMapFile = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, MEM_NAME);
    if (!hMapFile) {
        std::cerr << "Could not open file mapping object.\n";
        return 1;
    }

    void* pBuf = MapViewOfFile(hMapFile, FILE_MAP_ALL_ACCESS, 0, 0, sizeof(SharedMemoryData));
    if (!pBuf) {
        std::cerr << "Could not map view of file.\n";
        CloseHandle(hMapFile);
        return 1;
    }

    SharedMemoryData* data = reinterpret_cast<SharedMemoryData*>(pBuf);

    std::cout << "First 10 samples from shared memory:\n";
    for (int i = 0; i < 10; ++i) {
        std::cout << data->audioData[i] << " ";
    }
    std::cout << std::endl;

    std::cin.get(); // keep window open
    UnmapViewOfFile(pBuf);
    CloseHandle(hMapFile);
    return 0;
}
