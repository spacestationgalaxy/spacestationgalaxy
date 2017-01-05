//
// Created by Nick on 1/4/2017.
//

#include <iostream>

using namespace std;

const char* gszTitle[32] = {
        "==============================",
        "== SpaceStationGalaxy  v1.0 ==",
        "==============================",
        "====        CLIENT        ====",
        "=============================="
};

int main(int argc, char** argv)
{
    for (int i = 0; i < 5; ++i)
        cout << gszTitle[i] << endl;
    cout << endl;

    return 0;
}