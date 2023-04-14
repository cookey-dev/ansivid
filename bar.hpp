#ifndef BAR_HPP
#define BAR_HPP

#include <chrono>
#include <iostream>
#include <curses.h>
#include <vector>
#include "timer.hpp"
using namespace std;


class Bar {
public:
Bar(const string message) {
	initscr();
	set_message(message);
	loop.setInterval([&]() mutable {
		update(frames.at(i));
		if (i == frames.size() - 1) i = 0;
		else i++;
	}, 200);
	noecho();
}

void set_message(const string message) {
	msg = message;
}

void stop() {
	cancel = false;
	endwin();
}
private:
void update(string txt) {
	clear();
	move(0, 0);
	printw(txt.c_str());
	refresh();
}
string msg;
bool cancel;
int i = 0;
Timer loop;
std::vector<string> frames = {"|", "/", "-", "\\", "|", "/", "-", "\\"};
};

#endif
