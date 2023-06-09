/*
MIT License

Copyright (c) 2018 Shalitha Suranga

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

/*
 * Modified slightly by CookeyDev
 */

#include <iostream>
#include <thread>
#include <chrono>
#include <atomic>

using namespace std;

class Timer {
atomic<bool> active{true};

public:
void setTimeout(auto function, int delay);
void setInterval(auto function, int interval);
void stop();
};

void Timer::setTimeout(auto function, int delay) {
    active = true;
    thread t([=]() {
        if (!active.load()) return;
        this_thread::sleep_for(chrono::milliseconds(delay));
        if (!active.load()) return;
        function();
    });
    t.detach();
}

void Timer::setInterval(auto function, int interval) {
    active = true;
    thread t([=]() {
        while (active.load()) {
            this_thread::sleep_for(chrono::milliseconds(interval));
            if (!active.load()) return;
            function();
        }
    });
    t.detach();
}

void Timer::stop() {
    active = false;
}

