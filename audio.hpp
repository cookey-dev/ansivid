#pragma once
#include <iostream>
#include <string>
#include "exec.hpp"

#define PCM_DEVICE "default"

using namespace std;

void playmp3(string fname) {
    exec(("mpg123 -q " + fname).c_str());
}