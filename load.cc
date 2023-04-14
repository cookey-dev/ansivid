#include <iostream>
#include <fstream>
#include <vector>
#include <thread>
#include "timercpp.h"
#include "audio.hpp"

using namespace std;

int main() {
    string current;
    string currentFrame;
    vector<string> vid;
    ifstream Video("out.txt");
    while (getline(Video, current)) {
        size_t find = current.find("nf");
        if (find != string::npos) {
            vid.push_back(currentFrame);
            currentFrame = "";
        } else {
            currentFrame += current;
        }
    }
    Video.close();

    string _infoCurrent;
    string _fps;
    ifstream FPS("fps.txt");
    while (getline(FPS, _infoCurrent)) {
        _fps = _infoCurrent;
    }
    int fps = stoi(_fps);

    thread audio(playmp3, "out.mp3"); // TODO: Fix core dump

    Timer t = Timer();
    int i = 0;
    bool stop = false;
    t.setInterval([&]() {
        if (i == vid.size()) {
            stop = true;
            t.stop();
        } else {
            cout << vid.at(i).c_str();
        }
        i++;
    }, 1000 / fps);

    while (!stop) {};

    return 0;
}
