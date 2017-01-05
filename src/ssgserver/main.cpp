//
// Created by Nick on 1/4/2017.
//
#include <iostream>

const char* gszTitle[32] = {
    "==============================",
    "== SpaceStationGalaxy  v1.0 ==",
    "==============================",
    "====  DEDICATED   SERVER  ====",
    "=============================="
};

int main(int argc, char** argv)
{
    for (int i = 0; i < 5; ++i)
        std::cout << gszTitle[i] << std::endl;
    std::cout << std::endl;

    std::cout << "IP Address: " << "192.168.1.1\n";
    std::cout << "Port: " << "1024\n";
    std::cout << "\n";

    return 0;
}
